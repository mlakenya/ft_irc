#include "../../../includes/CmdList.hpp"
#include "../../../includes/Server.hpp"

void MODE(Server *server, Client *client, cmdList *cmd)
{
	std::string nickname = client->GetNickname();
	std::string target = cmd->parameters[0];
	std::string mode_string;

	if(cmd->parameters.size() > 1)
		mode_string = cmd->parameters[1];

	if(target[0] == '#')
	{
		std::string channel_name = target.substr(1, target.size() - 1);
		std::map<std::string, Channel*>::iterator it = server->GetChannels()->find(channel_name);

		if (it == server->GetChannels()->end())
		{
			client->_send_buff.append(ERR_NO_SUCH_CHANNEL(nickname, channel_name));
			return;
		}

		Channel* chennel = it->second;
		if(mode_string.empty() || (mode_string[0] != '+' && mode_string[0] != '-'))
		{
			client->_send_buff.append(RPL_CHANNEL_MODEIS(nickname, target, chennel->GetMode()));
			return;
		}
		
		if(!chennel->IsOperator(nickname))
		{
			client->_send_buff.append(ERR_CHAN_OP_RIVS_NEEDED(nickname, channel_name));
			return;
		}

		
		std::string mode_string = cmd->parameters[1];
		if(mode_string[0] == '+')
		{
			std::string mode_leter;
			for(unsigned long i = 1; mode_string.size() > i; i++)
			{
				if(mode_string[i] == 'i' || mode_string[i] == 't' || mode_string[i] == 'k' || mode_string[i] == 'o' || mode_string[i] == 'b'|| mode_string[i] == 'l')
				{
					if(chennel->GetMode().find(mode_string[i]) == std::string::npos)
						mode_leter = mode_leter + mode_string[i];
				}
					
			}
			chennel->AddMode(mode_leter);
			client->_send_buff.append(RPL_CHANNEL_MODEIS(nickname, target, chennel->GetMode()));
		}
		else if(mode_string[0] == '-')
		{
			for(unsigned long i = 1; mode_string.size() > i; i++)
			{
				if(mode_string[i] == 'i' || mode_string[i] == 't' || mode_string[i] == 'k' || mode_string[i] == 'o' || mode_string[i] == 'b'|| mode_string[i] == 'l')
				{
					if(chennel->GetMode().find(mode_string[i]) != std::string::npos)
						chennel->RemoveMode(std::string(1,mode_string[i]));
				}
			}
			client->_send_buff.append(RPL_CHANNEL_MODEIS(nickname, target, chennel->GetMode()));
		}
	}
	else
	{
		std::map<int, Client *> *clients = server->GetClients();
		std::map<int, Client*>::iterator it_target = clients->begin();
		int flag = 0;

		for(; it_target != clients->end(); it_target++)
		{
			Client *ex = it_target->second;
			if(ex->GetNickname() == target || ex->GetUsername() == target)
			{
				flag = 1;
			}
		}

		if(flag == 0)
		{
			client->_send_buff.append(ERR_NO_SUCH_NICK(nickname, target));
			return;
		}

		if(mode_string.empty())
		{
			std::string mode_string_user;
			if(client->GetModeI())
				mode_string_user = "+i";
			else
				mode_string_user = "-i";
			client->_send_buff.append(RPL_UMODEIS(nickname, mode_string_user));
		}
		else if(mode_string[0] == '+')
		{
			client->SetModeI(true);
			client->_send_buff.append(RPL_MODE_USER(nickname, "+i"));
		}
		else if(mode_string[0] == '-')
		{
			client->SetModeI(false);
			client->_send_buff.append(RPL_MODE_USER(nickname, "-i"));

		}
	}
}