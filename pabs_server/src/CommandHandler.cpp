/**
 * @author Sebastian Widemajer
 * last update: 22.04.2017
 *
 */
#include "CommandHandler.h"

CommandHandler::CommandHandler(std::vector<ServerConnector*> *c)
{
	clients = c;
}

CommandHandler::~CommandHandler()
{

}

void CommandHandler::run()
{
	std::string command;
	bool quit = false;
	while(!quit) {
		std::cin >> command;
		if(command == "quit"){
				std::cout <<"Received quit command\n";
			quit = true;
		}
		if(command == "start") {
				std::cout <<"Received start command\n";
			for(auto conn = clients->begin() ; conn != clients->end(); conn++) {
				(*conn)->scanBegin();
			}
		}
		if(command == "stop") {
			std::cout <<"Received stop command\n";
			for(auto conn = clients->begin() ; conn != clients->end(); conn++) {
				(*conn)->scanEnd();
			}
		}
	}
}

void CommandHandler::startScan(unsigned c)
{

}

void CommandHandler::stopScan(unsigned c)
{

}
