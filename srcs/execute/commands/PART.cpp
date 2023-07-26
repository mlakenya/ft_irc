#include "../../../includes/CmdList.hpp"
#include "../../../includes/Server.hpp"

void PART(Server *server, Client *client, cmdList *cmd)
{
    std::string nickname = client->GetNickname();
    std::string reason = "Good by";

	if (cmd->parameters.size() != 1)
	{
		client->_send_buff.append(ERR_NEED_MORE_PARAMS(nickname, cmd->command));
		return;
	}
    std::string channel_name = cmd->parameters[0].substr(1, cmd->parameters[0].size() - 1);
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

    if(!cmd->trailing.empty())
		reason = cmd->trailing;

	std::map<std::string, Client*> clients_on_chennel = chennel->GetClientList();
    std::map <std::string, Client*>::iterator it_client_it = clients_on_chennel.begin();

	for(; it_client_it != clients_on_chennel.end(); it_client_it++)
	{
		it_client_it->second->_send_buff.append(RPL_PART(nickname, channel_name, reason));
	}

    chennel->RemoveClient(nickname);
}