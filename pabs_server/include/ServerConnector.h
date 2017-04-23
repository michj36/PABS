/**
 * @author Sebastian Widemajer
 * last update: 22.04.2017
 *
 */
#ifndef SERVERCONNECTOR_H
#define SERVERCONNECTOR_H

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
#include "msg_types.h"

class ServerConnector
{
	public:
		ServerConnector(unsigned s);
		virtual ~ServerConnector();
		void start();
		void stop();
        void scanBegin();
        void scanEnd();
	protected:
	 int readAll(unsigned s, pabs::generic_msg *m);
	 void run();
	private:
		enum conn_state {
		WAITING_HELLO,
		CHECK_AGENT,
		CHECK_AGENT_RESPONSE,
		LISTENING,
		SCAN_BEGIN,
		SCAN_BEGIN_RESPONSE,
		SCAN_RECEIVING_DATA,
		SCAN_STOP,
		SCAN_STOP_RESPONSE,
		END,
		QUIT
	};
	int _port;
	bool quit, ended;
	bool user_end;
	unsigned sock;
	std::thread thread;
    bool scan_command = false;
	conn_state state;
	pthread_mutex_t state_lock;
};

#endif // SERVERCONNECTOR_H
