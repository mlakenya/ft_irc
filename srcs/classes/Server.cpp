#include "../../includes/Server.hpp"


Server::Server(char* port, char* password)
{
	_port = port;
	_passwd = password;
	_server_socket = 0;
	
	
}

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

	if (setsockopt(this->_server_socket, SOL_SOCKET, SO_REUSEADDR, NULL, sizeof(int)) == FAILURE)
		throw std::runtime_error("ERROR. Socker setting failed");

	if (bind(this->_server_socket, _servinfo->ai_addr, _servinfo->ai_addrlen) == FAILURE)
		throw std::runtime_error("ERROR. Bind failed");

	if (listen(this->_server_socket, 2) == FAILURE)
		throw std::runtime_error("ERROR. wtf lisent. Why are you not working?");

	freeaddrinfo(_servinfo);
}

int Server::GetServerSocket()
{
	return _server_socket;
}