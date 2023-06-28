#include "Client.hpp"

Client::Client(int fd) : _fd(fd), _status(PASSWORD)
{
}

Client::~Client()
{
	(void)_fd;
}

UserStatus Client::GetStatus()
{
	return this->_status;
}

void Client::SetStatus(UserStatus status)
{
	if (status == PASSWORD)
		return ;
	
	if (status == AUTHENTICATION)
	{
		if (_status == PASSWORD)
			_status = status;
		else
			return ;
	}

	if (status == ONLINE)
	{
		if (_status == AUTHENTICATION && !_nick.empty() \
			&& !_username.empty())
			_status = status;
		else
			return ;
	}

	if (status == DELETED)
	{
		if (_status == ONLINE)
			_status = status;
		else
			return ;
	}

}
