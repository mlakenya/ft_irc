#include "../../../includes/CmdList.hpp"
#include "../../../includes/Server.hpp"

void WHOIS(Server *server, Client *client, cmdList *cmd)
{
	// If no channel provided or no user name, then error.
	if (cmd->parameters.size() < 1 || cmd->parameters[0].size() == 0)
	{
		client->_send_buff.append(ERR_NO_NICKNAME_GIVEN(client->GetNickname()));
		return;
	}

	std::string user_nick = cmd->parameters[0];
	std::map<int, Client*> *server_clients = server->GetClients();
	std::map<int, Client*>::iterator it = server_clients->begin();
	while (it != server_clients->end())
	{
		if (it->second->GetNickname() == user_nick)
		{
			client->_send_buff.append(RPL_WHOIS_USER(user_nick, it->second->GetFullName()));
			client->_send_buff.append(RPL_END_OF_WHOIS(client->GetNickname()));

			return;
		}
	}
	client->_send_buff.append(ERR_NO_SUCH_NICK(user_nick, user_nick));
}