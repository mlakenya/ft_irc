#include "../../../includes/CmdList.hpp"
#include "../../../includes/Server.hpp"

void handleChannelMode(Server *server, Client *client, cmdList *cmd, std::string modes, std::string channel_name);
void handleUserMode(Server *server, Client *client, std::string modes, std::string client_name);
void addChannelModes(std::string modes, Channel *channel, cmdList *cmd);

void MODE(Server *server, Client *client, cmdList *cmd)
{
	if (cmd->parameters.size() < 1)
	{
		client->_send_buff.append(ERR_NEED_MORE_PARAMS(client->GetNickname(), "MODE"));
		return;
	}
	std::string target = cmd->parameters[0];
	std::string mode_string;

	cmd->parameters.erase(cmd->parameters.begin());
	if(cmd->parameters.size() > 0)
	{
		mode_string = cmd->parameters[0];
		cmd->parameters.erase(cmd->parameters.begin());
	}

	if(target[0] == '#')
	{
		std::string channel_name = target.substr(1, target.size() - 1);
		handleChannelMode(server, client, cmd, mode_string, channel_name);
	}
	else
	{
		handleUserMode(server, client, mode_string, target);
	}
}

bool acceptableMode(char mode) {
    for (int i = 0; i <= 6; i++) {
		if (ACCEPTABLE_MODES[i] == mode)
			return true;
    }
    return false;
}

void handleChannelMode(Server *server, Client *client, cmdList *cmd, std::string modes, std::string channel_name)
{
	std::string nickname = client->GetNickname();

	// Trying to find the channel on server.
	std::map<std::string, Channel*>::iterator it = server->GetChannels()->find(channel_name);
	if (it == server->GetChannels()->end())
	{
		client->_send_buff.append(ERR_NO_SUCH_CHANNEL(nickname, channel_name));
		return;
	}

	// If we want to get information about channel modes.
	Channel* channel = it->second;
	if(modes.empty() || (modes[0] != '+' && modes[0] != '-'))
	{
		if (DEBUG)
			std::cout << "Empty modes, return channel modes" << std::endl;
		client->_send_buff.append(RPL_CHANNEL_MODEIS(nickname, channel_name, channel->GetMode()));
		return ;
	}
	
	// If client wants to set/remove modes, he has to be operator.
	if(!channel->IsOperator(nickname))
	{
		client->_send_buff.append(ERR_CHAN_OP_RIVS_NEEDED(nickname, channel_name));
		return;
	}

	// If we want to add channel modes.
	if(modes[0] == '+')
	{
		addChannelModes(modes, channel, cmd);
		
		client->_send_buff.append(RPL_CHANNEL_MODEIS(nickname, channel_name, channel->GetMode()));
	}
	// If we want to remove modes.
	else if(modes[0] == '-')
	{
		for(unsigned long i = 1; modes.size() > i; i++)
		{
			if(acceptableMode(modes[i]))
			{
				if(channel->GetMode().find(modes[i]) != std::string::npos)
					channel->RemoveMode(std::string(1, modes[i]));
			}
		}
		client->_send_buff.append(RPL_CHANNEL_MODEIS(nickname, channel_name,  channel->GetMode()));
	}
}

void addChannelModes(std::string modes, Channel *channel, cmdList *cmd)
{
	std::string mode_leter;

	for(unsigned long i = 1; modes.size() > i; i++)
	{
		if (DEBUG)
			std::cout << "Trying to set new mode: " << modes[i] << std::endl;
		char mode = modes[i];

		// If this flag is already in channel.
		if(channel->GetMode().find(modes[i]) != std::string::npos)
			continue;
		
		// If this mode is unacceptable on this server.
		if (!acceptableMode(mode))
			continue;

		if (mode != 'o')
			mode_leter = mode_leter + modes[i];

		if (cmd->parameters.size() < 1)
				continue ;

		if (mode == 'l')
		{
			std::string new_capacity = cmd->parameters[0];
			cmd->parameters.erase(cmd->parameters.begin());
			channel->SetCapacityLimit(std::atoi(new_capacity.c_str()));
		}
		if (mode == 'k')
		{
			std::string new_password = cmd->parameters[0];
			cmd->parameters.erase(cmd->parameters.begin());
			channel->SetPassword(new_password);
		}
		if (mode == 'o')
		{
			std::string new_operator = cmd->parameters[0];
			cmd->parameters.erase(cmd->parameters.begin());
			channel->AddOperator(new_operator);
		}
	}

	if (!mode_leter.empty())
		channel->AddMode(mode_leter);
}

void handleUserMode(Server *server, Client *client, std::string modes, std::string client_name)
{
	std::string nickname = client->GetNickname();

	// Trying to get a client by nickname from servers' list of clients.
	Client *user;
	std::map<int, Client *> *clients = server->GetClients();
	std::map<int, Client*>::iterator it_target = clients->begin();
	for(; it_target != clients->end(); it_target++)
	{
		Client *ex = it_target->second;
		if(ex->GetNickname() == client_name || ex->GetUsername() == client_name)
		{
			user = ex;
			break ;
		}
	}

	// If no such client on server - error.
	if(it_target == clients->end())
	{
		client->_send_buff.append(ERR_NO_SUCH_NICK(nickname, client_name));
		return;
	}

	// If we want to get information about clients' modes.
	if(modes.empty())
	{
		std::string res;
		if(client->GetModeI())
			res = "+i";
		else
			res = "-i";
		client->_send_buff.append(RPL_UMODEIS(nickname, res));
	}
	// If we want to add some modes.
	else if(modes[0] == '+')
	{
		client->SetModeI(true);
		client->_send_buff.append(RPL_MODE_USER(nickname, "+i"));
	}
	// If we want to delete modes.
	else if(modes[0] == '-')
	{
		client->SetModeI(false);
		client->_send_buff.append(RPL_MODE_USER(nickname, "-i"));
	}
}