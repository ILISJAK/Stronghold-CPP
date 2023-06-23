#ifndef STRONGHOLD_API_H
#define STRONGHOLD_API_H

#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <vector>
#include "Player.h"
#include "Exception.h"
#include "Town.h"
#include "Archer.h"
#include "Knight.h"
#include "Maceman.h"
#include "Pikeman.h"
#include "Market.h"
#include "WoodCamp.h"
#include "WheatFarm.h"
#include "StoneQuarry.h"
#include "TurnManager.h"

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
	void deleteGameInstance();
	bool isGameInitialized();
	void initializeGameFromData(const json::value &savedData);
	Player *findPlayerByIdentifier(char identifier);
	void removePlayer(Player *player);
	std::string getTownProperty(Player *player, const std::wstring &property);
	std::string getStructureProperty(Town *town, const std::wstring &property);
	std::string getTroopProperty(Town *town, const std::wstring &property);
	std::vector<Player *> players;
	std::vector<Town *> towns;
	http_listener listener_;
	TurnManager *turnManager = nullptr;
};

#endif
