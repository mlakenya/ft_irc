#include "../../../includes/CmdList.hpp"
#include "../../../includes/Server.hpp"

void PASS(Server *server, Client *client, cmdList *cmd)
{
	if (cmd->command.empty() || cmd->parameters.size() == 0)
	{
		client->_send_buff.append(ERR_NEED_MORE_PARAMS(client->GetNickname(), cmd->command));
		return ;
	}

	std::string password = cmd->parameters[0];
	if (password != server->GetPassword())
	{
		client->_send_buff.append(ERR_PASSWD_MISMATCH(client->GetNickname()));
		return ;
	}

	client->SetStatus(AUTHENTICATION);
}
