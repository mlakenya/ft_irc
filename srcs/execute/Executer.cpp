#include "../../includes/stdafx.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/CmdList.hpp"

// TODO
void listDelete(cmdList **cmds, cmdList *cmd);

void Execute(Client *client, cmdList *cmds)
{
	// Do not execute commands if the client is deleted.
	if (client->status == DELETED)
		return ;

	cmdList *cmd = cmds;
	// Iterate through the commands.
	while (cmd != NULL)
	{
		// Skip commands if we can't execute them yet.
		if (client->status == PASSWORD)
			if (cmd->command != "PASS")
				continue;
		if (client->status == AUTHENTICATION)
			if (cmd->command != "NICK"
				|| cmd->command != "USER")
				continue;
		
		// Trying to find a command and execute it.
		if (command_funcs.find(cmd->command) != command_funcs.end()
			|| cmd->command == "PASS" && client->status != PASSWORD)
			command_funcs[cmd->command]();
		else
			client->_send_buff += "Error!"; // TODO unknown command.

		listDelete(&cmds, cmd);

		cmd = cmd->next;
	}

	if (cmds != NULL)
		Execute(client, cmds);
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