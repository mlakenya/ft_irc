#ifndef SERVERRESPONCES_HPP
# define SERVERRESPONCES_HPP

// Common
# define ERR_UNKNOWN_COMMAND(nickname, command) (":localhost 421 " + nickname + " " + command + " :Unknown command\r\n")
# define ERR_NEED_MORE_PARAMS(nickname, command) (":localhost 461 " + nickname + " " + command + " :Not enough parameters.\r\n")


// Welcome
# define RPL_YOUR_HOST(nickname, servername, version) (":localhost 002 " + nickname + " :Your host is " + servername + " (localhost), running version " + version + "\r\n")
# define RPL_CREATED(nickname, datetime) (":localhost 003 " + nickname + " :This server was created " + datetime + "\r\n")
# define RPL_MY_INFO(nickname, servername, version, user_modes, chan_modes, chan_param_modes) (":localhost 004 " + nickname + " " + servername + " " + version + " " + user_modes + " " + chan_modes + " " + chan_param_modes + "\r\n")
# define RPL_I_SUPPORT(nickname, tokens) (":localhost 005 " + nickname + " " + tokens " :are supported by this server\r\n")


// NICK
# define ERR_NO_NICKNAME_GIVEN(username) (":localhost 431 " + username + " :There is no nickname.\r\n")
# define ERR_ERRONEUS_NICKNAME(username, nickname) (":localhost 432 " + username + " " + nickname + " :Erroneus nickname\r\n")
# define ERR_NICKNAME_IN_USE(username, nickname) (":localhost 433 " + username + " " + nickname + " :Nickname is already in use.\r\n")

# define RPL_NICK(oldnickname, username, nickname) (":" + oldnickname + "!" + username + "@localhost NICK " +  nickname + "\r\n")


// PASS
# define ERR_PASSWD_MISMATCH(nickname) (":localhost 464 " + nickname + " :Password incorrect.\r\n")

// USER
# define ERR_ALREADY_REGISTERED(nickname) (":localhost 462 " + nickname + " :You may not reregister.\r\n")


// JOIN
# define ERR_BAD_CHANNEL_KEY(nickname, channel) ("475 " + nickname + " #" + channel + " :Cannot join channel (+k)\r\n")
# define ERR_CHANNEL_IS_FULL(nickname, channel) ("471 " + nickname + " #" + channel + " :Cannot join channel (+l)\r\n")
# define ERR_BANNED_FROM_CHAN(nickname, channel) ("474 " + nickname + " #" + channel + " :Cannot join channel (+b)\r\n")
# define RPL_JOIN(nickname, username, channel) (":" + nickname + "!" + username + "@localhost JOIN :#" +  channel + "\r\n")
# define RPL_TOPIC(nickname, channel, topic) (":localhost 332 " + nickname + " #" + channel + " " + topic + "\r\n")
# define RPL_NAMREPLY(nickname, symbol, channel, list_of_nicks) (":localhost 353 " + username + " " + symbol + " #" + channel + " :" + list_of_nicks + "\r\n")
# define RPL_ENDOFNAMES(nickname, channel) (":localhost 366 " + username + " #" + channel + " :End of /NAMES list.\r\n")

#endif