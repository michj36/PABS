/**
 * @author Sebastian Widemajer
 * last update: 22.04.2017
 *
 */
#include "AgentConnector.h"

AgentConnector::AgentConnector(char * name)
{
	int l = 0;
	while(name[l] != '\0') l++;
	hostname = new char[l];
	l = 0;
	while(name[l]) {
		hostname[l] = name[l];
		l++;
	}
	asocket();
	aconnect();
	m.ver = 1;
}

AgentConnector::~AgentConnector()
{
	close(sock);
}
int AgentConnector::readCheck()
{
	int ret;
	struct pollfd fd;
	fd.fd = sock;
	fd.events = POLLIN;
	ret = poll(&fd, 1, 100); // >0 można czytać, 0 - timeout, -1 err
	return ret;
}
void AgentConnector::end()
{
	m.type = pabs::AGENT_END;
	write(sock, &m, sizeof(pabs::msg));
	close(sock);
}

int AgentConnector::readAll()
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
			int r = read(sock, &m + bytes_read, to_read - bytes_read);
			if(r)
				bytes_read += r;
			else
				return 1;
		} else
			return -1;
	}
	return 0;
}
void AgentConnector::run()
{
	state = CONNECT;
	while(!quit) {
		switch(state) {
		case CONNECT:
			m.type = pabs::HELLO;
			write(sock, &m, sizeof(pabs::msg));
			state = WAITING_HELLO;
			break;
		case WAITING_HELLO:
			//read(sock, &m, sizeof(pabs::msg));
			readAll();
			if(m.type == pabs::OLLEH) {
				std::cout << "Received OLLEH from server\n";
				state = LISTENING;
			} else {
				m.type = pabs::ERR;
				write(sock, &m, sizeof(pabs::msg));
				throw std::runtime_error("Unexpected server message when awaiting OLLEH");
			}
			break;
		case LISTENING:
			//read(sock, &m, sizeof(pabs::msg));
			readAll();
			switch(m.type) {
			case pabs::START_SCN:
				std::cout << "Received START_SCN from server\n";
				m.type = pabs::OK;
				write(sock, &m, sizeof(pabs::msg));
				state = SENDING_DATA;
				break;
			case pabs::HW_RU:
				std::cout << "Received HW_RU from server\n";
				m.type = pabs::OK;
				write(sock, &m, sizeof(pabs::msg));
				break;
			case pabs::SERVER_END:
				std::cout << "Received SERVER_END from server\n";
				user_end = false;
				state = QUIT;
				break;
			case pabs::ERR:
				std::cout << "Received ERR from server\n";
				state = QUIT;
				break;
			default:
				m.type = pabs::ERR;
				write(sock, &m, sizeof(pabs::msg));
				pabs::printType((pabs::msg_type) m.type);
				throw std::runtime_error("Unexpected server message when listening");
			}
			break;
		case SENDING_DATA:
			usleep(500000);
			m.type = pabs::DATA;
			std::cout << "Sending DATA to server\n";
			getData();
			write(sock, &m, sizeof(pabs::msg));
			if(readCheck() > 0) {
				readAll();
				switch(m.type) {
				case pabs::ENOUGH:
					m.type = pabs::OK;
					write(sock, &m, sizeof(pabs::msg));
					state = LISTENING;
					break;
				case pabs::SERVER_END:
					m.type = pabs::OK;
					write(sock, &m, sizeof(pabs::msg));
					state = QUIT;
					break;
				case pabs::ERR:
					state = QUIT;
					break;
				default:
					throw std::runtime_error("Unexpected server message when sending DATA");
				}
			}
			break;
		case QUIT:
			quit = true;
			break;
		default:
			throw std::runtime_error("Agent state error");
		}
	}
	if(user_end) {
		m.type = pabs::AGENT_END;
		write(sock, &m, sizeof(pabs::msg));
	}
}

void AgentConnector::asocket()
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1) {
		throw std::runtime_error("opening datagram socket");
	}
	hp = gethostbyname(hostname);
	if(hp == (struct hostent *) 0) {
		throw std::runtime_error("unknown host");
	}
	memcpy((char *) &name.sin_addr, (char *) hp->h_addr, hp->h_length);
	name.sin_family = AF_INET;
	name.sin_port = htons(atoi("6666"));
}

void AgentConnector::aconnect()
{
	if(connect(sock, (struct sockaddr*) &name, sizeof name) == -1) {
		throw std::runtime_error("Error connecting to server");
	};
}
int AgentConnector::getData()
{
	m.bytes = 100;
	m.pkts = 666;
	m.time = time++;

	return 0;
}
