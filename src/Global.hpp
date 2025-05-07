#pragma once

/************************* standard libraries ************************/
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

typedef std::vector<std::string> vecOfString;
typedef std::string				string;

/************************* CommandFactory and related command headers ************************/
#include "commands/CommandFactory.hpp"
#include "commands/invite/Invite.hpp"
#include "commands/join/Join.hpp"
#include "commands/kick/Kick.hpp"
#include "commands/nick/Nick.hpp"
#include "commands/user/User.hpp"
#include "commands/pass/Pass.hpp"
#include "commands/bot/Bot.hpp"
#include "commands/mode/Mode.hpp"
#include "commands/prvmsg/Prvmsg.hpp"
#include "commands/topic/Topic.hpp"
#include "commands/ICommands.hpp"
#include "response/Response.hpp"

/*
 *** name-space to get enum-command
 */

namespace CmdSpace
{
	enum CmdType
	{
		CMDNOTFOUND,
		PASS,
		NICK,
		USER,
		MODE,
		KICK,
		JOIN,
		TOPIC,
		INVITE,
		BOT,
		PRIVMSG,
		PONG,
		QUIT
	};
	CmdType getCommandEnum(std::string const &);
}

/************************* BOT ************************/

#define TOKEN_FILE "/tmp/token.txt"
#define INFO_FILE "/tmp/info.txt"
#define UID "u-s4t2ud-1ec3b8eda237c0c014f5a639649ec51330f3834694046f15ebfa008a274eca46"
#define SECRET "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\""
#define _URL_TOKEN " https://api.intra.42.fr/oauth/token"
#define _URL_INFO "\" https://api.intra.42.fr/v2/users/"


/************************* parser ************************/
#include "parser/GlobalParser.hpp"
#include "parser/utils/Utils.hpp"

/************************* Server headers ************************/
#include <sys/socket.h> //=> for socket
#include <sys/types.h>	//=> for socket
#include <netdb.h>		//=> for addrinfo
#include <fcntl.h>		//=> for fcntl
#include "server/Server.hpp"
#include "server/client/Client.hpp"
#include "server/socket/Socket.hpp"
#include "server/channels/Channel.hpp"
#include "commands/Authentication/Authentication.hpp"
#include <poll.h>	   //=> for poll
#include <vector>	   //=> for vector
#include <string>	   //=> for strings
#include <arpa/inet.h> //=> for inet_ntoa
#include <unistd.h>	   //=> for close
#include <map>


#define MAX_BUFF 512

/************************* colors ************************/
#define RED "\e[1;31m"
#define BLU "\e[0;34m"
#define BLE "\e[1;34m"
#define LOO "\e[1;35m"
#define WHI "\e[0;37m"
#define GRE "\e[1;32m"
#define YEL "\e[1;33m"