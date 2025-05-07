#pragma once

#include "../../Global.hpp"

class ICommands;
class parsser;

typedef std::map<std::string, std::string> mapOfStrStr;

class PrvmsgCommand : public ICommands
{

	public:
		void execute();

	private:
		void				_ParsDriver      ();
		void				_tokenTheMsg     ();
		void				_tokenTheReciever();
		void				_SeparatChanOfClt();


		void				_sendLoopDriver ();
		void				_sendMsg4clients(std::string const &);
		void				_sendMsg4channel(std::string );

	private:
		std::string			_buff;
		std::string			_currNick;

		std::string			_toSend;
		vecOfString			_reciever;

		mapOfStrStr			_clients;
		vecOfString			_channels;
		vecOfString			_channels4Admins;
};
