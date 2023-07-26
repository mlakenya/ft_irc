#include "../../includes/stdafx.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Commands.hpp"

void Execute(Server *server, Client *client)
{
	cmdList *cmd;
	if (DEBUG)
		std::cout << "Execute " << client->GetStatus() << std::endl;
	
	// Do not execute commands if the client is deleted.
	if (client->GetStatus() == DELETED)
		return ;
	
	cmd = client->GetCmdBuff();
	// Iterate through the commands.
	while (cmd != NULL)
	{
		if (DEBUG)
			std::cout << "Try to execute command " << cmd->command << " in loop" << std::endl;

		// Skip commands if we can't execute them yet.
		if (client->GetStatus() == PASSWORD)
			if (cmd->command != "PASS")
			{
				cmd = cmd->next;
				continue;
			}
		if (client->GetStatus() == AUTHENTICATION)
			if (cmd->command != "NICK"
				&& cmd->command != "USER")
				{
					cmd = cmd->next;
					continue;
				}

		if (DEBUG)
			std::cout << "Execute command: " << cmd->command << std::endl;

		// TODO delete after commands realisation.
		// Trying to find a command and execute it.
		if (command_funcs.find(cmd->command) != command_funcs.end()
			&& (cmd->command != "PASS" || client->GetStatus() == PASSWORD))
			command_funcs[cmd->command](server, client, cmd);
		else if (cmd->command != "CAP")
			client->_send_buff.append(ERR_UNKNOWN_COMMAND(client->GetNickname(), cmd->command));
		cmdList *next = cmd->next;
		client->DeleteCommand(cmd);

		cmd = next;
	}
}
