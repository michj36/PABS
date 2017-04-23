/**
 * @author Sebastian Widemajer
 * last update: 22.04.2017
 *
 */
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <csignal>

#include "AgentConnector.h"


#include "msg_types.h"

using namespace std;
using namespace pabs;

static AgentConnector *ac;

void closingTime(int){
	if (ac!=nullptr)
		ac->end();
	exit(0);
}

int main(int argc, char *argv[])
{
	if(argc < 2) {
		cout << "Needs address!\n";
		return 1;
	}
	AgentConnector connector(argv[1]);
	ac = &connector;
	signal(SIGINT, closingTime);
	connector.run();
	return 0;
}
