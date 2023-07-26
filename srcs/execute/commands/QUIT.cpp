#include "../../../includes/CmdList.hpp"
#include "../../../includes/Server.hpp"

// TODO Sega
void QUIT(Server *server, Client *client, cmdList *cmd)
{
	(void)cmd;
	server->PollError(client->GetSocket());
}