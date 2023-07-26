#include "../../../includes/CmdList.hpp"
#include "../../../includes/Server.hpp"


void PING(Server *server, Client *client, cmdList *cmd)
{
    std::string nickname = client->GetNickname();
	std::string host_name = "localhost";
    (void)server;
    if(cmd->parameters.empty())
    {
    	client->_send_buff.append(ERR_NO_ORIGIN(nickname));
        return;
    }
    if(cmd->parameters.size() == 1)
    {
        if(cmd->parameters[0] == host_name)
        {
			client->_send_buff.append(RPL_PONG(host_name, nickname));
			return;
        }
    }
    else
    {
        client->_send_buff.append(ERR_NEED_MORE_PARAMS(nickname, cmd->command));
		return;
    }
}