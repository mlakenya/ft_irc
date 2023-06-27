#include "../../includes/Server.hpp"

void 		trim(std::string& str);
cmdList 	*CreateCmdList();

void ParseMessage(std::string message, cmdList **commands)
{
	std::stringstream	ss(message);
	std::string			line;
	cmdList 			*lst = NULL;


	while (getline(ss, line))
	{
		trim(line);
		if (line.empty())
			continue;

		// Add command to list.
		if (lst != NULL)
		{
			lst->next = CreateCmdList();
			lst = lst->next;
		}
		else
		{
			lst = CreateCmdList();
			*commands = lst;
		}

		std::string copy = line;

		// PREFIX
		if (line[0] == ':')
		{
			lst->prefix = line.substr(1, copy.find_first_of(' '));
			if (line.find_first_of(' ') != std::string::npos)
				copy.erase(0, copy.find_first_of(' ') + 1);
		}
		
		// COMMAND
		if (copy.find_first_of(' ') == std::string::npos)
		{
			lst->command = copy;
			copy.erase(0, lst->command.length());
		}
		else
		{
			lst->command = copy.substr(0, copy.find_first_of(' '));
			copy.erase(0, lst->command.length() + 1);
		}
		for (size_t i = 0; i < lst->command.size(); i++)
			lst->command[i] = std::toupper(lst->command[i]);

		// TRAILING
		std::string delimiter(":");
		size_t position;
		if ((position = copy.find(delimiter)) != std::string::npos)
		{
			std::string tmp = copy.substr(0, position);
			copy.erase(0, position + delimiter.length());
			lst->trailing = copy;
			copy = tmp;
		}
		
		// PARAMETERS
		std::string param;
		std::stringstream ss(copy);
		while (ss >> param)
			lst->parameters.push_back(param);


		// Adding parsed string to the list of commands.
		if (*commands != NULL)
			commands = &lst;		
	}
}

void trim(std::string& str)
{
	std::string whitespaces = " \r\n\t";

	size_t first = str.find_first_not_of(whitespaces);
	if (first == std::string::npos)
	{
		str = "";
		return;
	}
	size_t last = str.find_last_not_of(whitespaces);
	str = str.substr(first, (last - first + 1));
}

cmdList *CreateCmdList()
{
	cmdList *lst = new cmdList;
	
	lst->command = "";
	lst->prefix = "";
	lst->trailing = "";

	lst->next = NULL;

	return lst;
}
