#pragma once 


/* START MESSAGE  */

# define HELP_CMD                                          "KICK - Eject a client from the channel\nMODE - Change the channel’s mode\nINVITE - Invite a client to an invite-only channel (mode +i)\nTOPIC - Change the channel topic in a mode +t channel"
/************************************** replies Commands ***********************************************************/

# define RPL_WELCOME(nickName)							    ":VANGUARD 001 " + nickName + " :Welcome to the Internet Relay Chat " + nickName + "\r\n"
# define RPL_YOURHOST(nickName)							    ":VANGUARD 002 " + nickName + " :Your host is VANGUARD, running version 1.1\r\n"
# define OUR_SERVER					                        "█████   █████   █████████   ██████   █████   █████████  █████  █████   █████████   ███████████   ██████████  \n░░███   ░░███   ███░░░░░███ ░░██████ ░░███   ███░░░░░███░░███  ░░███   ███░░░░░███ ░░███░░░░░███ ░░███░░░░███ \n ░███    ░███  ░███    ░███  ░███░███ ░███  ███     ░░░  ░███   ░███  ░███    ░███  ░███    ░███  ░███   ░░███\n ░███    ░███  ░███████████  ░███░░███░███ ░███          ░███   ░███  ░███████████  ░██████████   ░███    ░███\n ░░███   ███   ░███░░░░░███  ░███ ░░██████ ░███    █████ ░███   ░███  ░███░░░░░███  ░███░░░░░███  ░███    ░███\n  ░░░█████░    ░███    ░███  ░███  ░░█████ ░░███  ░░███  ░███   ░███  ░███    ░███  ░███    ░███  ░███    ███ \n    ░░███      █████   █████ █████  ░░█████ ░░█████████  ░░████████   █████   █████ █████   █████ ██████████  \n     ░░░      ░░░░░   ░░░░░ ░░░░░    ░░░░░   ░░░░░░░░░    ░░░░░░░░   ░░░░░   ░░░░░ ░░░░░   ░░░░░ ░░░░░░░░░░   \n* * VANGUARD :made by  << fooxCoder | Molchi | Sinshi >> * * \r\n"
# define OUS_SERVER					                        ":VANGUARD  003 :made by  << fooxCoder | Molchi | Sinshi >> * * \r\n"

//? MODE :
#define  RPL_MODE(nickName, channel, modes)            		":" + nickName + " MODE " + channel + " " + modes + " "  + "\r\n"
# define ERR_UNKNOWNMODE(nickName, char)				    ":VANGUARD 472 " + nickName + " " +  char + " :is unknown mode char to me\r\n"
# define ERR_UMODEUNKNOWNFLAG(nickName)		                ":VANGUARD 501 " + nickName + " " + " :Unknown MODE flag\r\n"
# define ERR_USERSDONTMATCH(nickName)					    ":VANGUARD 502 " + nickName + " :Cant change mode for other users\r\n"
# define ERR_USERALRADYOP(nickName,target,channel)		    ":VANGUARD " + nickName + " NOTICE " + target + " :" + target + " is already an operator on " + channel + "\r\n"
# define RPL_CHANNELMODEIS(nickName, channel, mode)			":VANGUARD 324 " + nickName + " " + channel + " " + mode + "\r\n"

//? JOIN :

# define RPL_JOIN(nickName, localhost, user,  channel)		":" + nickName + "!~" + user + "@" + localhost + " JOIN " + channel + "\r\n"
# define RPL_NAMREPLY(nickName, channel, users)			    ":VANGUARD 353 " + nickName + " = " + channel + " :" + users + "\r\n"
# define RPL_ENDOFNAMES(nickName, channel)                  ":VANGUARD 366 " + nickName + " " + channel + " :End of /NAMES list\r\n"
# define ERR_BANNEDFROMCHAN(nickName, channel)              ":VANGUARD 474 " + nickName + " " + channel + " :Cannot join channel (+b)\r\n"
# define ERR_INVITEONLYCHAN(nickName, channel)              ":VANGUARD 473 " + nickName + " " + channel + " :Cannot join channel (+i)\r\n"
# define ERR_BADCHANNELKEY(nickName, channel)               ":VANGUARD 475 " + nickName + " " + channel + " :Cannot join channel (+k)\r\n"
# define ERR_BADCHANMASK(channel)             				":VANGUARD 475 " + channel + " :Bad Channel Mask\r\n"
//  ERR_CHANNELISFULL ERR_NOSUCHCHANNEL RPL_TOPIC ERR_NEEDMOREPARAMS

//? INVITE :
# define RPL_INVITING(nickName, target, channel)		    ":VANGUARD 341 " + nickName + " " + target + " " + channel + "\r\n"
# define RPL_INVITED(nickName, channel)			            ":VANGUARD 345 " + nickName + " :You have been invited to " + channel + "\r\n"
# define ERR_USERONCHANNEL(nickName, channel)		":VANGUARD 443 " + nickName  + " " + channel + " :is already on channel\r\n"
//  ERR_NEEDMOREPARAMS ~ ERR_NOSUCHNICK ~ ERR_NOTONCHANNEL ~ ERR_USERONCHANNEL ~ ERR_CHANOPRIVSNEEDED

