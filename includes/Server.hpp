#ifndef SERVER_HPP
# define SERVER_HPP

# include	"stdafx.hpp"
# include	"Client.hpp"
# include	"CmdList.hpp"
# include	"functions.hpp"
# include	"ServerResponces.hpp"
# include	"Channel.hpp"

class Channel;

class Server
{
private:
	int								_server_socket;
	std::map<int, Client *>			_clients;
	std::map<std::string, Channel*>	_channels;
	const char *					_port;
	const std::string				_passwd;
	std::string *					_welcomeMessages;
	

	void AddClient(int client_socket, std::vector<pollfd> *new_pfds);
	void ServerIsFull(int client_socket);
	void DelClient(int client_socket);
	void InitializeWelcomeMsg();

public:
	std::vector<pollfd>				_pfds;


	Server(char *port, char* password);
	~Server();

	void Prepare();
	void CreateConnection(std::vector<pollfd> *new_pfds);
	void HandleClientRequest(int client_fd);
	void MakeResponse(int client_fd);
	void PollError(int client_fd);
	void SendWelcomeMsg(Client *client);
	Channel	*CreateChannel(std::string channel_name);

	int 							GetServerSocket();
	std::string 					GetPassword();
	std::map<int, Client *> 		*GetClients();
	std::map<std::string, Channel*> *GetChannels();
};

#endif