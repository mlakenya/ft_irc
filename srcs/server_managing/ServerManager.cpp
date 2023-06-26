#include "Server.hpp"

void ServerManager(Server *server)
{
	std::vector<pollfd> new_pfds;
	pollfd	poll_server_socket;

	poll_server_socket.fd = server->GetServerSocket();
	poll_server_socket.events = POLLIN;

	server->_pfds.push_back(poll_server_socket);

	while (true)
	{
		// TODO add comments.
		int num_events;
		std::vector<pollfd>::iterator socket_it;

		// Waiting for the events on sockets.
		num_events = poll((pollfd *)&server->_pfds[0], (unsigned int)server->_pfds.size(), -1);
		if (num_events == 0)
			throw std::runtime_error("ERROR. Poll failed");

		socket_it = server->_pfds.begin();
		while (socket_it != server->_pfds.end())
		{
			// Checking sockets for occurred events.
			// POLLIN - we can now read from this socket.
			// POLLOUT - we can write to this socket.
			if (socket_it->revents & POLLIN)
			{
				try
				{
					// Handle server socket events
					if (socket_it->fd == server->GetServerSocket())
						server->CreateConnection(&new_pfds);
					// Handle Client events
					else
						server->HandleClientRequest(socket_it->fd);
				}
				catch(std::runtime_error ex)
				{
					std::cout << ex.what() << std::endl;
					break;
				}
			}
			else if (socket_it->revents & POLLOUT)
			 	server->MakeResponse(socket_it->fd);
			else if (socket_it->revents & POLLERR)
				;

			socket_it++;
		}
		server->_pfds.insert(server->_pfds.end(), new_pfds.begin(), new_pfds.end());
		new_pfds.clear();
	}
}
