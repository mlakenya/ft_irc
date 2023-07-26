#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Server.hpp"

# define ACCEPTABLE_MODES "itolk"

class	Client;

class Channel
{
private:
	std::map<std::string, Client *>	_clientList;
	std::vector<std::string>		_operators;
	std::string 					_name;
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
	void							AddOperator(std::string operator_name);
	
	/* Modes */
	void							AddMode(std::string const mode);
	void							RemoveMode(std::string const mode);
	void							RemovePassword();

};

#endif