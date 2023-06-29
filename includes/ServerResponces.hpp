#ifndef SERVERRESPONCES_HPP
# define SERVERRESPONCES_HPP

// Common
# define ERR_UNKNOWN_COMMAND(nickname, command) (":localhost 421 " + nickname + " " + command + " :Unknown command\r\n")
# define ERR_NEED_MORE_PARAMS(nickname, command) (":localhost 461 " + nickname + " " + command + " :Not enough parameters.\r\n")


// NICK
# define ERR_NO_NICKNAME_GIVEN(username) (":localhost 431 " + username + " :There is no nickname.\r\n")
# define ERR_ERRONEUS_NICKNAME(username, nickname) (":localhost 432 " + username + " " + nickname + " :Erroneus nickname\r\n")
# define ERR_NICKNAME_IN_USE(username, nickname) (":localhost 433 " + username + " " + nickname + " :Nickname is already in use.\r\n")

# define RPL_NICK(oldnickname, username, nickname) (":" + oldnickname + "!" + username + "@localhost NICK " +  nickname + "\r\n")


// PASS
# define ERR_PASSWD_MISMATCH(nickname) (":localhost 464 " + nickname + " :Password incorrect.\r\n")

// USER
# define ERR_ALREADY_REGISTERED(nickname) (":localhost 462 " + nickname + " :You may not reregister.\r\n")

#endif