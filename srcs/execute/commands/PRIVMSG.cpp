#include "../../../includes/CmdList.hpp"
#include "../../../includes/Server.hpp"

void PRIVMSG(Server *server, Client *client, cmdList *cmd)
{
    
	std::string nickname = client->GetNickname();
    if (cmd->parameters.size() != 1)
	{
		client->_send_buff.append(ERR_NO_RECIPI_ENT(nickname, cmd->command));
		return;
	}
    if (cmd->trailing.empty())
	{
		client->_send_buff.append(ERR_NO_TEXT_TO_SEND(nickname));
		return;
	}

    std::string user_name = client->GetUsername();
    std::string message = cmd->trailing;
    std::string target = cmd->parameters[0];
    if (target[0] == '#')
    {
        std::string channel_name = target.substr(1, target.size() - 1);
        std::map<std::string, Channel*>::iterator it = server->GetChannels()->find(channel_name);
        if (it == server->GetChannels()->end())
        {
            client->_send_buff.append(ERR_NO_SUCH_NICK(nickname, channel_name));
            return;
        }
        Channel* chennel = it->second;

        std::map<std::string, Client*> clients_on_chennel = chennel->GetClientList();
        std::map <std::string, Client*>::iterator it_client = clients_on_chennel.begin();
        
        for(; it_client != clients_on_chennel.end(); it_client++)
		{
            if(it_client->second->GetNickname() != nickname)
			    it_client->second->_send_buff.append(RPL_PRIVMSG(nickname, user_name, target, message));
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
                break;
			}
		}

        if(flag == 0)
		{
			client->_send_buff.append(ERR_NO_SUCH_NICK(nickname, target));
			return;
		}
    
        it_target->second->_send_buff.append(RPL_PRIVMSG(nickname, user_name, target, message));
        
    }
    

    
}
