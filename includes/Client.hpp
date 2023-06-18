#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "stdafx.hpp"

class Client
{
private:
	int				_fd;
	std::string		_nick;
	std::string		_username;
	std::string		_full_name;

public:
	Client(int fd);
	~Client();
};

#endif