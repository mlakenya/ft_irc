#include "../../../includes/CmdList.hpp"
#include "../../../includes/Server.hpp"

void INVITE(Server *server, Client *client, cmdList *cmd)
{
    std::string nickname = client->GetNickname();
    if (cmd->parameters.size() != 2)
	{
		client->_send_buff.append(ERR_NEED_MORE_PARAMS(nickname, cmd->command));
		return;
	}

    std::string channel_name = cmd->parameters[1].substr(1, cmd->parameters[0].size() - 1);
    std::map<std::string, Channel*>::iterator it = server->GetChannels()->find(channel_name);

	if (it == server->GetChannels()->end())
	{
		client->_send_buff.append(ERR_NO_SUCH_CHANNEL(nickname, channel_name));
		return;
	}
    	
    Channel* chennel = it->second;

    std::map <std::string, Client*>::iterator it_client = chennel->GetClientList().find(nickname);
    
	
	if(it_client == chennel->GetClientList().end())
	{
		client->_send_buff.append(ERR_NO_TON_CHANNEL(nickname, channel_name));
		return;
	}

    std::string user_name = cmd->parameters[0];
    std::map<int, Client *> *client_list = server->GetClients();
    std::map<int, Client*>::iterator it_target = client_list->begin();
    int flag = 0;
    for(; it_target != client_list->end(); it_target++)
    {
        Client *ex = it_target->second;
        if(ex->GetNickname() == user_name || ex->GetUsername() == user_name)
        {
            flag = 1;
            break;
        }
    }

    if(flag == 0)
    {
        client->_send_buff.append(ERR_NO_SUCH_NICK(nickname, user_name));
        return;
    }

    if(chennel->GetMode().find('i') != std::string::npos)
    {
        if(!chennel->IsOperator(nickname))
        {
            client->_send_buff.append(ERR_CHAN_OP_RIVS_NEEDED(nickname, channel_name));
            return;
        }
        else
        {
            client->_send_buff.append(RPL_INVITING(nickname, user_name, channel_name));
            it_target->second->_send_buff.append(RPL_INVITING(nickname, user_name, channel_name));
        }
    }
    else
    {
        client->_send_buff.append(RPL_INVITING(nickname, user_name, channel_name));
        it_target->second->_send_buff.append(RPL_INVITING(nickname, user_name, channel_name));
    }
}
