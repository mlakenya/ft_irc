#ifndef SERVER_HPP
# define SERVER_HPP

# include "stdafx.hpp"
# include "Client.hpp"

class Server
{
private:
	int						_server_socket;
	std::map<int, Client>	_clients;
	static int				_port;
	std::string				_passwd;

public:

};

#endif