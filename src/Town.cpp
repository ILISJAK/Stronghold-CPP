#include <iostream>
#include <algorithm>
#include "Town.h"
#include "Archer.h"
#include "Knight.h"
#include "Maceman.h"
#include "Pikeman.h"
#include "Market.h"

#define TEAM 'A'
#define HOUSING 10
#define GOLD 1000
#define RATIONS 350
#define WOOD 200
#define STONE 200

std::vector<Town *> Town::allTowns;

Town::Town() : Town::Town(TEAM, HOUSING, GOLD, RATIONS, WOOD, STONE, new Lord()) {}

Town::Town(char team) : Town::Town(team, HOUSING, GOLD, RATIONS, WOOD, STONE, new Lord()) {}

Town::Town(char team, int housing, double gold, int rations, int wood, int stone, Troop *lord)
{
	std::cout << "A new town " << team << " has been created!" << std::endl;
	setTeam(team);
	setHousing(housing);
	setGold(gold);
	setRations(rations);
	setWood(wood);
	setStone(stone);
	this->lord = lord;
}
Town::~Town()
{
	if (lord != nullptr)
	{
		delete lord;
	}
	for (auto &troop : army)
	{
		delete troop;
	}
	army.clear();
	for (auto &structure : structures)
	{
		delete structure;
	}
	structures.clear();
	std::cout << "Town " << getTeam() << " destroyed." << std::endl;
	// std::cout << "TEST6" << std::endl;
}

bool Town::operator>(const Town &town) const
{
	return gold > town.gold;
}

bool Town::operator==(const Town &town) const
{
	return gold > town.gold;
}

// key metode

void Town::addAllTowns(std::vector<Town *> &towns)
{
	for (auto town : towns)
	{
		allTowns.push_back(town);
	}
}

void const Town::info()
{
	std::cout << "TEAM " << team << " TOWN: " << std::endl;
	std::cout << "- Population: " << population << "/" << housing << ", " << population - activePopulation << " unassigned" << std::endl;
	std::cout << "- Army size: " << army.size() << std::endl;
	std::cout << "- Structures built: " << structures.size() << std::endl;
	std::cout << "- Gold: " << gold << std::endl;
	std::cout << "- Stone: " << stone << std::endl;
	std::cout << "- Wood: " << wood << std::endl;
	std::cout << "- Rations: " << rations << std::endl;
}

void Town::wait()
{
	std::cout << "Some time has passed..." << std::endl;
	for (auto it = allTowns.begin(); it != allTowns.end();)
	{
		if (!(*it)->sufficientRations()) // dereferenciranje iteratora
		{
			std::cout << (*it)->getTeam() << " suffers from famine." << std::endl;
			delete *it;
			it = allTowns.erase(it); // samo allTowns.erase(it) = memory leak
			continue;
		}
		else
		{
			for (auto &structure : (*it)->getStructures())
			{
				structure->work();
			}
			(*it)->setRations((*it)->getRations() - (*it)->getPopulation());
			++it;
		}
	}
}

// metode vezane uz strukture i ekonomiju

void const Town::listStructures()
{
	std::cout << "Town " << team << " structures: " << structures.size() << std::endl;
	int i = 1;
	for (auto &it : structures)
	{
		if (it != structures.back())
		{
			std::cout << i << ". " << it->getType() << ", ";
			i++;
		}
		else
		{
			std::cout << i << ". " << it->getType() << std::endl;
			;
		}
	}
}
void Town::buildHousing(int n) // placeholder exception handle
{
	int counter = 0;
	for (int i = 0; i < n && getWood() - 10 >= 0; i++)
	{
		setWood(getWood() - 10);
		housing++;
		counter++;
	}
	if (counter == 0)
	{
		std::cout << "Insufficient wood." << std::endl;
		return;
	}
	else
	{
		std::cout << "Town " << getTeam() << " has built " << counter << " houses." << std::endl;
	}
}
void Town::trainVillager(int n)
{
	int counter = 0;
	for (int i = 0; i < n && ((getRations() - 10 >= 0) && (population + 1) <= housing); i++)
	{
		setRations(getRations() - 10);
		population++;
		counter++;
	}
	if (counter == 0)
	{
		std::cout << "Insufficient housing or rations." << std::endl;
		return;
	}
	else
	{
		std::cout << "Town " << getTeam() << " has trained " << counter << " villagers." << std::endl;
	}
}

