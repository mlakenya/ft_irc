#ifndef FUNCTIONS_HPP
# define FUNCTIONS_HPP

# include "Server.hpp"

class Server;

void ServerManager(Server *server);
void ParseMessage(std::string message, cmdList **commands);
void Execute(Server *server, Client *client, cmdList * cmds);
void InitCommandFunctions();

#endif