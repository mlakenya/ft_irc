#include "../../../includes/CmdList.hpp"
#include "../../../includes/Server.hpp"

void TOPIC(Server *server, Client *client, cmdList *cmd)
{
    std::string nickname = client->GetNickname();
    if (cmd->parameters.size() == 0 || cmd->parameters.size() > 2)
	{
		client->_send_buff.append(ERR_NEED_MORE_PARAMS(nickname, cmd->command));
		return;
	}
    std::string channel_name = cmd->parameters[0].substr(1, cmd->parameters[0].size() - 1);
    std::map<std::string, Channel*>::iterator it = server->GetChannels()->find(channel_name);

    Channel* chennel = it->second;

    std::map <std::string, Client*>::iterator it_client = chennel->GetClientList().find(nickname);

    
    if(it_client == chennel->GetClientList().end())
	{
		client->_send_buff.append(ERR_NO_TON_CHANNEL(nickname, channel_name));
		return;
	}
    
    if(cmd->trailing.empty())
    {
        client->_send_buff.append(RPL_TOPIC(nickname, channel_name, chennel->GetTopic()));
        return ;
    }

    if(chennel->GetMode().find('t') != std::string::npos)
    {
        if(!chennel->IsOperator(nickname))
        {
            client->_send_buff.append(ERR_CHAN_OP_RIVS_NEEDED(nickname, channel_name));
            return;
        }
        else
        {
            chennel->SetTopic(cmd->trailing);
            client->_send_buff.append(RPL_TOPIC(nickname, channel_name, chennel->GetTopic()));
        }
    }
    else
    {
        chennel->SetTopic(cmd->trailing);
        client->_send_buff.append(RPL_TOPIC(nickname, channel_name, chennel->GetTopic()));
    }
}