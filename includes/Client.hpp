#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "stdafx.hpp"
# include "CmdList.hpp"

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

	std::string		_nickname;
	std::string		_username;
	std::string		_full_name;
	UserStatus		_status;
	cmdList			*_cmd_buff;

public:
	std::string		_recv_buff;
	std::string		_send_buff;

	Client(int fd);
	~Client();

	/*    ===== Accessors =====     */
	UserStatus	GetStatus();
	void		SetStatus(UserStatus);
	std::string	GetNickname();
	void		SetNickname(std::string nickname);
	std::string	GetUsername();
	void		SetUsername(std::string username);
	void		SetFullName(std::string full_name);
	cmdList		*GetCmdBuff();
	void 		SetCmdBuff(cmdList *cmd_buff);
	void		DeleteCommand(cmdList *cmd);
};

#endif