#pragma once

#include "../ICommands.hpp"
#include "../../Global.hpp"

class client;

typedef std::vector<string>			vec_s;

typedef std::pair<string,string>	pair_ss;
typedef std::vector<pair_ss>		vecPair;		


class JoinCommand : public ICommands
{

	public:
		void execute();

	private: /* Methods */

		void	parssChanPass();

		void	makeThemPair     ();
		void	tokenChNames_Pwds();

		void	generateChannels();
		void	driveChannel();
		bool	existChannel(size_t const &);
		void	creatChannel(size_t const &);

	private:
		string	currNickName;
		string	currUser;

		pair_ss	names_passwords;

		vec_s	ch_names;
		vec_s	ch_passwords;

		vecPair Channels_pswds;

};