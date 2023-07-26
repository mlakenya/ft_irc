#ifndef SERVERRESPONCES_HPP
# define SERVERRESPONCES_HPP

// Common
# define ERR_UNKNOWN_COMMAND(nickname, command) (":localhost 421 " + nickname + " " + command + " :Unknown command\r\n")
# define ERR_NEED_MORE_PARAMS(nickname, command) (":localhost 461 " + nickname + " " + command + " :Not enough parameters.\r\n")
# define ERR_NO_SUCH_CHANNEL(nickname, channel) ("403 " + nickname + " #" + channel + " :No such channel \r\n")


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
# define RPL_TOPIC(nickname, channel, topic) (":localhost 332 " + nickname + " #" + channel + " " + topic + " \r\n")
# define RPL_NAMREPLY(nickname, symbol, channel, list_of_nicks) (":localhost 353 " + username + " " + symbol + " #" + channel + " :" + list_of_nicks + "\r\n")
# define RPL_ENDOFNAMES(nickname, channel) (":localhost 366 " + username + " #" + channel + " :End of /NAMES list.\r\n")

// KICK
# define ERR_CHAN_OP_RIVS_NEEDED(nickname, channel) ("482 " + nickname + " #" + channel + " :You're not channel operator \r\n")
# define ERR_NO_TON_CHANNEL(nickname, channel) ("442 " + nickname + " #" + channel + " :You're not on that channel \r\n")
# define ERR_USER_NO_TIN_CHANNEL(nickname, channel) ("441 " + nickname + " #" + channel + " :They aren't on that channel \r\n")

// MODE
# define RPL_UMODEIS(nickname, mode_string) ("221 " + nickname + " " + mode_string + " \r\n")
# define RPL_MODE_USER(nickname, mode_string) (":" + nickname + " MODE " + nickname + " :" + mode_string + "\r\n")
# define ERR_NO_SUCH_NICK(nickname, user_name) ("401 " + nickname + " " + user_name + " :No such nick/channel \r\n")
# define RPL_CHANNEL_MODEIS(nickname, channel, mode_string) ("324 " + nickname + " " + channel + " " + mode_string + " \r\n")
# define ERR_UMODE_UNKNOWN_FLAG (nickname) ("501" + nickname + " :Unknown MODE flag \r\n")


//PING
# define ERR_NO_ORIGIN(nickname) ("409 " + nickname + " #" + " :No origin specified \r\n")
# define RPL_PONG(nickname, user_name) (":" + nickname + "!" + user_name + "@localhost PONG :" + "localhost" + "\r\n")

//INVITE
# define RPL_INVITING(nickname, user_name, channel_name) ("341 " + nickname + " " + user_name + " " + channel_name + " \r\n")

//PRIVMSG
# define ERR_NO_RECIPI_ENT(nickname, command) ("411" + nickname + " :No recipient given " + command + " \r\n")
# define ERR_NO_TEXT_TO_SEND(nickname) ("412" + nickname + " :No text to send \r\n")
# define RPL_PRIVMSG(nickname, user_name, target, message) (":" + nickname + "!" + user_name + "@localhost PRIVMSG " + target + " :" + message + "\r\n")

// WHO
# define RPL_WHOREPLY(client, channel, nickname, username) (":localhost 352 " + client + " " + channel + nickname + " localhost localhost " + nickname + " H :0 " + username + "\r\n")
# define RPL_ENDOFWHO(nickname) (":localhost 315 " + nickname + " :End of /WHO list\r\n")

#endif