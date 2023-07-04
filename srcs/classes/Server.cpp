#include "../../includes/Server.hpp"

Server::Server(char *port, char *password)
	: _server_socket(0), _port(port), _passwd(password)
{
	InitializeWelcomeMsg();
	std::cout << "Launching server..." << std::endl;
}

Server::~Server()
{
	delete[] this->_welcomeMessages;
	
	std::map<int, Client *>::iterator it = this->_clients.begin();
	while (it != this->_clients.end())
	{
		this->DelClient(it->first);
		it++;
	}
	this->_clients.clear();

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
	struct addrinfo *_servinfo;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;		 // Ipv4
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	hints.ai_flags = AI_PASSIVE;	 // fill in my IP for me

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
	char message[BUFFER_SIZE];
	size_t read_count;
	Client *client;

	memset(message, 0, sizeof(message));
	read_count = recv(client_fd, message, BUFFER_SIZE, 0);

	if (read_count > 0)
	{
		std::cout << "Message from client №" << client_fd << ": " << message;
		client = this->_clients.find(client_fd)->second;
		client->_recv_buff += message;

		if (client->_recv_buff.find("\r\n") != std::string::npos)
		{
			ParseMessage(client, client->_recv_buff);
			client->_recv_buff.clear();

			// /*   ------- Check parsing   --------
			if (DEBUG)
			{
				std::cout << "Full message: " << client->_recv_buff << std::endl;
				std::cout << "Parcing:" << std::endl
						  << std::endl;
				cmdList *tmp = client->GetCmdBuff();
				while (tmp != NULL)
				{
					std::cout << "Prefix: " << tmp->prefix << std::endl;
					std::cout << "Cmd: " << tmp->command << std::endl;

					std::cout << "Params: ";
					for (int i = 0; i < (int)tmp->parameters.size(); i++)
						std::cout << tmp->parameters[i] << " ";
					std::cout << std::endl;

					std::cout << "Trailing: " << tmp->trailing << std::endl
							  << std::endl;
					tmp = tmp->next;
				}
			}

			Execute(this, client);
		}
	}
	else
	{
		this->DelClient(client_fd);
		this->_clients.erase(client_fd);
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
	Client *client;
	size_t buff_end;
	std::string toSend;

	// Searching for a client by his socket.
	std::map<int, Client *>::iterator it = this->_clients.find(client_fd);
	if (it == this->_clients.end())
		throw std::runtime_error("ERROR. Client is not found.");

	// Sending data from send buffer(before first \r\n) to a client.
	client = it->second;
	buff_end = client->_send_buff.find("\r\n");
	if (buff_end == std::string::npos)
		return ;
	toSend = client->_send_buff.substr(0, buff_end + 2);
	client->_send_buff.erase(0, buff_end + 2);
	int sent_bytes = send(client_fd, toSend.c_str(), toSend.size(), 0);
	if (sent_bytes == FAILURE)
		throw std::runtime_error("ERROR! Send failed");

	// Printing sent data.
	std::cout << "Message sent to client №" << client_fd
				  << ": " << toSend << std::endl;
}

void Server::PollError(int client_fd)
{
	if (client_fd == this->_server_socket)
		throw new std::runtime_error("ERROR. Server socket fucked up.");
	else
	{
		this->DelClient(client_fd);
		this->_clients.erase(client_fd);
	}
}

/*
 *	۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝
 *	۝       PRIVATE METHODS      ۝
 *	۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝
 */

void Server::InitializeWelcomeMsg()
{
	std::string line;

	this->_welcomeMessages = new std::string[NUM_WELCOME_MSGS];
	std::string welcome_files[NUM_WELCOME_MSGS] = { "grogu", "shit" };

	for (int i = 0; i < NUM_WELCOME_MSGS; i++)
	{
		std::ifstream infile(("srcs/welcomeMessages/" + welcome_files[i]).c_str());
		if (infile.fail())
		{
			infile.close();
			throw new std::runtime_error("Can't open file " + welcome_files[i]);
		}
		while (std::getline(infile, line))
		{
			this->_welcomeMessages[i].append(line + "\n");
		}
		infile.close();
	}
}

void Server::AddClient(int client_socket, std::vector<pollfd> *new_pfds)
{
	pollfd poll_client_socket;

	poll_client_socket.fd = client_socket;
	poll_client_socket.events = POLLIN | POLLOUT;

	new_pfds->push_back(poll_client_socket);

	Client *new_client = new Client(client_socket);
	this->_clients.insert(std::pair<int, Client *>(client_socket, new_client));

	std::cout << std::endl
			  << "New client connected, his number is №" << client_socket << std::endl;
}

void Server::ServerIsFull(int client_socket)
{
	const char *err_msg = "[Server] You cannot join, the server is already full";

	send(client_socket, err_msg, strlen(err_msg) + 1, 0);
	close(client_socket);
	throw std::runtime_error(err_msg);
}

// TODO when we delete client we should send him quit message.
void Server::DelClient(int client_socket)
{
	Client *client;

	close(client_socket);
	
	// Delete client socket form pfds vector.
	std::vector<pollfd>::iterator it = this->_pfds.begin();
	while (it != this->_pfds.end() && it->fd != client_socket)
		it++;
	if (it != this->_pfds.end())
		this->_pfds.erase(it);

	// Get client from map by his socket.
	client = this->_clients.find(client_socket)->second;
	client->ClearCmdBuff();

	delete client;
}

void Server::SendWelcomeMsg(Client *client)
{
	client->_send_buff.append("001 " + client->GetNickname() + " :");
	client->_send_buff.append("\n \n✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧\n");
	client->_send_buff.append("✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧\n");
	client->_send_buff.append("✧✧✧✧✧✧✧✧✧✧✧꧁ WELCOME TO OUR FT_IRC SERVER!!!꧂✧✧✧✧✧✧✧✧✧✧✧\n");
	client->_send_buff.append("✧✧✧✧✧✧✧✧✧✧✧(´ ･ᴗ･`)✧✧✧✧＼(￣▽￣)／✧✧✧✧(´･ᴗ･ ` )✧✧✧✧✧✧✧✧✧\n");
	client->_send_buff.append("✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧✧\n");
	client->_send_buff.append("                    ⣀⣤⠚⠛⠛⠋⠉⠉⠉⠉⠉⠉⠉⠉⠙⠛⠛⠦⣄⣀\n");
	client->_send_buff.append("                 ⣀⠿⠛                      ⠙⢳⡄ \n");
	client->_send_buff.append("               ⢠⡿⠋                         ⠛⢷⣄\n");
	client->_send_buff.append("               ⣿⡇   Have fun and be nice     ⠘⣿\n");
	client->_send_buff.append("               ⣿⡇                          ⣠⡴⠟\n");
	client->_send_buff.append("               ⠙⢧                        ⣀⡼\n");
	client->_send_buff.append("                 ⠉⠛⠻⣷⣶⣤⣤⣄⣀⣀⣀⣀⣀⣀⣀⣠⣤⣤⣴⣶⠟⠛\n");
	client->_send_buff.append("                            ⣾⠏\n");
	client->_send_buff.append("                        ⣠⣾⡿⠁\n");
	client->_send_buff.append("                      ⣴⣾⠛⠋\n");
	client->_send_buff.append("                     ⣟⠋\n");
	client->_send_buff.append("                     ⠉\n");

	int chosen_message = rand() % NUM_WELCOME_MSGS;
	client->_send_buff.append(this->_welcomeMessages[chosen_message] + "\r\n");
	client->_send_buff.append(RPL_YOURHOST(client->GetNickname(), "ft_irc", "1.0"));
	// client->_send_buff.append(RPL_CREATED(client->GetNickname(), "04-07-2023 10:30:09")); TODO
	client->_send_buff.append(RPL_MYINFO(client->GetNickname(), "ft_irc", "1.0", "io", "kost", "k"));
	client->_send_buff.append(RPL_ISUPPORT(client->GetNickname(), "CHANNELLEN=32 NICKLEN=10 TOPICLEN=307"));
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

std::string Server::GetPassword()
{
	return this->_passwd;
}

std::map<int, Client *> *Server::GetClients()
{
	return &this->_clients;
}