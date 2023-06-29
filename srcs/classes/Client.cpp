#include "Client.hpp"

Client::Client(int fd) : _fd(fd), _status(PASSWORD)
{
}

Client::~Client()
{
	(void)_fd;
}

/*
*	۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝
*	۝         ACCESSORS          ۝
*	۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝
*/

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
		if (_status == AUTHENTICATION && !_nickname.empty() \
			&& !_username.empty())
		{
			this->_send_buff += "You're wellcome!\r\n";
			this->_status = status;
		}
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

std::string	Client::GetNickname()
{
	return this->_nickname;
}

void Client::SetNickname(std::string nickname)
{
	this->_nickname = nickname;
}

std::string	Client::GetUsername()
{
	return this->_username;
}

void Client::SetUsername(std::string username)
{
	this->_username = username;
}

void Client::SetFullName(std::string full_name)
{
	this->_full_name = full_name;
}
