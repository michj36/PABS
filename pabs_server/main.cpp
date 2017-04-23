/**
 * @author Sebastian Widemajer
 * last update: 22.04.2017
 *
 */
#include "Listener.h"
#include "CommandHandler.h"

using namespace std;

int main(void)
{
	Listener l;
	CommandHandler handler(l.getClients());
	l.start();
    handler.run();
    l.setQuit();
	return 0;
}
