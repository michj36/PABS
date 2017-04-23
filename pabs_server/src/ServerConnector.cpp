/**
 * @author Sebastian Widemajer
 * last update: 22.04.2017
 *
 */
#include "ServerConnector.h"

ServerConnector::ServerConnector(unsigned s)
{
	sock = s;
	quit = false;
	scan_command = false;
	ended = false;
	user_end = true;
}

ServerConnector::~ServerConnector()
{
	stop();
}
void ServerConnector::start()
{
	thread = std::thread(&ServerConnector::run, this);
}
void ServerConnector::stop()
{
	quit = true;
	thread.join();
}

void ServerConnector::scanBegin()
{
	scan_command = true;
}

void ServerConnector::scanEnd()
{
	scan_command = false;
}

int ServerConnector::readAll(unsigned s, pabs::generic_msg* m)
{
	int to_read = sizeof(pabs::msg);
	int bytes_read = 0;
	while(bytes_read != to_read) {
		int ret;
		struct pollfd fd;
		fd.fd = sock;
		fd.events = POLLIN;
		ret = poll(&fd, 1, 10000);
		if(ret > 0) {
			int r = read(sock, m + bytes_read, to_read - bytes_read);
			if(r)
				bytes_read += r;
			else
				return 1;
		} else
			return -1;
	}
	return 0;
}

void ServerConnector::run()
{
	std::cout << "Received connection from " << sock << std::endl;
	pabs::generic_msg m;
	state = WAITING_HELLO;
	int ret;
	while(!quit) {
		switch(state) {
		case WAITING_HELLO:
			//read(c, &m, sizeof(pabs::generic_msg));
			readAll(sock, &m);
			if(m.type == pabs::HELLO) {
				std::cout << "Received HELLO from " << sock << std::endl;
				m.type = pabs::OLLEH;
				write(sock, &m, sizeof(pabs::msg));
				state = CHECK_AGENT;
			} else {
				m.type = pabs::ERR;
				write(sock, &m, sizeof(pabs::msg));
				throw std::runtime_error("Unexpected agent message when awaiting HELLO");
			}
			break;
		case CHECK_AGENT:
			sleep(1);
			m.type = pabs::HW_RU;
			write(sock, &m, sizeof(pabs::msg));
			state = CHECK_AGENT_RESPONSE;
			std::cout << "HW_RU to " << sock << std::endl;
			break;
		case CHECK_AGENT_RESPONSE:
			ret = readAll(sock, &m);
			switch(ret) {
			case -1:
				//Timeout
				throw std::runtime_error("Poll error or timeout at check");
				break;
			case 0:
				//Correct
				break;
			case 1:
				//EOF
				throw std::runtime_error("EOF");
				break;
			default:
				break;
			}
			if(m.type == pabs::OK) {
				std::cout << "Received OK from " << sock << std::endl;
				if(scan_command)
					state = SCAN_BEGIN;
				else
					state = CHECK_AGENT;
			} else if(m.type == pabs::AGENT_END) {
				std::cout << "Received AGENT_END from " << sock << std::endl;
				user_end = false;
				state = QUIT;
			} else if(m.type == pabs::ERR) {
				std::cout << "Received ERR from " << sock << std::endl;
				user_end = false;
				state = QUIT;
			} else {
				throw std::runtime_error("Unexpected agent response at check");
			}
			break;
		case SCAN_BEGIN:
			std::cout << "Starting scan on " << sock << std::endl;
			m.type = pabs::START_SCN;
			write(sock, &m, sizeof(pabs::msg));
			state = SCAN_BEGIN_RESPONSE;
			break;
		case SCAN_BEGIN_RESPONSE:
			ret = readAll(sock, &m);
			switch(ret) {
			case -1:
				//Timeout
				throw std::runtime_error("Poll error or timeout at check");
				break;
			case 0:
				//Correct
				break;
			case 1:
				//EOF
				throw std::runtime_error("EOF");
				break;
			default:
				break;
			}
			if(m.type == pabs::OK) {
				state = SCAN_RECEIVING_DATA;
			} else {
				pabs::printType((pabs::msg_type)m.type);
				std::cout << std::endl;
				throw std::runtime_error("Unexpected agent response at scan start");
			}
			break;
		case SCAN_RECEIVING_DATA:
			ret = readAll(sock, &m);
			switch(ret) {
			case -1:
				//Timeout
				throw std::runtime_error("Poll error or timeout at check");
				break;
			case 0:
				//Correct
				break;
			case 1:
				//EOF
				throw std::runtime_error("EOF");
				break;
			default:
				break;
			}
			if(m.type == pabs::DATA) {
				std::cout << "DATA sock " << sock << ":" << std::endl;
				pabs::printMsg((pabs::data_msg*)&m);
				if(!scan_command) state = SCAN_STOP;
			} else {
				pabs::printType((pabs::msg_type)m.type);
				throw std::runtime_error("Unexpected agent response at scan");
			}
			break;
		case SCAN_STOP:
			std::cout << "Stopping scan on " << sock << std::endl;
			m.type = pabs::ENOUGH;
			write(sock, &m, sizeof(pabs::msg));
			state = SCAN_STOP_RESPONSE;
			break;
		case SCAN_STOP_RESPONSE:
			ret = readAll(sock, &m);
			switch(ret) {
			case -1:
				//Timeout
				throw std::runtime_error("Poll error or timeout at check");
				break;
			case 0:
				//Correct
				break;
			case 1:
				//EOF
				throw std::runtime_error("EOF");
				break;
			default:
				break;
			}
			if(m.type == pabs::DATA)
				continue;
			if(m.type == pabs::OK) {
				std::cout << "Scan stopped on " << sock << std::endl;
				state = CHECK_AGENT;
			} else {
				throw std::runtime_error("Unexpected agent response at scan");
			}
			break;
		case QUIT:
			quit = true;
			break;
		default:
			throw std::runtime_error("Unknown server state");
		}
	}
	if(user_end) {
		m.type = pabs::SERVER_END;
		write(sock, &m, sizeof(pabs::msg));
	}
	std::cout << "Closing connection with " << sock << std::endl;
	close(sock);
	ended = true;
}
