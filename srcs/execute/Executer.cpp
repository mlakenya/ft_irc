#include "../../includes/stdafx.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/CmdList.hpp"
#include "../../includes/Commands.hpp"

// TODO
void listDelete(cmdList **cmds, cmdList *cmd);

void Execute(Server *server, Client *client, cmdList *cmds)
{
	std::cout << "Execute " << client->GetStatus() << std::endl;
	// Do not execute commands if the client is deleted.
	if (client->GetStatus() == DELETED)
		return ;

	cmdList *cmd = cmds;
	// Iterate through the commands.
	while (cmd != NULL)
	{
		std::cout << "Execute command " << cmd->command << " in loop" << std::endl;

		// Skip commands if we can't execute them yet.
		if (client->GetStatus() == PASSWORD)
			if (cmd->command != "PASS")
			{
				cmd = cmd->next;
				continue;
			}
		if (client->GetStatus() == AUTHENTICATION)
			if (cmd->command != "NICK"
				|| cmd->command != "USER")
				{
					cmd = cmd->next;
					continue;
				}

		// Trying to find a command and execute it.
		if (command_funcs.find(cmd->command) != command_funcs.end()
			&& (cmd->command != "PASS" || client->GetStatus() == PASSWORD))
			command_funcs[cmd->command](server, client, cmds);
		else
			client->_send_buff += "Error! unknown command."; // TODO unknown command.

		// TODO Check
		cmdList *next = cmd->next;
		listDelete(&cmds, cmd);

		cmd = next;
	}
}

void listDelete(cmdList **cmds, cmdList *cmd)
{
	if (*cmds == NULL || cmd == NULL)
		return ;

	cmdList *tmp = *cmds;
	if (tmp == cmd)
		*cmds = (*cmds)->next;
	else
	{
		while (tmp->next != NULL && tmp->next != cmd)
			tmp = tmp->next;
		
		tmp->next = cmd->next;
	}
	delete cmd;
}
