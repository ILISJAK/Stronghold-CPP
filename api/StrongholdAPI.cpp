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
			try
			{
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
			try
			{
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
				else if (action == L"build-structure")
				{
					std::wstring structureType = requestBody[L"structure"].as_string();
					// Create an instance of the corresponding structure based on the type
					Structure *structure = nullptr;
					if (structureType == L"WoodCamp")
					{
						structure = new WoodCamp(town);
					}
					else if (structureType == L"StoneQuarry")
					{
						structure = new StoneQuarry(town);
					}
					else if (structureType == L"WheatFarm")
					{
						structure = new WheatFarm(town);
					}
					else if (structureType == L"Market")
					{
						structure = new Market(town);
					}

					if (structure)
					{
						int amount = requestBody[L"amount"].as_integer();
						// Call the buildStructure method on the town, passing the structure and amount
						town->buildStructure(structure, amount);
					}
					else
					{
						std::cout << "Invalid structure type." << std::endl;
					}
				}

				else if (action == L"buy-from-market")
				{
					std::wstring resource = requestBody[L"resource"].as_string();
					int amount = requestBody[L"amount"].as_integer();
					// Convert the resource from std::wstring to std::string
					std::string resourceStr(resource.begin(), resource.end());
					// Call the buyFromMarket method on the town, passing the resource and amount
					town->buyFromMarket(resourceStr, amount);
				}
				else if (action == L"sell-to-market")
				{
					std::wstring resource = requestBody[L"resource"].as_string();
					int amount = requestBody[L"amount"].as_integer();
					// Convert the resource from std::wstring to std::string
					std::string resourceStr(resource.begin(), resource.end());
					// Call the sellToMarket method on the town, passing the resource and amount
					town->sellToMarket(resourceStr, amount);
				}
				else if (action == L"train-troop")
				{
					std::wstring troopType = requestBody[L"troop"].as_string();
					int amount = requestBody[L"amount"].as_integer();
					// Create an instance of the corresponding troop based on the type
					Troop *troop = nullptr;
					if (troopType == L"Archer")
					{
						troop = new Archer();
					}
					else if (troopType == L"Knight")
					{
						troop = new Knight();
					}
					else if (troopType == L"Maceman")
					{
						troop = new Maceman();
					}
					else if (troopType == L"Pikeman")
					{
						troop = new Pikeman();
					}

					if (troop)
						try
						{
							town->trainTroop(troop, amount);
							json::value response;
							response[L"message"] = json::value::string(L"Move made");
							request.reply(status_codes::OK, response);
						}
						catch (const std::exception &e)
						{
							std::cout << "Error: " << e.what() << std::endl;
							json::value response;
							response[L"error"] = json::value::string(U("Error during troop training"));
							request.reply(status_codes::InternalError, response);
						}
					else
					{
						std::cout << "Invalid troop type." << std::endl;
					}
				}
				else if (action == L"attack-lord")
				{
					try
					{
						// Retrieve the target player identifier from the request body
						char targetPlayerIdentifier = requestBody[L"targetPlayerIdentifier"].as_string()[0];
						// Find the target player by identifier
						Player *targetPlayer = findPlayerByIdentifier(targetPlayerIdentifier);
						if (!targetPlayer)
						{
							throw std::runtime_error("Target player not found");
						}
						// Get the target town of the target player
						Town *targetTown = targetPlayer->getTown();

						// Perform the attack action on the target town
						town->attackLord(targetTown);
					}
					catch (const std::exception &e)
					{
						std::cerr << "An error occurred during the attack-lord action: " << e.what() << std::endl;
						throw;
					}
				}
				else if (action == L"raid")
				{
					// Retrieve the target player identifier from the request body
					char targetPlayerIdentifier = requestBody[L"targetPlayerIdentifier"].as_string()[0];
					// Find the target player by identifier
					Player *targetPlayer = findPlayerByIdentifier(targetPlayerIdentifier);
					if (!targetPlayer)
					{
						throw std::runtime_error("Target player not found");
					}
					// Get the target town of the target player
					Town *targetTown = targetPlayer->getTown();

					// Perform the raid action on the target town
					town->raid(targetTown);
				}
				else
				{
					std::cout << "Invalid action." << std::endl;
				}

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
