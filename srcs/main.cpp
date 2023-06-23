#include "../includes/Server.hpp"

// TODO 
void ServerManager(Server *server);

int main(int argc, char **argv)
{
	Server server(argv[1], argv[2]);

	if (argc != 3)
	{
		//TODO
		std::cout << "ERROR. Invalid number of arguments. Example: ... " << std::endl;
		return 1;
	}
	try
	{
		server.Prepare();

		ServerManager(&server);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}



}
