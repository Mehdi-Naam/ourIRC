#include "Channel.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <exception>
#include <string>

Channel::Channel(string const &chName, string const &chPswd)
{

	_ch_name = chName;
	_ch_password = chPswd;
	_ch_topic = "";
	_topicMode = false;
	_invitMode = false;
	if (chName[0] == '&')
		_invitMode = true;
	_restrict = _invitMode;
	_user_limit = 0;
}

/*
 *** Setters
 */

void Channel::setNameChannel(string const &nc)
{
	_ch_name = nc;
}

void Channel::setPassChannel(string const &pc)
{
	_ch_password = pc;
}

void Channel::setTopiChannel(string const &tc)
{
	_ch_topic = tc;
}

void Channel::setAdmiChannel()
{

	Client *clt = Server::get_server()->getCurrClt();
	if (!clt)
		utils::exit_msg("No Current Clt ...");
	_ch_admins.push_back(clt);
	_ch_clients.push_back(clt);
	std::cout << GRE "\t\t\tthe " << clt->getNick() << " added as Admin/client to " << _ch_name << WHI << std::endl;
}

/*
 *** Getters
 */

string Channel::getNameChannel() const
{
	return _ch_name;
}

string Channel::getPassChannel() const
{
	return _ch_password;
}

string Channel::getTopicChannel() const
{
	return _ch_topic;
}

mapSC const &Channel::getInvitList() const
{
	return _ch_inviters;
}

mapSC const &Channel::getAdmins() const
{
	return _ch_admins;
}

mapSC const &Channel::getClients() const
{
	return _ch_clients;
}

bool const &Channel::getTopicMode()const 
{
	return _topicMode;
}

bool const &Channel::getInvitMode()const 
{
	return _invitMode;
}

bool const & Channel::getRestrict() const {
	return _restrict;
}
/*
		*** Methods ***
*/
//			 *** bool Methods

bool Channel::clientInvited()
{

	Client *to_find = Server::get_server()->getCurrClt();

	if (!to_find)
		utils::exit_msg("Error: clientInvited: Current Clt empty ...");

	for (mapSC::iterator i = _ch_inviters.begin(); i != _ch_inviters.end(); i++)
		if ((*i)->getNick() == to_find->getNick())
			return true;
	return false;
}

bool Channel::clientIsExist()
{

	Client *to_find = Server::get_server()->getCurrClt();

	for (size_t i = 0; i < _ch_clients.size(); i++)
		if (_ch_clients[i]->getNick() == to_find->getNick())
			return true;
	return false;
}

bool Channel::clientIsExist(const std::string &to_find)
{

	for (size_t i = 0; i < _ch_clients.size(); i++)
		if (_ch_clients[i]->getNick() == to_find)
			return true;
	return false;
}

void Channel::addClient()
{
	Client *clt = Server::get_server()->getCurrClt();

	// check if the channel is mode invite
	if (_invitMode)
		if (!clientInvited())
		{ // check if the client is invited
			utils::reply(ERR_INVITEONLYCHAN(clt->getNick(), _ch_name));
			utils::exit_msg(YEL "\t\t\tClient Not Invited" WHI);
		}

	// check if the channel had a limit-Mode
	if (_user_limit > 0 && _ch_clients.size() >= _user_limit ) {
		utils::reply(ERR_CHANNELISFULL(clt->getNick(), _ch_name));
		utils::exit_msg("The Channel is full");
	}

	_ch_clients.push_back(clt);

	try_broadcast(RPL_JOIN(clt->getNick(), clt->get_ip(), clt->getUser(), _ch_name));

	if (!_ch_topic.empty())
		utils::reply(RPL_TOPIC(clt->getNick(), _ch_name, _ch_topic));

	utils::reply(RPL_NAMREPLY(clt->getNick(), _ch_name, gen_channel_member_ships() ));
    utils::reply(RPL_ENDOFNAMES(clt->getNick(), _ch_name));

	std::cout << GRE "\t\t\ttheClient " << clt->getNick() << " added to " << _ch_name << WHI << std::endl;
}

void Channel::setInviteOnlyMode(bool mode)
{
	_invitMode = mode;
	std::string modeChangeMsg = (_invitMode ? "+i" : "-i");
	try_broadcast(RPL_MODE(Server::get_server()->getCurrClt()->getNick(), getNameChannel(), modeChangeMsg));
}

void Channel::setTopicChangeMode(bool mode)
{
	_topicMode = mode;
	std::string modeChangeMsg = (_topicMode ? "+t" : "-t");
	try_broadcast(RPL_MODE(Server::get_server()->getCurrClt()->getNick(), getNameChannel(), modeChangeMsg));
}

