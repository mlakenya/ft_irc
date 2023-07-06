#include "../../../includes/CmdList.hpp"
#include "../../../includes/Server.hpp"

void KICK(Server *server, Client *client, cmdList *cmd)
{
    std::string channel_name = cmd->parameters[0];
    std::string nickname = client->GetNickname();

    if (cmd->command.empty() || cmd->parameters.size() == 0)
    {
		client->_send_buff.append(ERR_NEED_MORE_PARAMS(nickname, cmd->command));
        return;
    }
    std::map<std::string, Channel*>::iterator it = server->GetChannels()->find(cmd->parameters[0]);

    if (it == server->GetChannels()->end())
    {
        client->_send_buff.append(ERR_NO_SUCH_CHANNEL(nickname, channel_name));
        return;
    }

    std::map<std::string, Channel*>::iterator client_operator = server->GetChannels();
    if()
    {

    }
}
