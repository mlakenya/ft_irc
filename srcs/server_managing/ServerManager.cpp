#include "Server.hpp"

void ServerManager(Server server)
{
	std::vector<pollfd> pfds;
	pollfd	poll_server_socket;

	poll_server_socket.fd = server.GetServerSocket();
	poll_server_socket.events = POLLIN;

	pfds.push_back(poll_server_socket);

}