void Town::buildStructure(Structure *structure, int amount)
{
	int counter = 0;
	for (int i = 0; i < amount && structure && gold >= structure->getGoldCost() && wood >= structure->getWoodCost() && stone >= structure->getStoneCost() && (activePopulation + 2) <= population; i++)
	{
		setActivePopulation(getActivePopulation() + 2);
		setGold(getGold() - structure->getGoldCost());
		setWood(getWood() - structure->getWoodCost());
		setStone(getStone() - structure->getStoneCost());
		structures.push_back(structure);
		counter++;
	}
	if (counter == 0)
	{
		std::cout << "Unable to build " << structure->getType() << " or insufficient resources." << std::endl;
	}
	else
	{
		std::cout << "Town " << getTeam() << " has built " << counter << " " << structure->getType() << "." << std::endl;
	}
}

void Town::destroyStructure(Structure *structure)
{
	if (structure)
	{
		auto it = std::find(structures.begin(), structures.end(), structure);
		if (it != structures.end())
		{
			setActivePopulation(getActivePopulation() - 2);
			setGold(getGold() + structure->getGoldCost() / 2);
			setWood(getWood() + structure->getWoodCost() / 2);
			setStone(getStone() + structure->getStoneCost() / 2);
			structures.erase(it);
			std::cout << "A structure has been destroyed in " << team << " town." << std::endl;
		}
	}
}
void Town::buyFromMarket(std::string what, int amount)
{
	for (auto it = structures.begin(); it != structures.end(); ++it)
	{
		if ((*it)->getType() == "Market")
		{
			Market *market = dynamic_cast<Market *>(*it);
			if (market)
			{
				std::cout << "Market: ";
				if (!market->buy(what, amount)) // ako pronadeni market nema dovoljno resursa, trazimo sljedeci
				{
					continue;
				}
				else
				{
					return;
				}
			}
		}
	}
	std::cout << "No suitable market in town." << std::endl;
}
void Town::sellToMarket(std::string what, int amount)
{
	for (auto it = structures.begin(); it != structures.end(); ++it)
	{
		if ((*it)->getType() == "Market")
		{
			Market *market = dynamic_cast<Market *>(*it);
			if (market)
			{
				std::cout << "Market: ";
				if (!market->sell(what, amount)) // ako pronadeni market nema dovoljno resursa, trazimo sljedeci
				{
					continue;
				}
				else
				{
					return;
				}
			}
		}
	}
	std::cout << "No suitable market in town." << std::endl;
}
bool Town::sufficientRations()
{
	return (rations >= population);
}

// metode vezane uz vojsku

void const Town::listTroops() // treba smisliti nacin za identifikaciju troopova - smisljeno
{
	std::cout << "Town " << team << " army size: " << army.size() << std::endl;
	for (auto &it : army)
	{
		if (it != army.back())
			it->info();
		else
		{
			it->info();
			std::cout << std::endl;
		}
	}
}

// void Town::trainTroop(Troop* troop)
// {
// 	if (troop && gold >= troop->getCost() && (activePopulation + 1) <= population)
// 	{
// 		setGold(getGold() - troop->getCost());
// 		army.push_back(troop);
// 		setActivePopulation(getActivePopulation() + 1);
// 	}
// 	else
// 	{
// 		std::cout << "No villagers to turn into troops." << std::endl;
// 	}
// }