//? KICK :
# define RPL_KICK(nickName,user, localhost, channel, kickuser, msg) ":" + nickName + "!~" + user + "@" + localhost + " KICK " + channel + " " + kickuser + " :" + msg + "\r\n"
//  ERR_NEEDMOREPARAMS ~ ERR_NOSUCHCHANNEL ~ ERR_CHANOPRIVSNEEDED ~ ERR_NOTONCHANNEL

//? CHANNEL :
# define ERR_NOSUCHCHANNEL(nickName , channel)			    ":VANGUARD 403 " + nickName + " " + channel + " :No such channel\r\n"
# define ERR_USERNOTINCHANNEL(nickName, target, channel)    ":VANGUARD 441 " + nickName + " " + target + " " + channel + " :They aren't on that channel\r\n"
# define ERR_NOTONCHANNEL(nickName, channel)			    ":VANGUARD 442 " + nickName + " " + channel + " :You're not on that channel\r\n"
# define ERR_CHANNELISFULL(nickName, channel)			    ":VANGUARD 471 " + nickName + " " + channel + " :Cannot join channel (+l)\r\n"
# define ERR_CHANOPRIVSNEEDED(nickName, channel)		    ":VANGUARD 482 " + nickName + " " + channel + " :You're not channel operator\r\n"
# define ERR_CANNOTSENDTOCHAN(nickName, channel)		    ":VANGUARD 404 " + nickName + " " + channel + " :Cannot send to channel\r\n"

//? PRIVMSG :
# define RPL_PRIVMSG(nickName, user, localhost, receive, msg)		":" + nickName + "!~" + user + "@" + localhost + " PRIVMSG " + receive + " " + msg + "\r\n"
//  ERR_NEEDMOREPARAMS ~ ERR_NOTEXTTOSEND ~ ERR_CANNOTSENDTOCHAN ~ ERR_NOSUCHNICK


//? TOPIC :
# define RPL_TOPIC(nickName, channel , topic)		    ":VANGUARD 332 " + nickName + " " + channel + " :" + topic + "\r\n"
# define RPL_NOTOPIC(nickName, channel)		            ":VANGUARD 331 " + nickName + " " + channel + " :No topic is set" + "\r\n"
// ERR_NEEDMOREPARAMS ~ ERR_NOTONCHANNEL ~ ERR_CHANOPRIVSNEEDED

//? NICK :
# define RPL_NICK(nickName, user, localhost, newNickName)			":" + nickName +  " NICK " + newNickName + "\r\n"
# define ERR_NOSUCHNICK(nickName, target)				            ":VANGUARD 401 " + nickName + " " + target + " :No such nick\r\n"
# define ERR_NONICKNAMEGIVEN						                ":VANGUARD 431 :No nickname given\r\n"
# define ERR_ERRONEUSNICKNAME(nickName, user)			            ":VANGUARD 432 " + nickName + " " + user + " :Erroneus nickname\r\n"
# define ERR_NICKNAMEINUSE(nickName, user)			                ":VANGUARD 433 " + nickName + " " + user + " :Nickname is already in use\r\n"

//? PASS :
# define ERR_PASSWDMISMATCH							    ":VANGUARD 464 FAIL :Password incorrect\r\n"
// ERR_NEEDMOREPARAMS ERR_ALREADYREGISTRED

//? USER :
# define ERR_ALREADYREGISTRED(nickName)					":VANGUARD 462 " + nickName + " :You may not reregister\r\n"

// ERR_NEEDMOREPARAMS


/************************************** OTHER_ERROR_MESSAGES ***********************************************************/
# define ERR_NOSUCHSERVER(serv)						    ":VANGUARD 402 " + serv + " :No such server\r\n"
# define ERR_NOTEXTTOSEND(nickName)						":VANGUARD 412 " + nickName + " :No text to send\r\n"
# define ERR_UNKNOWNCOMMAND							    ":VANGUARD 421 * :Command not found\r\n"

# define ERR_NOTREGISTERED							    ":VANGUARD 451 * :You have not registered\r\n"

# define ERR_NEEDMOREPARAMS(nickName, command)			":VANGUARD 461 " + nickName + " " + command + " :Not enough parameters\r\n"

# define ERR_NOPRIVILEGES(nickName)						":VANGUARD 481 " + nickName + " :Permission Denied- You're not an IRC operator\r\n"

// ? FOR PART
// # define RPL_QUIT(nickName, user, localhost, chName)	":" + nickName + "!~" + user + "@" + localhost + " PART " + chName + "\r\n"
# define RPL_QUIT(nickName, user, localhost, chName)	":" + nickName + "!~" + user + "@" + localhost + " QUIT " + chName + "\r\n"
