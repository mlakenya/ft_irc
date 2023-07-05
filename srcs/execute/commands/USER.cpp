#include "../../../includes/CmdList.hpp"
#include "../../../includes/Server.hpp"

void USER(Server *server, Client *client, cmdList *cmd)
{
	std::string username;
	std::string realname;
	(void)server;

	if (cmd->parameters.size() < 3)
	{
		client->_send_buff.append(ERR_NEED_MORE_PARAMS(client->GetNickname(), cmd->command));
		return ;
	}

	username = cmd->parameters[0];
	realname = cmd->trailing;
	if (username.empty() || realname.empty())
		client->_send_buff.append(ERR_NEED_MORE_PARAMS(client->GetNickname(), cmd->command));
	else if (!client->GetUsername().empty())
		client->_send_buff.append(ERR_ALREADY_REGISTERED(client->GetNickname()));
	else
	{
		client->SetUsername(username);
		client->SetFullName(realname);
		client->SetStatus(ONLINE);
		if (client->GetStatus() == ONLINE)
			server->SendWelcomeMsg(client);
	}
}