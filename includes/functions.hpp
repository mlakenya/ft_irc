#ifndef FUNCTIONS_HPP
# define FUNCTIONS_HPP

# include "Server.hpp"

class Server;

void ServerManager(Server *server);
void ParseMessage(Client *client, std::string message);
int	 Execute(Server *server, Client *client);
void InitCommandFunctions();
void set_shutdown_signal(int signal);

#endif