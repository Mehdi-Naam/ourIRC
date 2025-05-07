#include "Prvmsg.hpp"
#include <cstddef>
#include <utility>

void PrvmsgCommand::_tokenTheReciever() {

	size_t first_space = utils::find_first_sp( _buff );
	if (first_space == std::string::npos)
		utils::reply(ERR_NOTEXTTOSEND(_currNick));

	std::string users = _buff.substr(0, first_space);
	utils::splitUsingSep(users, _reciever, ',');
}

void PrvmsgCommand::_tokenTheMsg() {

	size_t	msg_ss		= _buff.find(' ');
	size_t	msg_start	= _buff.find_first_not_of(' ', msg_ss);
	size_t	msg_end		= std::string::npos;

	if (_buff[msg_start] != ':') {
		size_t	skip_users;

		skip_users = utils::find_first_sp(_buff);
		msg_start  = _buff.find_first_not_of(' ', skip_users);
		msg_end    = _buff.find(' ', msg_start) - msg_start;
	}

	_toSend = _buff.substr(msg_start, msg_end);
	_buff.clear();
}

void PrvmsgCommand::_SeparatChanOfClt() {

	std::string isChannel ("@#&");

	for (size_t recv_i = 0; recv_i < _reciever.size(); recv_i++) {

		if ( isChannel.find(_reciever[recv_i][0]) != std::string::npos )
			_channels.push_back(_reciever[recv_i]);
		else
			_clients.insert(std::make_pair(_reciever[recv_i], _reciever[recv_i]));
	}
	_reciever.clear();
}

void PrvmsgCommand::_ParsDriver() {

	_currNick	= Server::get_server()->getCurrClt()->getNick();

	if( (_buff	= parsser::getInstence()->getParameter()).empty() )
		utils::reply(ERR_NEEDMOREPARAMS(_currNick, "PRIVMSG"));

	_tokenTheReciever();
	std::cout << WHI "_tokenReciever    : " GRE "DONE" WHI << std::endl; 

	_tokenTheMsg     ();
	std::cout << WHI "_tokenTheMsg      : " GRE "DONE" WHI << std::endl; 

	_SeparatChanOfClt();
	std::cout << WHI "_SeparatChanOfClt : " GRE "DONE" WHI << std::endl; 
}

void PrvmsgCommand::_sendMsg4clients(std::string const & nickName) {

	// check if the nick name is exist
	Client * myClt = Server::get_server()->getClient(nickName);
	if (!myClt) {
		utils::reply(ERR_NOSUCHNICK(_currNick, nickName));
		return ;
	}

	std::string user ( Server::get_server()->getCurrClt()->getUser() );
	std::string message( RPL_PRIVMSG(_currNick, user , Server::get_server()->getCurrClt()->get_ip(), nickName, _toSend) );

	if ( send(myClt->get_fd(), message.c_str(), message.size(), 0) == -1 )
		utils::exit_msg("PrvmsgCommand::_sendMsg4clients::send");

}

void PrvmsgCommand::_sendMsg4channel(std::string channelName) {

	bool prefix = channelName[0] == '@' ? true : false; 

	if (prefix)
		channelName.erase(0, 1);

	// check if the Channel is Exist
	Channel * channel = Server::get_server()->getChannel(channelName);
	if (!channel){
		utils::reply(ERR_NOSUCHCHANNEL(_currNick, channelName));
		return ;
	}

	// check if the client is Kicked or Not in the Channel
	if ( !channel->clientIsExist() ) {
		utils::reply(ERR_CANNOTSENDTOCHAN(_currNick, channelName));
		return ;
	}

	// if the clt banned from the channel fail the command silently
	// if ( channel->clientKicked() )
	// 	return ;

	// BroadCast the msg to all users or operators
	channel->broadcastPrivMsg(_toSend, prefix);
}

void PrvmsgCommand::_sendLoopDriver() {

	for ( mapOfStrStr::iterator clt_i = _clients.begin(); clt_i != _clients.end(); clt_i++)
		_sendMsg4clients(clt_i->first);

	for ( size_t ch_i = 0; ch_i < _channels.size(); ch_i++)
		_sendMsg4channel(_channels[ch_i]);

}

void PrvmsgCommand::execute()
{
	_ParsDriver();
	_sendLoopDriver();
}
