#include "StrongholdAPI.h"
#include <iostream>

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

				// Check if the game is already initialized
				if (isGameInitialized())
				{
					std::cout << "Request to restart game. Restarting..." << std::endl;
					deleteGameInstance();
				}

				// Initialize the new game and create towns for each player
				std::cout << "Game started: " << std::endl;
				initializeGame(numPlayers);
				json::value response;
				response[L"message"] = json::value::string(L"Game started");
				request.reply(status_codes::OK, response);
			}
			catch (const std::exception &e)
			{
				json::value response;
				response[L"error"] = json::value::string(utility::conversions::to_utf16string(e.what()));
				request.reply(status_codes::InternalError, response);
			}
		}
		else if (request.request_uri().path() == L"/load-game")
		{
			try
			{
				json::value requestBody = request.extract_json().get();

				if (isGameInitialized())
				{
					std::cout << "Request to restart game. Restarting..." << std::endl;
					deleteGameInstance();
				}

				// Pass the saved data to the initializeGameFromData method
				initializeGameFromData(requestBody);

				json::value response;
				response[L"message"] = json::value::string(L"Game loaded");
				request.reply(status_codes::OK, response);
			}
			catch (const std::exception &e)
			{
				json::value response;
				response[L"error"] = json::value::string(utility::conversions::to_utf16string(e.what()));
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

				if (turnManager->getCurrentPlayer()->getIdentifier() == playerIdentifier)
				{
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
								response[L"currentTurn"] = json::value::number(turnManager->getTurnNumber());
								char currentPlayerIdentifier = turnManager->getCurrentPlayer()->getIdentifier();
								std::wstring currentPlayerIdentifierWStr(1, currentPlayerIdentifier);
								response[L"currentPlayer"] = json::value::string(currentPlayerIdentifierWStr);
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

					turnManager->nextTurn();

					json::value response;
					response[L"message"] = json::value::string(L"Move made");
					response[L"currentTurn"] = json::value::number(turnManager->getTurnNumber());
					char currentPlayerIdentifier = turnManager->getCurrentPlayer()->getIdentifier();
					std::wstring currentPlayerIdentifierWStr(1, currentPlayerIdentifier);
					response[L"currentPlayer"] = json::value::string(currentPlayerIdentifierWStr);

					request.reply(status_codes::OK, response);
				}
				else
				{
					json::value response;
					response[L"error"] = json::value::string(U("It's not your turn."));
					request.reply(status_codes::BadRequest, response);
				}
			}
			catch (const std::exception &e)
			{
				json::value response;
				response[L"error"] = json::value::string(U("Move made error"));
				request.reply(status_codes::InternalError, response);
			}
		}
		else if (request.request_uri().path() == L"/get-property")
		{
			try
			{
				// Parse the JSON request body
				auto jsonBody = request.extract_json().get();

				// Retrieve the player identifier, entity type, and property from the request body
				char playerIdentifier = utility::conversions::to_utf8string(jsonBody[L"playerIdentifier"].as_string())[0];
				std::wstring entityType = jsonBody[L"entityType"].as_string();
				std::wstring property = jsonBody[L"property"].as_string();

				// Find the player by identifier
				Player *player = findPlayerByIdentifier(playerIdentifier);
				if (!player)
				{
					throw std::runtime_error("Player not found");
				}

				// Get the property value for the specified entity type and property
				std::string propertyValue;
				if (entityType == L"town")
				{
					propertyValue = getTownProperty(player, property);
				}
				else if (entityType == L"structure")
				{
					// Assuming structures are stored in the Town class
					propertyValue = getStructureProperty(player->getTown(), property);
				}
				else
				{
					throw std::runtime_error("Invalid entity type");
				}

				// Create the JSON response
				json::value response;
				response[utility::conversions::to_utf16string(property)] = json::value::string(utility::conversions::to_utf16string(propertyValue));

				// Send the JSON response
				request.reply(status_codes::OK, response);
			}
			catch (const std::exception &e)
			{
				json::value response;
				response[L"error"] = json::value::string(U("Error retrieving property"));
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
	for (int i = 0; i < numPlayers; i++)
	{
		char playerIdentifier = static_cast<char>('A' + i);
		Town *town = new Town(playerIdentifier);
		towns.push_back(town);
		Player *player = new Player(town);
		players.push_back(player);
	}
	Town::addAllTowns(towns);
	turnManager = new TurnManager(players);
}

struct TownData
{
	char team;
	double gold;
	int rations;
	int housing;
	int wood;
	int stone;
	int population;
	int activePopulation;
};

std::vector<TownData> deserializeTowns(const std::string &serializedTowns)
{
	try
	{
		json::value townsJson = json::value::parse(utility::conversions::to_utf16string(serializedTowns));

		std::vector<TownData> townDataList;
		for (const auto &townVal : townsJson.as_array())
		{
			TownData data;

			std::wcout << L"Parsing townVal: " << townVal.serialize() << std::endl;
			data.team = townVal.at(L"team").as_string()[0];
			std::cout << "Parsed team: " << data.team << std::endl;
			data.gold = townVal.at(L"gold").as_double();
			std::cout << "Parsed gold: " << data.gold << std::endl;
			data.rations = townVal.at(L"rations").as_integer();
			std::cout << "Parsed rations: " << data.rations << std::endl;
			data.housing = townVal.at(L"housing").as_integer();
			std::cout << "Parsed housing: " << data.housing << std::endl;
			data.wood = townVal.at(L"wood").as_integer();
			std::cout << "Parsed wood: " << data.wood << std::endl;
			data.stone = townVal.at(L"stone").as_integer();
			std::cout << "Parsed stone: " << data.stone << std::endl;
			data.population = townVal.at(L"population").as_integer();
			std::cout << "Parsed population: " << data.population << std::endl;
			data.activePopulation = townVal.at(L"activePopulation").as_integer();
			std::cout << "Parsed activePopulation: " << data.activePopulation << std::endl;

			townDataList.push_back(data);
		}

		return townDataList;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error in deserializeTowns: " << e.what() << std::endl;
		return std::vector<TownData>(); // Return an empty vector
	}
}

void StrongholdAPI::initializeGameFromData(const json::value &savedData)
{
	try
	{
		std::string serializedTowns = utility::conversions::to_utf8string(savedData.at(L"serializedTowns").as_string());

		std::cout << "Serialized Towns: " << serializedTowns << std::endl;

		std::vector<TownData> townDataList = deserializeTowns(serializedTowns);

		this->towns.clear();
		this->players.clear();

		for (auto &data : townDataList)
		{
			std::cout << "Creating town with team: " << data.team << std::endl;

			Town *town = new Town(data.team);

			town->setGold(data.gold);
			town->setRations(data.rations);
			town->setWood(data.wood);
			town->setStone(data.stone);
			town->setPopulation(data.population);
			town->setActivePopulation(data.activePopulation);
			town->setHousing(data.housing);

			this->towns.push_back(town);

			Player *player = new Player(town);
			this->players.push_back(player);

			std::cout << "Town created for team " << data.team << std::endl;
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error in initializeGameFromData: " << e.what() << std::endl;
	}
}

void StrongholdAPI::deleteGameInstance()
{
	// Delete dynamically allocated Town objects
	for (Town *town : towns)
	{
		delete town;
	}
	// Clear the towns vector
	towns.clear();

	// Delete dynamically allocated Player objects
	for (Player *player : players)
	{
		delete player;
	}
	// Clear the players vector
	players.clear();

	delete turnManager;
	turnManager = nullptr;
}

bool StrongholdAPI::isGameInitialized()
{
	return !players.empty();
}

Player *StrongholdAPI::findPlayerByIdentifier(char identifier)
{
	for (auto player : players)
	{
		if (player->getIdentifier() == identifier)
		{
			if (player->getTown() == nullptr)
			{
				return nullptr;
			}
			else
				return player;
		}
	}
	return nullptr;
}
void StrongholdAPI::removePlayer(Player *player)
{
	// Find the player in the vector
	auto it = std::find(players.begin(), players.end(), player);
	if (it != players.end())
	{
		delete *it;
		players.erase(it);
	}
}

std::string StrongholdAPI::getTownProperty(Player *player, const std::wstring &property)
{
	// Retrieve the town from the player
	Town *town = player->getTown();

	if (property == L"team")
	{
		return std::string(1, town->getTeam());
	}
	else if (property == L"gold")
	{
		return std::to_string(town->getGold());
	}
	else if (property == L"rations")
	{
		return std::to_string(town->getRations());
	}
	else if (property == L"housing")
	{
		return std::to_string(town->getHousing());
	}
	else if (property == L"wood")
	{
		return std::to_string(town->getWood());
	}
	else if (property == L"stone")
	{
		return std::to_string(town->getStone());
	}
	else if (property == L"population")
	{
		return std::to_string(town->getPopulation());
	}
	else if (property == L"activePopulation")
	{
		return std::to_string(town->getActivePopulation());
	}
	else
	{
		throw std::runtime_error("Invalid town property");
	}
}

std::string StrongholdAPI::getStructureProperty(Town *town, const std::wstring &property)
{
	// Retrieve the structures from the town
	std::vector<Structure *> structures = town->getStructures();

	for (const auto &structure : structures)
	{
		if (property == L"type")
		{
			return utility::conversions::to_utf8string(structure->getType());
		}
		else if (property == L"goldCost")
		{
			return std::to_string(structure->getGoldCost());
		}
		else if (property == L"woodCost")
		{
			return std::to_string(structure->getWoodCost());
		}
		else if (property == L"stoneCost")
		{
			return std::to_string(structure->getStoneCost());
		}
		else if (property == L"parentTown")
		{
			return std::string(1, structure->getParentTown()->getTeam());
		}
	}

	throw std::runtime_error("Invalid structure property");
}
std::string StrongholdAPI::getTroopProperty(Town *town, const std::wstring &property)
{
	// Retrieve the troops from the town
	std::vector<Troop *> troops = town->getArmy();

	for (const auto &troop : troops)
	{
		if (property == L"health")
		{
			return std::to_string(troop->getHealth());
		}
		else if (property == L"damage")
		{
			return std::to_string(troop->getDamage());
		}
		else if (property == L"defense")
		{
			return std::to_string(troop->getDefense());
		}
		else if (property == L"agility")
		{
			return std::to_string(troop->getAgility());
		}
		else if (property == L"cost")
		{
			return std::to_string(troop->getCost());
		}
		else if (property == L"mounted")
		{
			return troop->getMounted() ? "true" : "false";
		}
		else if (property == L"ranged")
		{
			return troop->getRanged() ? "true" : "false";
		}
		else if (property == L"identifier")
		{
			return utility::conversions::to_utf8string(troop->getIdentifier());
		}
	}

	throw std::runtime_error("Invalid troop property");
}
