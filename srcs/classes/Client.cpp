#include "Client.hpp"

Client::Client(int fd) : _fd(fd), status(PASSWORD)
{
}

Client::~Client()
{
	(void)_fd;
}