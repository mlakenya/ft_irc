#include "../../../includes/CmdList.hpp"
#include "../../../includes/Server.hpp"

bool HasWrongChars(std::string nickname);
bool isUsed(Server *server, std::string nickname);

void NICK(Server *server, Client *client, cmdList *cmd)
{
	std::cout << "NICK" << std::endl;
	std::string nickname;
	std::string oldnick;

	if (cmd->parameters.size() < 1 || cmd->parameters[0].empty())
	{
		client->_send_buff.append(ERR_NO_NICKNAME_GIVEN(client->GetNickname()));
		std::string nick = "";
		client->_send_buff.append(RPL_NICK(client->GetNickname(), client->GetUsername(), nick));
		return ;
	}

	nickname = cmd->parameters[0];
	if (HasWrongChars(nickname))
		client->_send_buff.append(ERR_ERRONEUS_NICKNAME(client->GetNickname(), nickname));

	else if (isUsed(server, nickname) == true)
		client->_send_buff.append(ERR_NICKNAME_IN_USE(client->GetNickname(), nickname));

	else
	{
		if (client->GetNickname().empty())
		{
			client->SetNickname(nickname);
			oldnick = nickname;
			client->SetStatus(ONLINE);
			if (client->GetStatus() == ONLINE)
				server->SendWelcomeMsg(client);
		}
		else
		{
			oldnick = client->GetNickname();
			client->SetNickname(nickname);
		}
	}

	client->_send_buff.append(RPL_NICK(oldnick, client->GetUsername(), nickname));
}

bool	HasWrongChars(std::string nickname)
{
	if (nickname.find(' ') != std::string::npos
		|| nickname.find(',') != std::string::npos
		|| nickname.find('*') != std::string::npos
		|| nickname.find('?') != std::string::npos
		|| nickname.find('!') != std::string::npos
		|| nickname.find('@') != std::string::npos
		|| nickname.find('.') != std::string::npos)
		return true;
	if (nickname[0] == '$' || nickname[0] == ':' || nickname[0] == '#')
		return true;
	return false;
}

bool isUsed(Server *server, std::string nickname)
{
	std::map<int, Client *>::iterator client;

	client = server->GetClients()->begin();
	for (; client != server->GetClients()->end(); client++)
	{
		if (client->second->GetNickname() == nickname)
			return true;
	}
	return false;
}