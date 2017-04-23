/**
 * @author Sebastian Widemajer
 * last update: 22.04.2017
 *
 */
#ifndef LISTENER_H
#define LISTENER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>
#include <thread>
#include <poll.h>
#include <iostream>
#include <vector>

#include "msg_types.h"
#include "ServerConnector.h"

class Listener
{
public:
	Listener();
	virtual ~Listener();
	void start();
	void run();
	std::vector<ServerConnector*> * getClients() {return &clients;};
	void setQuit() {quit = true;}
protected:
	void lsocket();
	void lbind();
	void llisten();
	void laccept();
	int lselect();
private:
	std::vector<ServerConnector*> clients;
	std::thread thread;
	fd_set sset;
	bool quit;
	unsigned sock, length;
	struct sockaddr_in name;
	struct sockaddr client;
	unsigned client_sock;
};

#endif // LISTENER_H
