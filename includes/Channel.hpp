#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Server.hpp"

class	Client;

class Channel
{
private:
	std::map<std::string, Client *>	_clientList;
	std::vector<std::string>		_kicked_users;
	std::vector<std::string>		_banned_users;
	std::vector<std::string>		_operators;
	std::vector<std::string>		_voiced_users;
	std::string 					_name;
	std::string						_operatorPassword;
	std::string						_topic;
	std::string						_mode;
	std::string						_channel_password;
	int								_capacity_limit;

public:
	Channel(std::string const &name);
	~Channel();

	/* Accessors */
	std::string&					GetName();
	std::string&					GetTopic();
	std::string&					GetMode();
	std::string&					GetPassword();
	int&							GetCapacityLimit();
	std::vector<std::string>&		GetOperators();
	std::vector<std::string>&		GetKickedUsers();
	std::vector<std::string>&		GetBannedUsers();
	std::vector<std::string>&		GetVoicedUsers();
	std::map <std::string,Client*>&	GetClientList();
	std::string						GetListOfMembers();
	std::string						GetSymbol();
	void							SetTopic(std::string& newTopic);
	void							SetPassword(std::string password);
	void							SetCapacityLimit(int limit);
	bool							DoesClientExist(std::string &client_name);
	
	/* Manage client in Channel */
	void							AddClient(Client *client);
	void							RemoveClient(std::string &client_name);
	
	/* Operators */
	void							AddFirstOperator(std::string operator_name);
	void							RemoveOperator(std::string operator_name);
	bool							IsOperator(std::string &operator_name);
	
	/* Modes */
	void							AddMode(std::string const mode);
	void							RemoveMode(std::string const mode);
	void							RemovePassword();
	
	/* Client status */
	void							AddToKicked(std::string &banned_name);
	void							AddToBanned(std::string &banned_name);
	void							RemoveFromBanned(std::string &banned_name);
	bool							IsBanned(std::string &banned_name);
	void							AddToVoiced(std::string &voiced_name);
	void							RemoveFromVoiced(std::string &voiced_name);
	bool							IsVoiced(std::string &voiced_name);

};

#endif