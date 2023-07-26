#include "../includes/Server.hpp"
#include "../includes/Commands.hpp"

// Global variables for signal handling and map of commands.
bool shutdown_signal = false;
std::map<std::string, void(*)(Server *, Client *, cmdList *)> command_funcs;

int main(int argc, char **argv)
{	
	if (argc != 3)
	{
		std::cout << RED << "ERROR! Invalid arguments. " << RESET << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	
	srand((unsigned)time(NULL));
	signal(SIGINT, set_shutdown_signal);

	Server *server;
	try
	{
		server = new Server(argv[1], argv[2]);
		server->Prepare();
		InitCommandFunctions();
		ServerManager(server);
	}
	catch(const std::exception& e)
	{
		std::cerr << RED << e.what() << RESET << '\n';
	}
	
	delete server;
}

// Init map of command's names and functions.
// Functions should be declared in Commands.hpp file.
void InitCommandFunctions()
{
	// command_funcs["OPER"] = OPER;
	// command_funcs["QUIT"] = *QUIT;
	command_funcs["PRIVMSG"] = *PRIVMSG;
	command_funcs["PING"] = *PING;
	command_funcs["INVITE"] = *INVITE;
	command_funcs["TOPIC"] = *TOPIC;
	command_funcs["MODE"] = *MODE;
	command_funcs["JOIN"] = *JOIN;
	command_funcs["KICK"] = *KICK;
	command_funcs["NICK"] = *NICK;
	command_funcs["USER"] = *USER;
	command_funcs["PASS"] = *PASS;
}

void	set_shutdown_signal(int signal)
{
	(void)signal;
	shutdown_signal = true;
}
