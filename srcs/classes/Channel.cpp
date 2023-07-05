#include "../../includes/Channel.hpp"

/*
 *	۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝
 *	۝   Constructor&destructor   ۝
 *	۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝۝
 */

Channel::Channel(std::string const &channelName): _name(channelName), _capacity_limit(-1)
{
	_banned_users.clear();
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
std::string&						Channel::GetChannelPassword()	{ return (_channel_password); }
int&								Channel::GetCapacityLimit()	{ return (_capacity_limit); }
std::map <std::string, Client*>&	Channel::GetClientList()	{ return (_clientList); }
std::vector<std::string>&			Channel::GetBannedUsers()	{ return (_banned_users); }
std::vector<std::string>&			Channel::GetKickedUsers()	{ return (_kicked_users); }
std::vector<std::string>&			Channel::GetVoicedUsers()	{ return (_voiced_users); }
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

		if (this->IsOperator(nick))
			members_list += "@";
		members_list += nick;
		members_list += " ";

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

void	Channel::AddToKicked(std::string &kicked_name)
{
	std::vector<std::string>::iterator it;
	for (it = _kicked_users.begin(); it != _kicked_users.end(); it++)
	{
		if (*it == kicked_name)
		{
			std::cout << kicked_name << " is already kicked from the channel " << GetName() << std::endl;
			return ;
		}
	}
	_kicked_users.push_back(kicked_name);
	std::cout << RED << kicked_name << " is now kicked from the channel " << GetName() << RESET << std::endl;
}

void	Channel::AddToBanned(std::string &banned_name)
{
	std::vector<std::string>::iterator it;
	for (it = _banned_users.begin(); it != _banned_users.end(); it++)
	{
		if (*it == banned_name)
		{
			std::cout << banned_name << " is already banned from the channel " << GetName() << std::endl;
			return ;
		}
	}
	_banned_users.push_back(banned_name);
	std::cout << RED << banned_name << " is now banned from the channel " << GetName() << RESET << std::endl;
}

void	Channel::RemoveFromBanned(std::string &banned_name)
{
	std::vector<std::string>::iterator user;
	for (user = _banned_users.begin(); user != _banned_users.end(); user++)
	{
		if (*user == banned_name)
		{
			_banned_users.erase(user);
			std::cout << banned_name << " is not banned anymore from the channel " << GetName() << std::endl;
			return ;
		}
	}
	std::cout << "No need! " << banned_name << " has never been banned from the channel " << GetName() << std::endl;
}

bool	Channel::IsBanned(std::string &banned_name)
{
	std::vector<std::string>::iterator user;
	if (_banned_users.empty() == true) // work
		return (false);
	for (user = _banned_users.begin(); user != _banned_users.end(); user++)
	{
		if (*user == banned_name)
			return (true);
	}
	return (false);	
}

void	Channel::AddToVoiced(std::string &voiced_name)
{
	std::vector<std::string>::iterator it;
	for (it = _voiced_users.begin(); it != _voiced_users.end(); it++)
	{
		if (*it == voiced_name)
		{
			std::cout << voiced_name << " is already voiced from the channel " << GetName() << std::endl;
			return ;
		}
	}
	_voiced_users.push_back(voiced_name);
	std::cout << RED << voiced_name << " is now voiced from the channel " << GetName() << RESET << std::endl;
}

void	Channel::RemoveFromVoiced(std::string &voiced_name)
{
	std::vector<std::string>::iterator user;
	for (user = _voiced_users.begin(); user != _voiced_users.end(); user++)
	{
		if (*user == voiced_name)
		{
			_voiced_users.erase(user);
			std::cout << voiced_name << " is not voiced anymore from the channel " << GetName() << std::endl;
			return ;
		}
	}
	std::cout << "No need! " << voiced_name << " has never been voiced from the channel " << GetName() << std::endl;
}

bool	Channel::IsVoiced(std::string &voiced_name)
{
	std::vector<std::string>::iterator user;
	if (_voiced_users.empty() == true) // work
		return (false);
	for (user = _voiced_users.begin(); user != _voiced_users.end(); user++)
	{
		if (*user == voiced_name)
			return (true);
	}
	return (false);	
}

/*
*				################################
*				###	  OPERATORS FUNCTIONS    ###
*				################################
*/

void	Channel::AddFirstOperator(std::string operatorName)
{
	if (_operators.empty())
		_operators.push_back(operatorName);
}

void	Channel::RemoveOperator(std::string operatorName)
{
	std::vector<std::string>::iterator it;
	for (it = _operators.begin(); it != _operators.end(); it++)
	{
		if (*it == operatorName)
			_operators.erase(it);
	}
}

bool 	Channel::IsOperator(std::string &operatorName)
{
	std::vector<std::string>::iterator user;
	if (_operators.empty())
		return (false);
	for (user = _operators.begin(); user != _operators.end(); user++)
	{
		if (*user == operatorName)
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
}

void	Channel::RemovePassword()
{
	_channel_password.clear();
}
