#ifndef COMMANDS_HPP
# define COMMANDS_HPP

#include "Server.hpp"
#include "CmdList.hpp"

/*
*  List of signatures for server commands.
*/

void PART(Server *server, Client *client, cmdList *cmd);
void PASS(Server *server, Client *client, cmdList *cmd);
void NICK(Server *server, Client *client, cmdList *cmd);
void USER(Server *server, Client *client, cmdList *cmd);
void JOIN(Server *server, Client *client, cmdList *cmd);
void MODE(Server *server, Client *client, cmdList *cmd);
void KICK(Server *server, Client *client, cmdList *cmd);
void PING(Server *server, Client *client, cmdList *cmd);
void TOPIC(Server *server, Client *client, cmdList *cmd);
void INVITE(Server *server, Client *client, cmdList *cmd);
void WHO(Server *server, Client *client, cmdList *cmd);
void PRIVMSG(Server *server, Client *client, cmdList *cmd);
void WHOIS(Server *server, Client *client, cmdList *cmd);

extern std::map<std::string, void(*)(Server *, Client *, cmdList *)>	command_funcs;

#endif
