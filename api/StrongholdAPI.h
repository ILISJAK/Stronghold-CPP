#ifndef STRONGHOLD_API_H
#define STRONGHOLD_API_H

#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <vector>
#include "Player.h"

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

class StrongholdAPI
{
public:
	StrongholdAPI();
	void start();
	void stop();

private:
	void handleRequest(http_request request);
	void initializeGame(int numPlayers);
	Player *findPlayerByIdentifier(char identifier);

	std::vector<Player *> players;
	std::vector<Town *> towns;
	http_listener listener_;
};

#endif
