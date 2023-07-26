#include "../../includes/Channel.hpp"

/*
 *	۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝
 *	۝   Constructor&destructor   ۝
 *	۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝
 */

Channel::Channel(std::string const &channelName): _name(channelName), _capacity_limit(-1)
{
	_clientList.clear();
	_topic.clear();
}

Channel::~Channel() {}

/*
 *	۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝
 *	۝         ACCESSORS          ۝
 *	۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝
 */

std::string&						Channel::GetName() 			{ return (_name); }
std::string&						Channel::GetTopic() 		{ return (_topic); }
std::string&						Channel::GetMode()			{ return (_mode); }
std::string&						Channel::GetPassword()	{ return (_channel_password); }
int&								Channel::GetCapacityLimit()	{ return (_capacity_limit); }
std::map <std::string, Client*>&	Channel::GetClientList()	{ return (_clientList); }
std::vector<std::string>&			Channel::GetOperators() 	{ return (_operators); }

std::string Channel::GetListOfMembers()
{
	std::string members_list;
	std::string	nick;
	std::map<std::string, Client*>::iterator it;

	it = this->_clientList.begin();
	while (it != this->_clientList.end())
	{
		nick = it->second->GetNickname();

		if (!it->second->GetModeI() || this->IsOperator(nick))
		{
			if (this->IsOperator(nick))
				members_list += "@";
			members_list += nick;
			members_list += " ";
		}
		it++;
	}

	if (members_list.size() >= 1 && members_list[members_list.size() - 1] == ' ')
		members_list.erase(members_list.end()-1);
	
	return members_list;
}

std::string Channel::GetSymbol()
{
	if (this->GetMode().find('s') != std::string::npos)
		return "@";
	else if (this->GetMode().find('p') != std::string::npos)
		return "*";
	return "=";
}

void		Channel::SetTopic(std::string& newTopic)
{
	_topic = newTopic;
}

void		Channel::SetPassword(std::string password)
{
	_channel_password = password;
}

void		Channel::SetCapacityLimit(int limit)
{
	_capacity_limit = limit;
}

bool		Channel::DoesClientExist(std::string &clientName)
{	
	if (_clientList.size() == 0)
		return (false);

	std::map <std::string, Client*>::iterator it = _clientList.find(clientName);
	if (it == _clientList.end())
		return (false);
	return (true);
}

/*
*				################################
*				### MANAGE CLIENT FUNCTIONS  ###
*				################################
*/

void	Channel::RemoveClient(std::string &clientName)
{
	std::map <std::string, Client*>::iterator it = this->_clientList.find(clientName);
	if (it != _clientList.end())
		this->_clientList.erase(it);
	
	// This function checks if the ClientName is present in the Operator list
	// and if so, deletes it.
	RemoveOperator(clientName); 
}

/*
*				################################
*				###	 CLIENT STATUS FUNCTIONS ###
*				################################
*/

void	Channel::AddClient(Client *client)
{
	_clientList.insert(std::pair<std::string, Client*>(client->GetNickname(), client));
}

/*
*				################################
*				###	  OPERATORS FUNCTIONS    ###
*				################################
*/

void	Channel::AddFirstOperator(std::string operator_name)
{
	if (_operators.empty())
		_operators.push_back(operator_name);
}

void	Channel::AddOperator(std::string operator_name)
{
	_operators.push_back(operator_name);
}

void	Channel::RemoveOperator(std::string operator_name)
{
	std::vector<std::string>::iterator it;
	for (it = _operators.begin(); it != _operators.end(); it++)
	{
		if (*it == operator_name)
			_operators.erase(it);
	}
}

bool 	Channel::IsOperator(std::string &operator_name)
{
	std::vector<std::string>::iterator user;
	if (_operators.empty())
		return (false);
	for (user = _operators.begin(); user != _operators.end(); user++)
	{
		if (*user == operator_name)
			return (true);
	}
	return (false);
}

/*
*				################################
*				###	    MODES FUNCTIONS      ###
*				################################
*/

void	Channel::AddMode(std::string const mode)
{
	if (_mode.empty() == true)
		_mode = "+" + mode;
	else 
		_mode += mode;
}

void	Channel::RemoveMode(std::string const mode)
{
	size_t pos = _mode.find(mode);
	_mode.erase(pos, 1);
	if(_mode.size() == 1)
		_mode = "";
}

void	Channel::RemovePassword()
{
	_channel_password.clear();
}
