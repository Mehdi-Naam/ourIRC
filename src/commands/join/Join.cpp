#include "Join.hpp"
#include <exception>
#include <sstream>
#include <utility>

void JoinCommand::parssChanPass()
{

	currNickName = Server::get_server()->getCurrClt()->getNick();
	currUser = Server::get_server()->getCurrClt()->getUser();
	std::string param = parsser::getInstence()->getParameter();

	if (param.empty())
	{
		utils::reply(ERR_NEEDMOREPARAMS(currNickName, "JOIN"));
		utils::exit_msg("");
	}

	std::string Channels_, passWords, error;

	std::istringstream seprator(param);

	(seprator >> Channels_ >> passWords);

	/*
		for : join #ch pass word :: ignore the `word`
	*/
	// if ((seprator >> error))
	// 	utils::exit_msg("More than Two Args gived to Join");

	names_passwords = std::make_pair(Channels_, passWords);
}

static void channelNameSyn(vec_s const &ch_names)
{

	std::string ch_syn("#&");

	for (size_t i = 0; i < ch_names.size(); i++)
	{

		if (ch_names[i].length() == 1 || ch_syn.find(ch_names[i][0]) == std::string::npos || !utils::checkTrueName(ch_names[i], 'c'))
		{
			utils::reply(ERR_BADCHANMASK(ch_names[i]));
			utils::exit_msg(RED "Please Channels Names Start by #OR& and a name" WHI);
		}
	}
}

void JoinCommand::tokenChNames_Pwds()
{

	std::cout << "\t"
				 "token channels:"
			  << std::flush;

	utils::splitUsingSep(names_passwords.first, ch_names, ',');
	utils::splitUsingSep(names_passwords.second, ch_passwords, ',');

	// check that channels Name Start with ` # ` or ` & `
	channelNameSyn(ch_names);

	std::cout << GRE " DONE" WHI << std::endl;
}

void JoinCommand::makeThemPair()
{

	std::cout << "\t"
				 "make  pair    :"
			  << std::flush;
	for (size_t i_ch = 0; i_ch < ch_names.size(); i_ch++)
	{
		if (ch_names[i_ch].empty())
			continue;

		if (i_ch >= ch_passwords.size())
		{
			Channels_pswds.push_back(std::make_pair(ch_names[i_ch], ""));
			continue;
		}

		//	for : join #ch :pass   : take `pass` as passWord
		if (ch_passwords[i_ch][0] == ':')
			ch_passwords[i_ch].erase(0,1);

		Channels_pswds.push_back(std::make_pair(ch_names[i_ch], ch_passwords[i_ch]));
	}
	std::cout << GRE " DONE" WHI << std::endl;
}

static bool passWordIsCorrect(string ch_pass, string clt_pass)
{

	if (ch_pass.empty())
		return true;
	if (!ch_pass.empty() && clt_pass.empty())
		return false;
	if (ch_pass == clt_pass)
		return true;
	return false;
}

bool JoinCommand::existChannel(size_t const &i_ch)
{

	std::cout << "\t" BLU "check existChannel: " WHI << std::flush;

	// check if the Channel is already Exist:
	Server *ins = Server::get_server();
	chn_map_it it_channel = ins->getChannels().find(Channels_pswds[i_ch].first);
	if (it_channel == ins->getChannels().end())
		return false;

	std::cout << ": " << Channels_pswds[i_ch].first << ": Founded" << std::endl;

	if (it_channel->second.clientIsExist()) {
		utils::reply(ERR_USERONCHANNEL(currNickName, Channels_pswds[i_ch].first));
		utils::exit_msg(BLE "\t\t\taddClient: Already Exist in Channel" WHI);
	}

	// check if the passWord is Correct
	if (!passWordIsCorrect(it_channel->second.getPassChannel(), Channels_pswds[i_ch].second))
	{
		utils::reply(ERR_BADCHANNELKEY(currNickName, Channels_pswds[i_ch].first));
		utils::exit_msg(RED "\t\t\tError: PassWord: Incorrect" WHI);
	}

	// addTheClient to the channel
	it_channel->second.addClient();

	return true;
}

void JoinCommand::creatChannel(size_t const &i_ch)
{

	std::cout << "Channel Not Found" << std::endl;

	string Ch_name = Channels_pswds.at(i_ch).first;
	string Ch_pswd = Channels_pswds.at(i_ch).second;

	std::cout << "\t  " BLU "create new Channel: " WHI << Ch_name << std::endl;

	Channel NewChannel(Ch_name, Ch_pswd);

	std::cout << "\t\t"
				 "adding the client: "
			  << std::endl;
	NewChannel.setAdmiChannel();
	Server::get_server()->getChannels().insert(std::make_pair(Ch_name, NewChannel));

	utils::reply(RPL_JOIN(currNickName, Server::get_server()->getCurrClt()->get_ip(), currUser, Ch_name));
	utils::reply(RPL_NAMREPLY(currNickName, Ch_name, "@" + currNickName));
    utils::reply(RPL_ENDOFNAMES(currNickName, Ch_name));
}

void JoinCommand::generateChannels()
{

	for (size_t i_ch = 0; i_ch < Channels_pswds.size(); i_ch++)
	{
		try {
			if (!existChannel(i_ch))
				creatChannel(i_ch);
		} catch (std::exception & e) {
			std::cout << e.what();
		}
	}
}

void JoinCommand::execute()
{
	parssChanPass();
	tokenChNames_Pwds();
	makeThemPair();
	generateChannels();
}