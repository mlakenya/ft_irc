#ifndef SERVER_HPP
# define SERVER_HPP

# include "stdafx.hpp"
# include "Client.hpp"

class Server
{
private:
	int						_server_socket;
	std::map<int, Client>	_clients;
	static char *			_port;
	static std::string		_passwd;

public:
	Server(char *port, char* password);
	~Server();

	void Server::Prepare();

	int Server::GetServerSocket();
};

#endif