#include "Client.hpp"

Client::Client(int fd)
: _fd(fd), _status(PASSWORD), _cmd_buff(NULL)
{
}

Client::~Client()
{
	(void)_fd;
	this->_cmd_buff = NULL;
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
			this->_status = status;
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
	if (nickname.size() <= 10)
		this->_nickname = nickname;
	else
		this->_nickname = nickname.substr(0, 10);
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

cmdList		*Client::GetCmdBuff()
{
	return this->_cmd_buff;
}

void Client::SetCmdBuff(cmdList *cmd_buff)
{
	this->_cmd_buff = cmd_buff;
}

void		Client::DeleteCommand(cmdList *cmd)
{
	if (this->_cmd_buff == NULL || cmd == NULL)
		return ;

	cmdList *tmp = this->_cmd_buff;
	// If the command is the first in the list.
	if (cmd == tmp)
		this->_cmd_buff = this->_cmd_buff->next;
	else
	{
		while (tmp->next != NULL && tmp->next != cmd)
			tmp = tmp->next;

		tmp->next = cmd->next;
	}
	delete cmd;
}
