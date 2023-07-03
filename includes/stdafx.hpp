#ifndef STDAFX_HPP
# define STDAFX_HPP

/* Includes */

# include <iostream>
# include <string>
# include <sys/socket.h>
# include <vector>
# include <map>
# include <poll.h>
# include <sys/types.h>
# include <netdb.h>
# include <cstring>
# include <exception>
# include <unistd.h>
# include <algorithm>
# include <sstream>
# include <fstream>

/* Defines */

# define FAILURE		    -1
# define MAX_CLIENT 	    11
# define BUFFER_SIZE	    1024
# define NUM_WELCOME_MSGS   1

// Color
# define RED            "\033[0;31m"
# define RESET          "\033[0m"

# define DEBUG 1

/* Global */

extern bool								shutdown_signal;

#endif