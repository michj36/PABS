/**
 * @author Sebastian Widemajer
 * last update: 22.04.2017
 *
 */
#include "Listener.h"

Listener::Listener(): thread()
{
	lsocket();
	lbind();
	llisten();
	quit = false;
}

Listener::~Listener()
{
	for(auto c = clients.begin(); c != clients.end(); c++) {
		(*c)->stop();
	}
	thread.join();
}


void Listener::lsocket()
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1) {
		throw std::runtime_error("opening datagram socket");
	}
	name.sin_family = AF_INET;
	name.sin_addr.s_addr = INADDR_ANY;
	name.sin_port = htons(6666);
}

void Listener::lbind()
{
	if(bind(sock, (struct sockaddr *)&name, sizeof name) == -1) {
		throw std::runtime_error("binding datagram socket");
	}
	length = sizeof(name);
	if(getsockname(sock, (struct sockaddr *) &name, &length) == -1) {
		throw std::runtime_error("getting socket name");
	}
}

void Listener::llisten()
{
	if(listen(sock, 128) != 0)
		throw std::runtime_error("listen call error");
}

void Listener::laccept()
{
	unsigned s;
	if((client_sock = accept(sock, (struct sockaddr*) &client, &s)) == 1) {
		throw std::runtime_error("accept error");
	}
}
int Listener::lselect()
{
	int s;
	FD_ZERO(&sset);
	FD_SET(sock, &sset);
	struct timeval timeout;
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;
	s = select(sock + 1, &sset, NULL, NULL, &timeout);
	if(s == -1) {
		throw std::runtime_error("accept error");
	}
	return s;
}
void Listener::start()
{
	thread = std::thread(&Listener::run, this);
}

void Listener::run()
{
	std::cout << "Accepting now...\n";
	while(!quit) {
//		laccept();
//		//std::thread *n = new std::thread(connection, client_sock);
//		if(clients.size() < 10) {
//			ServerConnector *cconn = new ServerConnector(client_sock);
//			cconn->start();
//			clients.push_back(cconn);
//		}
		if(lselect())
			if(FD_ISSET(sock, &sset)) {
				laccept();
				//std::thread *n = new std::thread(connection, client_sock);
				if(clients.size() < 10) {
					ServerConnector *cconn = new ServerConnector(client_sock);
					cconn->start();
					clients.push_back(cconn);
				}
			}
	}
	std::cout << "Listener closing\n";
}