void Channel::setChannelKey(string const key)
{
	std::string modeChangeMsg;

	if (key.empty())
	{
		_ch_password.clear();
		modeChangeMsg = "-k";
	}
	else
	{
		_ch_password = key;
		modeChangeMsg = "+k";
	}

	try_broadcast(RPL_MODE(Server::get_server()->getCurrClt()->getNick(), getNameChannel(), modeChangeMsg));
}

void Channel::setUserLimit(int limit)
{
	if (limit < 0)
		return ;
	_user_limit = limit;
	std::string modeChangeMsg = (_user_limit > 0) ? "+l" : "-l";
	try_broadcast(RPL_MODE(Server::get_server()->getCurrClt()->getNick(), getNameChannel(), modeChangeMsg));
}

void Channel::addOperator(Client *client)
{

	if (!clientIsExist(client->getNick()))
		utils::reply(ERR_USERNOTINCHANNEL(Server::get_server()->getCurrClt()->getNick(), client->getNick(), getNameChannel()));

	else
	{
		bool isOperator = false;
		for (std::vector<Client *>::iterator it = _ch_admins.begin(); it != _ch_admins.end(); ++it)
		{
			if (*it == client)
			{
				isOperator = true;
				break;
			}
		}
		if (!isOperator)
		{
			_ch_admins.push_back(client);
			std::string modeChangeMsg = "+o " + client->getNick();
			try_broadcast(RPL_MODE(Server::get_server()->getCurrClt()->getNick(), getNameChannel(), modeChangeMsg));
		}
		else
		{
			utils::reply(ERR_USERALRADYOP(Server::get_server()->getCurrClt()->getNick(), client->getNick(), getNameChannel()));
		}
	}
}

void Channel::addInvitation(Client *client)
{

	if (clientIsExist(client->getNick()))
		utils::reply(ERR_USERONCHANNEL(client->getNick(), getNameChannel()));
	else
	{
		_ch_inviters.push_back(client);
		// removeKickedClient(client);			// will remove the Client from the Kicked-list-Channel if is founded 
		std::string invitation = RPL_INVITED(client->getNick(), getNameChannel());
		if ( send(client->get_fd(), invitation.c_str(), invitation.size(), 0) == -1)
			utils::exit_msg("invite::send::Failed");
	}
}

void Channel::removeOperator(Client *client)
{
	for (mapSC::iterator it = _ch_admins.begin(); it != _ch_admins.end(); ++it)
	{
		if (*it == client)
		{
			Client * newOp = NULL;
			// check if there is only one Admin
			if (_ch_admins.size() == 1)
			{
				// check if there is someone can take Role
				if (_ch_clients.size() > 1)
					newOp = _ch_clients[0] != client ? _ch_clients[0] : _ch_clients[1];
				else
					utils::exit_msg("Can not remove the privilage you are the only one on the channel");
			}

			std::string modeChangeMsg = "-o " + client->getNick();
			try_broadcast(RPL_MODE(Server::get_server()->getCurrClt()->getNick(), getNameChannel(), modeChangeMsg));
			_ch_admins.erase(it);

			if (newOp != NULL)
				addOperator(newOp);
			return;
		}
	}
	utils::reply(ERR_USERNOTINCHANNEL(Server::get_server()->getCurrClt()->getNick(), client->getNick(), getNameChannel()));
}

void Channel::broadcast(const std::string &message)
{
	mapSC::iterator it = _ch_clients.begin();
	mapSC::iterator end = _ch_clients.end();

	while (it != end)
	{
		if (*it)
		{
			int fd = (*it)->get_fd();
			if (send(fd, message.c_str(), message.size(), 0) == -1)
				utils::exit_msg("Error: channel.cpp/brodcast/send");
		}
		++it;
	}
}

void Channel::broadcastPrivMsg(const std::string &msg, bool op) /* broadcast a msg to ALL channel members */
{
	std::string currClt = Server::get_server()->getCurrClt()->getNick();
	std::string userClt = Server::get_server()->getCurrClt()->getUser();
	std::string ipClt = Server::get_server()->getCurrClt()->get_ip();

	mapSC::iterator it, end;

	if (op)
	{
		it = _ch_admins.begin();
		end = _ch_admins.end();
	}
	else
	{
		it = _ch_clients.begin();
		end = _ch_clients.end();
	}

	while (it != end)
	{
		if (*it && *it != Server::get_server()->getCurrClt())
		{
			int fd = (*it)->get_fd();
			std::string message(RPL_PRIVMSG(currClt, userClt, ipClt, _ch_name, msg));
			if (send(fd, message.c_str(), message.size(), 0) == -1)
				utils::exit_msg("Channel::broadcastPrivMsg::send");
		}
		++it;
	}
}

