#ifndef SERVER_HPP
# define SERVER_HPP

# include "stdafx.hpp"
# include "Client.hpp"

class Server
{
private:
	int						_server_socket;
	std::map<int, Client>	_clients;
	const char *			_port;
	const std::string		_passwd;

	void AddClient(int client_socket, std::vector<pollfd> *new_pfds);
	void ServerIsFull(int client_socket);
	void DelClient(int client_socket);

public:
	Server(char *port, char* password);
	~Server();

	std::vector<pollfd>		_pfds;

	void Prepare();
	void CreateConnection(std::vector<pollfd> *new_pfds);
	void HandleClientRequest(int client_fd);

	int GetServerSocket();
};

#endif