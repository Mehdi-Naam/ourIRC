// topic.cpp

#include "Topic.hpp"
#include <cstddef>

TopicCommand::TopicCommand() {
	_toEffect = NOEFFET;
	_currNick = Server::get_server()->getCurrClt()->getNick();
}

void TopicCommand::_tokenChannel() {

	size_t		end_ch		= _parame.find(' ');
	if (end_ch == std::string::npos && _toEffect == NOEFFET)
		_toEffect = DISPALY;

	_nameCh	= _parame.substr(0,end_ch);
}

void TopicCommand::_tokenTopic() {

	if (_toEffect == DISPALY)
		return;

	size_t 	start_t = _parame.find(' ');
			start_t = _parame.find_first_not_of(' ', start_t);

	size_t end = std::string::npos;
	if (_parame[start_t] != ':')
		end = _parame.find(' ', start_t);
	std::string topicTmp = _parame.substr(start_t, end - start_t);

	if (topicTmp.size() == 1 && topicTmp[0] == ':' )
		_toEffect = CLEAR_T;
	else {
	 	_toEffect = CHANGE;
		_topicS   = topicTmp.substr(topicTmp[0] == ':',std::string::npos);
	}
}

void TopicCommand::_checkChannel() {

	Server* ins = Server::get_server();

	_itChannel		= ins->getChannels().find(_nameCh);
	if (_itChannel == ins->getChannels().end()) {

		utils::reply(ERR_NOSUCHCHANNEL(_currNick, _nameCh));
		utils::exit_msg("");
	}
}

void TopicCommand::_checkCltExistInChannel() {

	Client*		currClt		= Server::get_server()->getCurrClt();
	mapSC		channelClt	= _itChannel->second.getClients();

	mapSC::iterator it = std::find(channelClt.begin(), channelClt.end(), currClt);
	if (it != channelClt.end())
		return ;

	utils::reply(ERR_NOSUCHCHANNEL(currClt->getNick(), _itChannel->first));
	utils::exit_msg("");
}

void TopicCommand::_TopicRun() {

	if (_toEffect != DISPALY ) {

		_itChannel->second.setTopiChannel(_topicS);
		std::cout << "\t\t" "Channel: " << _itChannel->first << ": " << _topicS << std::endl;
		_itChannel->second.broadcast(RPL_TOPIC(_currNick, _itChannel->first, _topicS));

		// send the RPL_TOPICWHOTIME (333)
		return;
	}
	std::string ChanneTopic = _itChannel->second.getTopicChannel();
	if (ChanneTopic.empty()) {

		utils::reply(RPL_NOTOPIC(_currNick, _itChannel->first));
		utils::exit_msg("RPL_NOTOPIC (331) : send it to all Clt in Channel");
	}
	std::cout << "\t\t" GRE "ChannelTopic: " WHI << ChanneTopic << std::endl;
	utils::reply(RPL_TOPIC(_currNick, _itChannel->first, ChanneTopic));
}

void TopicCommand::execute()
{
	_parame = parsser::getInstence()->getParameter();
	if (_parame.empty()) {
		utils::reply(ERR_NEEDMOREPARAMS(_currNick, "TOPIC"));
		utils::exit_msg("");
	}
	std::cout << "\t" "_tokenChannel   : " WHI << std::flush;
	_tokenChannel();
	std::cout << GRE "DONE" WHI << std::endl;

	std::cout << "\t" "_tokenTopic    : " << std::flush;
	_tokenTopic  ();
	std::cout << GRE "DONE" WHI << std::endl;


	// check if channel exist
	std::cout << "\t" "_checkChannel  : " << std::flush;
	_checkChannel();
	std::cout << GRE "DONE" WHI << std::endl;


	// check if The curr is exist in channel
	std::cout << "\t" "_checkCltExistInChannel: " << std::flush;
	_checkCltExistInChannel();
	std::cout << GRE "DONE" WHI << std::endl;
	
	// check if the channel in topic_p mode
	std::cout << "\t" "_checkCltAdmin: " << std::flush;
	if (_itChannel->second.getTopicMode())
		_itChannel->second.checkCltIsAdmin();
	std::cout << GRE "DONE" WHI << std::endl;

	std::cout << "\t" "_TopicRun: " << std::endl;
	_TopicRun();
	std::cout << GRE "\t" " ** DONE **" WHI << std::endl;

}