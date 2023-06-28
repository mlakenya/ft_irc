#ifndef CMDLIST_HPP
# define CMDLIST_HPP

#include "stdafx.hpp"

// It's a linked list that stores parsed lines
struct cmdList
{
	std::string prefix;
	std::string command;
	std::vector<std::string> parameters;
	std::string trailing;

	cmdList *next;
};

#endif
