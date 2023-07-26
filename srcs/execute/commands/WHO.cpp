#include "../../../includes/CmdList.hpp"
#include "../../../includes/Server.hpp"

void WHO(Server *server, Client *client, cmdList *cmd)
{
	// If no channel provided or no channel name, then error.
	if (cmd->parameters.size() < 1 || cmd->parameters[0].size()  < 2 || cmd->parameters[0][0] != '#')
	{
		client->_send_buff.append(ERR_NEED_MORE_PARAMS(client->GetNickname(), cmd->command));
		return;
	}

    std::string chan_name = cmd->parameters[0].substr(1, cmd->parameters[0].size() - 1);
	std::map<std::string, Channel*>::iterator it_chan = server->GetChannels()->find(chan_name);
	if (it_chan != server->GetChannels()->end())
	{
		Channel *channel = it_chan->second;
		std::map<std::string, Client *>::iterator it = channel->GetClientList().begin();
		while (it != channel->GetClientList().end())
		{
			if (!it->second->GetModeI())
				client->_send_buff.append(RPL_WHO_REPLY(client->GetNickname(), channel->GetName(), it->second->GetUsername(), it->second->GetFullName()));
			it++;
		}
		client->_send_buff.append(RPL_END_OF_WHO(client->GetNickname()));
	}
}
