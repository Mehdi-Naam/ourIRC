#pragma once

#include "../../Global.hpp"
#include "../ICommands.hpp"


#define NOEFFET 0
#define CLEAR_T 1
#define DISPALY 2
#define CHANGE	3


class TopicCommand : public ICommands {

	public:
		TopicCommand();
		void execute();

	private:
		void	_parssParmater();
		void	_tokenChannel ();
		void	_tokenTopic   ();

		void	_checkChannel ();

		void	_checkCltExistInChannel();
		void	_checkCltAdmin();

		void	_TopicRun();

	private:
		std::string			_currNick;

		std::string			_parame;

		std::string			_nameCh;
		std::string			_topicS;

		int					_toEffect;
		chn_map::iterator	_itChannel;
};