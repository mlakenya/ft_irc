#include "../../includes/Server.hpp"


Server::Server(char* port, char* password) : _port(port), _passwd(password)
{
	_server_socket = 0;
}

Server::~Server()
{}

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

	freeaddrinfo(_servinfo);
}

int Server::GetServerSocket()
{
	return _server_socket;
}


void Server::AddClient(int client_socket, std::vector<pollfd> *new_pfds)
{
	pollfd	poll_client_socket;

	poll_client_socket.fd = client_socket;
	poll_client_socket.events = POLLIN | POLLOUT;
	
	new_pfds->push_back(poll_client_socket);

	Client	new_client(client_socket);
	this->_clients.insert(std::pair<int, Client>(client_socket, new_client));

	std::cout << "Added cleint #" << client_socket << std::endl;
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


void Server::ServerIsFull(int client_socket)
{
	const char *err_msg = "[Server] You cannot join, the server is already full";

	send(client_socket, err_msg, strlen(err_msg) + 1, 0);
	close(client_socket);
	throw std::runtime_error(err_msg);
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
	char message[BUFFER_SIZE];
	size_t read_count;

	memset(message, 0, sizeof(message));
	read_count = recv(client_fd, message, BUFFER_SIZE, 0);

	if ((int)read_count == FAILURE)
	{
		this->DelClient(client_fd);
		std::stringstream ss;
		ss << "ERROR. Cann't read from client №" << client_fd << ". Client was disconnected.";
		throw std::runtime_error(ss.str());
	}

}