void Town::trainTroop(Troop *troop, int amount) // updated ver
{
	int counter = 0;
	for (int i = 0; i < amount && troop && gold >= troop->getCost() && (activePopulation + 1) <= population; i++)
	{
		setGold(getGold() - troop->getCost());
		army.push_back(troop);
		setActivePopulation(getActivePopulation() + 1);
		counter++;
	}
	if (counter == 0)
	{
		std::cout << "No villagers to turn into troops or insufficient gold." << std::endl;
	}
	else
	{
		std::cout << "Town " << getTeam() << " has trained " << counter << " " << troop->getIdentifier() << "." << std::endl;
	}
}

void Town::raid(Town *town)
{
	std::cout << "DEBUG: Town " << getTeam() << " is raiding " << town->getTeam() << "!" << std::endl;
	int allyCasualties = 0;
	int enemyCasualties = 0;
	bool hasAttackPriority = true;

	while (!this->army.empty() && !town->army.empty())
	{
		Troop *attacker;
		Troop *target;

		if (hasAttackPriority)
		{
			attacker = this->army.front();
			target = town->army.front();
		}
		else
		{
			attacker = town->army.front();
			target = this->army.front();
		}

		std::cout << "DEBUG: Attacker: " << attacker->getIdentifier() << ", Health: " << attacker->getHealth() << std::endl;
		std::cout << "DEBUG: Target: " << target->getIdentifier() << ", Health: " << target->getHealth() << std::endl;

		attacker->attack(target);

		std::cout << "DEBUG: Attacker health: " << attacker->getHealth() << std::endl;
		std::cout << "DEBUG: Target health: " << target->getHealth() << std::endl;

		if (target->getHealth() <= 0)
		{
			if (hasAttackPriority)
			{
				enemyCasualties++;
				delete target;
				town->army.erase(town->army.begin());
				town->setActivePopulation(town->getActivePopulation() - 1);
				town->setPopulation(town->getPopulation() - 1);
				std::cout << "DEBUG: Enemy troop " << target->getIdentifier() << " destroyed." << std::endl;
			}
			else
			{
				allyCasualties++;
				delete target;
				this->army.erase(this->army.begin());
				setActivePopulation(getActivePopulation() - 1);
				setPopulation(getPopulation() - 1);
				std::cout << "DEBUG: Ally troop " << target->getIdentifier() << " destroyed." << std::endl;
			}
		}

		if (attacker->getHealth() <= 0)
		{
			if (hasAttackPriority)
			{
				allyCasualties++;
				delete attacker;
				this->army.erase(this->army.begin());
				setActivePopulation(getActivePopulation() - 1);
				setPopulation(getPopulation() - 1);
				std::cout << "DEBUG: Ally troop " << attacker->getIdentifier() << " destroyed." << std::endl;
			}
			else
			{
				enemyCasualties++;
				delete attacker;
				town->army.erase(town->army.begin());
				town->setActivePopulation(town->getActivePopulation() - 1);
				town->setPopulation(town->getPopulation() - 1);
				std::cout << "DEBUG: Enemy troop " << attacker->getIdentifier() << " destroyed." << std::endl;
			}
		}

		hasAttackPriority = !hasAttackPriority;
	}

	if (enemyCasualties > 0)
	{
		std::cout << "DEBUG: Resources plundered: ";
		int plunderedGold = enemyCasualties * 10;
		int plunderedRations = enemyCasualties * 10;
		int plunderedStone = enemyCasualties * 10;
		int plunderedWood = enemyCasualties * 10;

		this->gold += plunderedGold;
		this->rations += plunderedRations;
		this->stone += plunderedStone;
		this->wood += plunderedWood;

		std::cout << "Gold: " << plunderedGold << ", Rations: " << plunderedRations << ", Stone: " << plunderedStone << ", Wood: " << plunderedWood << std::endl;
	}

	std::cout << "DEBUG: Town " << getTeam() << " casualties: " << allyCasualties << std::endl;
	std::cout << "DEBUG: Town " << town->getTeam() << " casualties: " << enemyCasualties << std::endl;
}

