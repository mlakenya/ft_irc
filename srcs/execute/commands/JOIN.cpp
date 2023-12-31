#include "../../../includes/CmdList.hpp"
#include "../../../includes/Server.hpp"

std::string 	GetChannelName(std::string &params);
std::string 	GetPassword(std::string &passwords);
void			SendChanInfos(Channel *channel, Client *client);

void JOIN(Server *server, Client *client, cmdList *cmd)
{
	std::string nickname = client->GetNickname();

	if (cmd->command.empty() || cmd->parameters.size() == 0)
	{
		client->_send_buff.append(ERR_NEED_MORE_PARAMS(nickname, cmd->command));
		return;
	}
	
	while (!cmd->parameters[0].empty())
	{
		Channel		*channel;
		std::string channel_name = GetChannelName(cmd->parameters[0]);

		// Trying to find this channel on server.
		// If there is no one, create new.
		// Otherwise, check if the channel has a password and do they match.
		std::map<std::string, Channel*>::iterator it = server->GetChannels()->find(channel_name);
		if (it == server->GetChannels()->end())
			channel = server->CreateChannel(channel_name);
		else 
			channel = it->second;

		if (channel->GetMode().find('k') != std::string::npos)
		{
			std::string key = GetPassword(cmd->parameters[1]);
			if (key != channel->GetPassword())
			{
				client->_send_buff.append(ERR_BAD_CHANNEL_KEY(nickname, channel_name));
				continue ;
			}
		}

		// If the channel has a limit, and it's full.
		if (channel->GetMode().find('l') != std::string::npos
			 && (int)channel->GetClientList().size() >= channel->GetCapacityLimit())
		{
			client->_send_buff.append(ERR_CHANNEL_IS_FULL(nickname, channel_name));
			continue ;
		}

		// If this channel has invite-only mode.
		if (channel->GetMode().find('i') != std::string::npos)
		{
			client->_send_buff.append(ERR_INVITE_ONLY_CHAN(channel_name));
			continue ;
		}

		channel->AddClient(client);
		if (channel->GetOperators().empty())
			channel->AddFirstOperator(nickname);
		SendChanInfos(channel, client);
	}
}

// Returns one name from channel_names string, and erase it from this string.
// Example: before - #channel1,#channel2,#channel3, after - #channel2,#channel3
std::string GetChannelName(std::string &channel_names)
{
	std::string name;
	size_t comma_pos;

	comma_pos = channel_names.find(',');
	if (comma_pos != std::string::npos)
	{
		name = channel_names.substr(1, comma_pos - 1);
		channel_names.erase(0, comma_pos + 1);
	}
	else
	{
		name = channel_names.substr(1, channel_names.size() - 1);
		channel_names.clear();
	}

	return name;
}

// Returns one password from passwords string, and erase it from this string.
std::string GetPassword(std::string &passwords)
{
	std::string pass;
	size_t comma_pos;

	comma_pos = passwords.find(',');
	if (comma_pos != std::string::npos)
	{
		pass = passwords.substr(0, comma_pos);
		passwords.erase(0, comma_pos + 1);
	}
	else
	{
		pass = passwords;
		passwords.clear();
	}

	return pass;
}

void SendChanInfos(Channel *channel, Client *client)
{
	std::string	nick		= client->GetNickname();
	std::string username	= client->GetUsername();
	std::string channel_symbol = channel->GetSymbol();
 	
	std::map<std::string, Client*>::iterator member = channel->GetClientList().begin();

	while (member != channel->GetClientList().end())
	{
		member->second->_send_buff.append(RPL_JOIN(nick, username, channel->GetName()));
		if (!channel->GetTopic().empty()) 
			member->second->_send_buff.append(RPL_TOPIC(nick, channel->GetName(), channel->GetTopic()));

		member->second->_send_buff.append(RPL_NAMREPLY(username, channel_symbol, channel->GetName(), channel->GetListOfMembers()));
		member->second->_send_buff.append(RPL_ENDOFNAMES(username, channel->GetName()));
		member++;
	}
}
