#ifndef COMMANDS_HPP
# define COMMANDS_HPP

#include "Server.hpp"
#include "CmdList.hpp"

/*
*  List of signatures for server commands.
*/

void PASS(Server *server, Client *client, cmdList *cmd);
void NICK(Server *server, Client *client, cmdList *cmd);
void USER(Server *server, Client *client, cmdList *cmd);
void JOIN(Server *server, Client *client, cmdList *cmd);

extern std::map<std::string, void(*)(Server *, Client *, cmdList *)>	command_funcs;

#endif