void Town::attackLord(Town *targetTown)
{
	if (targetTown == nullptr || targetTown->lord == nullptr)
	{
		std::cerr << "Invalid target town or lord." << std::endl;
		return;
	}

	std::cout << "DEBUG: Starting attackLord function" << std::endl;
	std::cout << "DEBUG: Attacking town " << targetTown->getTeam() << std::endl;

	std::cout << "DEBUG: Number of attackers: " << this->army.size() << std::endl;

	auto it = this->army.begin(); // Iterator to track the current attacker

	while (it != this->army.end() && targetTown->lord->getHealth() > 0)
	{
		std::cout << "DEBUG: Inside attack loop" << std::endl;

		// Get the current attacker
		Troop *attacker = *it;

		// Perform the attack
		attacker->attack(targetTown->lord);

		std::cout << "DEBUG: Attacker health: " << attacker->getHealth() << std::endl;
		std::cout << "DEBUG: Lord health: " << targetTown->lord->getHealth() << std::endl;

		// Check if the attacker has been killed
		if (attacker->getHealth() <= 0)
		{
			std::cout << "DEBUG: Attacker has been killed" << std::endl;

			// Erase the attacker from the army and delete it
			it = this->army.erase(it);
			delete attacker;

			// Update the town's population
			setActivePopulation(getActivePopulation() - 1);
			setPopulation(getPopulation() - 1);
		}
		else
		{
			++it; // Move the iterator to the next attacker
		}

		// Check if the lord has been killed
		if (targetTown->lord->getHealth() <= 0)
		{
			std::cout << "DEBUG: Lord has been killed" << std::endl;

			// Plunder the resources and assimilate the population
			this->gold += targetTown->gold;
			this->rations += targetTown->rations;
			this->stone += targetTown->stone;
			this->wood += targetTown->wood;
			this->population += targetTown->population;

			// Destroy the target lord
			delete targetTown->lord;
			targetTown->lord = nullptr;
		}
	}

	if (this->army.empty())
	{
		std::cout << "DEBUG: No more attackers remaining" << std::endl;
	}
	else if (targetTown->lord->getHealth() <= 0)
	{
		std::cout << "DEBUG: Lord has been defeated" << std::endl;
	}

	std::cout << "DEBUG: End of attackLord function" << std::endl;
}

// getteri
std::vector<Town *> &Town::getAllTowns()
{
	return allTowns;
}
std::vector<Structure *> Town::getStructures()
{
	return structures;
}
std::vector<Troop *> Town::getArmy()
{
	return army;
}
char Town::getTeam()
{
	return team;
}
double Town::getGold()
{
	return gold;
}
int Town::getRations()
{
	return rations;
}
int Town::getHousing()
{
	return housing;
}
int Town::getWood()
{
	return wood;
}
int Town::getStone()
{
	return stone;
}
int Town::getPopulation()
{
	return population;
}
int Town::getActivePopulation()
{
	return activePopulation;
}

// setteri
void Town::setTeam(char team)
{
	this->team = team;
}
void Town::setGold(double gold)
{
	if (gold >= 0)
		this->gold = gold;
	else
		throw InputValidation("Gold value can't be negative.");
}
void Town::setRations(int rations)
{
	if (rations >= 0)
		this->rations = rations;
	else
		throw InputValidation("Rations value can't be negative.");
}
void Town::setHousing(int housing)
{
	if (housing >= 0)
		this->housing = housing;
	else
		throw InputValidation("Housing value can't be negative.");
}
void Town::setWood(int wood)
{
	if (wood >= 0)
		this->wood = wood;
	else
		throw InputValidation("Wood value can't be negative.");
}
void Town::setStone(int stone)
{
	if (stone >= 0)
		this->stone = stone;
	else
		throw InputValidation("Stone value can't be negative.");
}
void Town::setPopulation(int population)
{
	if (population >= 0 && population <= housing)
		this->population = population;
	else
	{
		this->population = housing;
		throw InputValidation("Population value can't be negative.");
	}
}
void Town::setActivePopulation(int activePopulation)
{
	if (activePopulation >= 0 && activePopulation <= population)
		this->activePopulation = activePopulation;
	else
		throw InputValidation("Active population value can't be negative or greater than population.");
}