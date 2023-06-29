#include "../includes/Server.hpp"
#include "../includes/Commands.hpp"

bool shutdown_signal = false;
std::map<std::string, void(*)(Server *, Client *, cmdList *)> command_funcs;

int main(int argc, char **argv)
{	
	if (argc != 3)
	{
		//TODO
		std::cout << "ERROR. Invalid number of arguments. Example: ... " << std::endl;
		return 1;
	}
	try
	{
		Server server(argv[1], argv[2]);
		server.Prepare();
		InitCommandFunctions();
		ServerManager(&server);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

// TODO
void InitCommandFunctions()
{
	// command_funcs["default"] = *InitCommandFunctions;
	// command_funcs["INVITE"] = INVITE;
	// command_funcs["JOIN"] = JOIN;
	// command_funcs["KICK"] = KICK;
	// command_funcs["KILL"] = KILL;
	// command_funcs["LIST"] = LIST;
	// command_funcs["MODE"] = MODE;
	// command_funcs["MOTD"] = MOTD;
	// command_funcs["NAMES"] = NAMES;
	command_funcs["NICK"] = *NICK;
	// command_funcs["NOTICE"] = NOTICE;
	// command_funcs["OPER"] = OPER;
	// command_funcs["PART"] = PART;
	// command_funcs["PING"] = PING;
	// command_funcs["PRIVMSG"] = PRIVMSG;
	// command_funcs["QUIT"] = QUIT;
	// command_funcs["TOPIC"] = TOPIC;
	command_funcs["USER"] = *USER;
	command_funcs["PASS"] = *PASS;
}