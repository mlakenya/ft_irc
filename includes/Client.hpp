#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "stdafx.hpp"

enum UserStatus
{
	PASSWORD,
	AUTHENTICATION,
	ONLINE,
	DELETED
};

class Client
{
private:
	int				_fd;

	std::string		_nick;
	std::string		_username;
	std::string		_full_name;
	UserStatus		_status;

public:
	std::string		_recv_buff;
	std::string		_send_buff;

	Client(int fd);
	~Client();
	UserStatus	GetStatus();
	void		SetStatus(UserStatus);
};

#endif