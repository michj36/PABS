/**
 * @author Sebastian Widemajer
 * last update: 22.04.2017
 *
 */
#ifndef AGENTCONNECTOR_H
#define AGENTCONNECTOR_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#include <unistd.h>
#include <cstring>
#include <stdexcept>
#include <thread>
#include <poll.h>
#include <iostream>

#include "msg_types.h"

class AgentConnector
{
public:
	AgentConnector(char* name);
	virtual ~AgentConnector();
	void run();
	void end();
protected:
	void asocket();
	void aconnect();
	int getData();
	int readCheck();
	int readAll();
private:
	enum conn_state{
		CONNECT,
		WAITING_HELLO,
		LISTENING,
		SENDING_DATA,
		END,
		QUIT
	} state;
	pabs::data_msg m;
	int sock;
	struct sockaddr_in name;
	struct hostent *hp;
	char* hostname;

	bool quit = false;
	bool user_end = true;
	unsigned time = 0;
};

#endif // AGENTCONNECTOR_H
