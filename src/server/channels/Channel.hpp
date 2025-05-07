#pragma once
#include "../../Global.hpp"
#include <string>

class Client;

typedef std::string string;
typedef std::vector<Client *> mapSC;

class Channel
{

public:
	Channel();
	Channel(string const &, string const &);

	void setNameChannel(string const &);
	void setPassChannel(string const &);
	void setTopiChannel(string const &);
	void setAdmiChannel();

	//? MODE:
	void setInviteOnlyMode(bool);
	void setTopicChangeMode(bool);
	void setChannelKey(string const);
	void addOperator(Client *user);
	void removeOperator(Client *user);
	void setUserLimit(int);
	void broadcast(const std::string &message);
	void broadcast(const std::string &message, Client *exclude);
	void checkCltIsAdmin();
	bool clientIsExist(const std::string &);

	void genStringOfChannelModes();


	//? INVITE:
	void addInvitation(Client *user);

	string getNameChannel() const;
	string getPassChannel() const;
	string getTopicChannel() const;

	mapSC const &getAdmins() const;
	mapSC const &getClients() const;
	mapSC const &getInvitList() const;

	void addAdmin();
	void addClient();

	bool clientIsExist();
	bool clientInvited();

	bool const &getTopicMode() const;
	bool const &getInvitMode() const;
	bool const &getRestrict () const;

	void broadcastPrivMsg  (const std::string &, bool);
	void try_broadcast(const std::string &msg);

	void eraseClient();

	// FOR KICK
	void removeClient(Client *, bool);
	void removeAdmin(Client *, bool);
	void removeFromInvite(Client *);

	// FOR  JOIN
	string gen_channel_member_ships();


private:
	bool	_restrict;
	bool	_invitMode;
	bool	_topicMode;
	size_t	_user_limit;

	string	_ch_name;
	string	_ch_password;
	string	_ch_topic;

	mapSC	_ch_admins;
	mapSC	_ch_clients;

	mapSC	_ch_inviters;
};