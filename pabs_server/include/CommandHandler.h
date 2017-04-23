/**
 * @author Sebastian Widemajer
 * last update: 22.04.2017
 *
 */
#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <unistd.h>
#include <iostream>
#include <vector>

#include "msg_types.h"
#include "ServerConnector.h"


class CommandHandler
{
	public:
		CommandHandler(std::vector<ServerConnector*> *c) ;
		virtual ~CommandHandler();
		void run();
	protected:
		void startScan(unsigned c);
        void stopScan(unsigned c);
	private:
        std::vector<ServerConnector*> *clients;
};

#endif // COMMANDHANDLER_H
