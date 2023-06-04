#include "StrongholdAPI.h"
#include <iostream>
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

StrongholdAPI::StrongholdAPI() : listener_(L"http://localhost:8080")
{
	listener_.support(methods::POST, std::bind(&StrongholdAPI::handleRequest, this, std::placeholders::_1));
}

void StrongholdAPI::start()
{
	listener_.open().wait();
	std::cout << "Stronghold API is listening on http://localhost:8080" << std::endl;
}

void StrongholdAPI::stop()
{
	listener_.close().wait();
	std::cout << "Stronghold API has stopped" << std::endl;
}

void StrongholdAPI::handleRequest(http_request request)
{
	if (request.method() == methods::POST)
	{
		if (request.request_uri().path() == L"/start-game")
		{
			// Handle the "start-game" action
			// Implement the necessary logic for starting a game
			try
			{
				// Your game initialization code here
				// Example:
				// Retrieve the number of players from the request body
				json::value requestBody = request.extract_json().get();
				int numPlayers = requestBody[L"numPlayers"].as_integer();

				// Initialize the game and create towns for each player
				initializeGame(numPlayers);

				json::value response;
				response[L"message"] = json::value::string(L"Game started");
				request.reply(status_codes::OK, response);
			}
			catch (const std::exception &e)
			{
				json::value response;
				response[L"error"] = json::value::string(U("Game started error"));
				request.reply(status_codes::InternalError, response);
			}
		}
		else if (request.request_uri().path() == L"/make-move")
		{
			// Handle the "make-move" action
			// Implement the necessary logic for making a move
			try
			{
				// Your game move logic here
				// Example:
				// Retrieve the move details from the request body
				json::value requestBody = request.extract_json().get();
				std::wstring action = requestBody[L"action"].as_string();

				// Retrieve the player identifier from the request body
				char playerIdentifier = requestBody[L"playerIdentifier"].as_string()[0];
				// Find the player by identifier
				Player *player = findPlayerByIdentifier(playerIdentifier);
				if (!player)
				{
					throw std::runtime_error("Player not found");
				}

				// Process the move action on the player's town
				Town *town = player->getTown();

				if (action == L"train-villager")
				{
					int amount = requestBody[L"amount"].as_integer();
					// Call the trainVillager method on the town, passing the amount
					town->trainVillager(amount);
				}
				else if (action == L"build-housing")
				{
					int amount = requestBody[L"amount"].as_integer();
					// Call the buildHousing method on the town, passing the amount
					town->buildHousing(amount);
				}
				// Implement other move actions in a similar manner

				json::value response;
				response[L"message"] = json::value::string(L"Move made");
				request.reply(status_codes::OK, response);
			}
			catch (const std::exception &e)
			{
				json::value response;
				response[L"error"] = json::value::string(U("Move made error"));
				request.reply(status_codes::InternalError, response);
			}
		}
		else
		{
			request.reply(status_codes::NotFound);
		}
	}
	else
	{
		request.reply(status_codes::MethodNotAllowed);
	}
}

void StrongholdAPI::initializeGame(int numPlayers)
{
	// Your game initialization code here
	// Example:
	for (int i = 0; i < numPlayers; i++)
	{
		char playerIdentifier = static_cast<char>('A' + i);
		Town *town = new Town(playerIdentifier);
		towns.push_back(town);
		Player *player = new Player(town);
		players.push_back(player);
	}
	Town::addAllTowns(towns);
	// Rest of your game initialization logic
	// ...
}

Player *StrongholdAPI::findPlayerByIdentifier(char identifier)
{
	for (auto player : players)
	{
		if (player->getIdentifier() == identifier)
		{
			return player;
		}
	}
	return nullptr;
}
