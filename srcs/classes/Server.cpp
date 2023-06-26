#include "../../includes/Server.hpp"

Server::Server(char* port, char* password)
: _server_socket(0), _port(port), _passwd(password)
{
	std::cout << "Launching server..." << std::endl;
}

Server::~Server()
{
	// TODO free map
	std::cout << "Bye bye... Have a good day!" << std::endl;
}

/*
*	۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝
*	۝       PUBLIC METHODS       ۝
*	۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝
*/

void Server::Prepare()
{
	struct addrinfo hints;
	struct addrinfo	*_servinfo;


	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;		  // Ipv4
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	hints.ai_flags = AI_PASSIVE;	  // fill in my IP for me
	

	if (getaddrinfo(NULL, this->_port, &hints, &_servinfo) != 0)
		throw std::runtime_error("ERROR. getaddrinfo failed");

	this->_server_socket = socket(AF_INET, SOCK_STREAM, _servinfo->ai_protocol);
	if (this->_server_socket == FAILURE)
		throw std::runtime_error("ERROR. Socker creation failed");

	int optvalue = 1;
	if (setsockopt(this->_server_socket, SOL_SOCKET, SO_REUSEADDR, &optvalue, sizeof(optvalue)) == FAILURE)
		throw std::runtime_error("ERROR. Socker setting failed");

	if (bind(this->_server_socket, _servinfo->ai_addr, _servinfo->ai_addrlen) == FAILURE)
		throw std::runtime_error("ERROR. Bind failed");

	if (listen(this->_server_socket, 11) == FAILURE)
		throw std::runtime_error("ERROR. wtf lisent. Why are you not working?");

	std::cout << "Server is listening on port " << this->_port << std::endl;

	freeaddrinfo(_servinfo);
}

void Server::CreateConnection(std::vector<pollfd> *new_pfds)
{
	int client_socket;

	client_socket = accept(this->_server_socket, NULL, NULL);
	if (client_socket == -1)
		throw std::runtime_error("ERROR. Accept failed");
	
	if (this->_pfds.size() - 1 < MAX_CLIENT)
		AddClient(client_socket, new_pfds);
	else
		ServerIsFull(client_socket);
}

void Server::HandleClientRequest(int client_fd)
{
	char	message[BUFFER_SIZE];
	size_t 	read_count;
	Client	*client;

	memset(message, 0, sizeof(message));
	read_count = recv(client_fd, message, BUFFER_SIZE, 0);

	if (read_count > 0)
	{
		std::cout << "Message from client №" << client_fd << ": " << message;
		// TODO could be an error
		client = this->_clients.find(client_fd)->second;
		client->_recv_buff += message;

		if (client->_recv_buff.find("\r\n") != std::string::npos)
		{
			cmdList *c = NULL;
			ParseMessage(client->_recv_buff, &c);
			client->_recv_buff.clear();
			/*    ------- Check parsing   -------- 

			std::cout << "Full message: " << client->_recv_buff << std::endl;
			std::cout << "Parcing:" << std::endl << std::endl;
			while (c != NULL)
			{
				std::cout << "Prefix: " << c->prefix << std::endl;
				std::cout << "Cmd: " << c->command << std::endl;

				std::cout << "Params: ";
				for (int i = 0; i < (int)c->parameters.size(); i++)
					std::cout << c->parameters[i] << " ";
				std::cout << std::endl;

				std::cout << "Trailing: " << c->trailing << std::endl << std::endl;
				c = c->next;
			}
			*/


			

			// TODO
			//Execute(c);
			//Free commands
		}
	}
	else
	{
		this->DelClient(client_fd);
		std::stringstream ss;
		if ((int)read_count == FAILURE)
			ss << "ERROR. Cann't read from client №" << client_fd << ". Client was disconnected.";
		else
			ss << "Client №" << client_fd << " disconnected from the server.";
		throw std::runtime_error(ss.str());
	}
}

void Server::MakeResponse(int client_fd)
{
	// Searching for a client by his socket.
	std::map<int, Client *>::iterator it = this->_clients.find(client_fd);
	if (it == this->_clients.end())
		throw std::runtime_error("ERROR. Client is not found.");

	// Sending data from send buffer to a client.
	Client *client = it->second;
	int sent_bytes = send(client_fd, client->_send_buff.c_str(), client->_send_buff.size(), 0);
	if (sent_bytes == FAILURE)
		throw std::runtime_error("ERROR! Send failed");
	
	// Printing sent data.
	std::istringstream	buf(client->_send_buff);
	std::string			line;
	while (getline(buf, line))
		std::cout << "Message sent to client #" << client_fd \
					<< ": " << line << std::endl;

	// 
	client->_send_buff.clear();
}

/*
*	۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝
*	۝       PRIVATE METHODS      ۝
*	۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝
*/

void Server::AddClient(int client_socket, std::vector<pollfd> *new_pfds)
{
	pollfd	poll_client_socket;

	poll_client_socket.fd = client_socket;
	poll_client_socket.events = POLLIN | POLLOUT;
	
	new_pfds->push_back(poll_client_socket);

	// TODO delete
	Client	*new_client = new Client(client_socket);
	this->_clients.insert(std::pair<int, Client *>(client_socket, new_client));

	std::cout << std::endl << "New client connected, his number is №" << client_socket << std::endl;
}

void Server::ServerIsFull(int client_socket)
{
	const char *err_msg = "[Server] You cannot join, the server is already full";

	send(client_socket, err_msg, strlen(err_msg) + 1, 0);
	close(client_socket);
	throw std::runtime_error(err_msg);
}

void Server::DelClient(int client_socket)
{
	close(client_socket);
	this->_clients.erase(client_socket);

	// TODO that can be an Error
	std::vector<pollfd>::iterator it = this->_pfds.begin();
	while (it != this->_pfds.end() && it->fd != client_socket)
		it++;
	if (it != this->_pfds.end())
		this->_pfds.erase(it);
}

/*
*	۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝
*	۝         ACCESSORS          ۝
*	۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝
*/

int Server::GetServerSocket()
{
	return _server_socket;
}