void Channel::broadcast(const std::string &message, Client *exclude)
{
	mapSC::iterator begin = _ch_clients.begin();
	mapSC::iterator end = _ch_clients.end();

	while (begin != end)
	{
		if (*begin == exclude)
		{
			++begin;
			continue;
		}
		if (*begin)
		{
			int fd = (*begin)->get_fd();
			if (send(fd, message.c_str(), message.size(), 0) == -1)
				utils::exit_msg("send");
		}
		++begin;
	}
}

void Channel::checkCltIsAdmin()
{

	std::string currClt = Server::get_server()->getCurrClt()->getNick();

	for (mapSC::iterator i = _ch_admins.begin(); i != _ch_admins.end(); i++)
		if ((*i)->getNick() == currClt)
			return;

	utils::reply(ERR_CHANOPRIVSNEEDED(currClt, _ch_name));
	utils::exit_msg("");
}

void Channel::eraseClient()
{

	Client *to_find = Server::get_server()->getCurrClt();

	if (!clientIsExist(to_find->getNick()))
		return;

	removeClient(to_find, 0);
	removeAdmin(to_find, 0);
	removeFromInvite(to_find);

	if (_ch_admins.size() == 0 && _ch_clients.size() != 0 ) {
		Client * newOp = _ch_clients[0];
		// _ch_admins.push_back(newOp);
		addOperator(newOp);
		std::cout << newOp->getNick() << "Is Now Operator" << std::endl;
	}
}


// FOR KICK NEW UPDATE
void Channel::removeClient(Client *client, bool isKick)
{
	mapSC::iterator it = std::find(_ch_clients.begin(), _ch_clients.end(), client);
	if (it != _ch_clients.end()) {
		string nickName, user, localhost;
		nickName = (*it)->getNick();
		user     = (*it)->getUser();
		localhost= (*it)->get_ip();
		_ch_clients.erase(it);
		if (!isKick)
			try_broadcast(RPL_QUIT(nickName, user, localhost, _ch_name));
	}
}

void Channel::removeAdmin(Client *client, bool isKick) {

	mapSC::iterator it = std::find(_ch_admins.begin(), _ch_admins.end(), client);
	if (it != _ch_admins.end()){

		std::string modeChangeMsg = "-o " + client->getNick();
		_ch_admins.erase(it);
		if (isKick)
			try_broadcast(RPL_MODE(Server::get_server()->getCurrClt()->getNick(), getNameChannel(), modeChangeMsg));
	}
}

void Channel::removeFromInvite(Client *client) {

	mapSC::iterator it = std::find(_ch_inviters.begin(), _ch_inviters.end(), client);
	if (it != _ch_inviters.end())
		_ch_inviters.erase(it);
}

string Channel::gen_channel_member_ships() {

	std::string final_string;
	mapSC	ch_clients_cpy = _ch_clients;

	for (size_t i = 0; i  < _ch_admins.size(); i++) {
		string mem_ship = "@" + _ch_admins[i]->getNick() + " ";
		final_string.append(mem_ship);
		mapSC::iterator it = std::find(ch_clients_cpy.begin(), ch_clients_cpy.end(), _ch_admins[i]);
		if (it != ch_clients_cpy.end())
			ch_clients_cpy.erase(it);
	}

	for (size_t i = 0; i < ch_clients_cpy.size(); i++) {
		string mem_ship =  ch_clients_cpy[i]->getNick();
		if (i < ch_clients_cpy.size() - 1 )
			mem_ship.append(" ");
		final_string.append(mem_ship);
	}

	return final_string;
}

void Channel::genStringOfChannelModes() {

	std::string ch_Modes ("+n");
	std::string info_s   (""  );

	if (_topicMode)
		ch_Modes += "t";
	if (_invitMode)
		ch_Modes += "i";
	if (_user_limit > 0) {
		ch_Modes += "l";
		info_s += " " + utils::IntToString(_user_limit);
	}
	if (!_ch_password.empty()) {
		ch_Modes += "k";
		info_s += " " + _ch_password;
	}

	ch_Modes += info_s;
	utils::reply(RPL_CHANNELMODEIS(Server::get_server()->getCurrClt()->getNick(), _ch_name, ch_Modes));
}

void	Channel::try_broadcast(const std::string &msg) {
	try {
		broadcast(msg);
	} catch (std::exception & e) {
		e.what();
	}
}
