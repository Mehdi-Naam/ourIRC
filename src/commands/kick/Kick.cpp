#include "Kick.hpp"
#include <__errc>
#include <cstddef>
#include <iostream>

KickCommand::KickCommand() {
	_currNick	= Server::get_server()->getCurrClt()->getNick();
	_comment	= " Kicked for Some Reasons";
}

void KickCommand::tokenChannels() {

	size_t		end_ch		= _parameter.find(' ');
	if (end_ch == std::string::npos) {
		utils::reply(ERR_NEEDMOREPARAMS(_currNick, "KICK"));
		utils::exit_msg("");
	}

	_channel	= _parameter.substr(0,end_ch);
}

void KickCommand::tokenUsers() {

	size_t		skip_ch	 = _parameter.find(' ');
	size_t		start_us = _parameter.find_first_not_of(' ', skip_ch);

	if (_parameter.at(start_us) == ':') {
		utils::reply(ERR_NEEDMOREPARAMS(_currNick, "KICK"));
		utils::exit_msg("");
	}

	size_t		end_us	 = _parameter.find(' ', start_us);
	std::string	users_s	 = _parameter.substr(start_us, end_us - start_us);

	utils::splitUsingSep(users_s, _users, ',');
}

void KickCommand::tokenComment() {

	size_t		start_cm	= _parameter.find(' ');
				start_cm	= _parameter.find_first_not_of(' ', start_cm);
				start_cm	= _parameter.find(' ', start_cm);
				if (start_cm == std::string::npos)
					return;
				start_cm	= _parameter.find_first_not_of(' ', start_cm);

	size_t		end			= std::string::npos;
	if (_parameter[start_cm] != ':')
		end = _parameter.find(' ', start_cm);
	_comment = _parameter.substr(start_cm, end - start_cm);
}

void KickCommand::prasseDriver() {

	tokenChannels();
	tokenUsers();
	tokenComment();
}

/*
	END parssing functions
*/

void KickCommand::checkChannel() {

	Server* ins = Server::get_server();

	_it_channel		=	ins->getChannels().find(_channel);
	if (_it_channel ==	ins->getChannels().end()) {
		utils::reply(ERR_NOSUCHCHANNEL(_currNick, _channel));
		utils::exit_msg("");
	}
}

void KickCommand::checkUsersIsInChannel() {

	mapSC	channelClient = _it_channel->second.getClients();

	for (size_t i = 0; i < _users.size(); i++) {
		size_t it = 0;
		for(; it < channelClient.size() ; it++)
			if (channelClient[it]->getNick() == _users[i])
				break  ;
		if (it == channelClient.size()) {
			utils::reply(ERR_NOTONCHANNEL(_users[i], _it_channel->first));
			_users.erase(_users.begin() + i);
			i = -1;
		}
	}
}

void KickCommand::kickTheUsers() {

	for (size_t i = 0; i < _users.size(); i++ ) {

		Client * clt = Server::get_server()->getClient(_users[i]);

		std::string user = Server::get_server()->getCurrClt()->getUser();
		std::string ip = Server::get_server()->getCurrClt()->get_ip();
		_it_channel->second.broadcast(RPL_KICK(_currNick, user, ip, _channel, _users[i], _comment));
		
		_it_channel->second.removeClient(clt, 1);
		_it_channel->second.removeAdmin(clt, 1);
		_it_channel->second.removeFromInvite(clt);

		std::cout << "\t\t\t" YEL << clt->getNick() << RED ": Kicked: " WHI << _comment << std::endl;
	}

	if (_it_channel->second.getClients().size() == 0)
			_it_channel = Server::get_server()->getChannels().erase(_it_channel);
}
 
void KickCommand::execute() {


	_parameter =  parsser::getInstence()->getParameter();
	if (_parameter.empty()) {
		utils::reply(ERR_NOSUCHCHANNEL(_currNick, _channel));
		utils::exit_msg("");
	}

	// parser the parameter ( get channels -- users -- the comment )
	std::cout << WHI "\t" "ParsserDriver        : " << std::flush;
	prasseDriver();
	std::cout << GRE "DONE" WHI << std::endl;

	// check if the channels are exist
	std::cout << "\t" "checkChannel         : " << std::flush;
	checkChannel();
	std::cout << GRE "DONE" WHI << std::endl;


	// check if the client run the command is Admin
	std::cout << "\t" "checkCltIsAdmin      : " << std::flush;
	Channel *channel = Server::get_server()->getChannel(_channel);
	channel->checkCltIsAdmin();
	std::cout << GRE "DONE" WHI << std::endl;
 
	// check if already Kicked
	// checkAlreadyKicked();

	// check if the users are exist on the channel
	std::cout << "\t" "checkUsersIsInChannel: " << std::flush;
	checkUsersIsInChannel();
	std::cout << GRE "DONE" WHI << std::endl;

	// add each client in the kicked list the channel
	std::cout << "\t" "kickTheUsers: " << std::endl;
	kickTheUsers();
	std::cout << GRE "\t\t ** DONE **" WHI << std::endl;

}

// Kick user from channel logic goes here.
// Verify if the requesting user has permissions to kick another user from the channel.
// Remove the user from the channel's user list.
// Notify the channel's users that a user has been kicked.
// Possibly send a message to the user who was kicked to inform them.