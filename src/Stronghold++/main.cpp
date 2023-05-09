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
int main()
{
	try
	{
		Town* town1 = new Town('A');
		Town* town2 = new Town('B');
		Town* town3 = new Town('C');
		std::vector<Town*> towns{ town1, town2, town3 };
		Town::addAllTowns(towns);

		town1->setGold(10000);
		town1->setWood(10000);
		town1->setRations(10000);
		town1->setStone(10000);
		town1->buildHousing(50);
		town1->trainVillager(40);
		town1->info();
		town1->buildStructure(new StoneQuarry(town1));

		std::cout << std::endl;
		town3->trainVillager(5);
		town3->setRations(0);
		town3->info();

		std::cout << std::endl;
		Town::wait();
		/*towns.pop_back();*/
		std::cout << std::endl;
		town1->info();
		std::cout << std::endl;

		town1->trainTroop(new Archer());
		town1->trainTroop(new Archer());
		town1->trainTroop(new Maceman());
		town1->trainTroop(new Pikeman());
		town1->trainTroop(new Archer());
		town1->trainTroop(new Archer());
		town1->trainTroop(new Maceman());
		town1->trainTroop(new Pikeman());
		town1->trainTroop(new Archer());
		town1->trainTroop(new Archer());
		town1->trainTroop(new Maceman());
		town1->trainTroop(new Pikeman());
		town1->listTroops();

		std::cout << std::endl;
		town2->listTroops();

		town1->raid(town2);
		std::cout << std::endl;
		town1->listTroops();
		std::cout << std::endl;
		town1->attackLord(town2);

		std::cout << std::endl;
		town1->buildStructure(new Market(town1));
		town1->buildStructure(new StoneQuarry(town1));
		town1->buildStructure(new WoodCamp(town1));
		town1->buildStructure(new WheatFarm(town1));
		town1->listStructures();
		std::cout << std::endl;
		town1->info();
		town1->buyFromMarket("wood", 50);
		std::cout << std::endl;
		town1->info();
	}
	catch (InputValidation& e)
	{
		std::cout << "InputValidation exception caught: " << e.what() << std::endl;
	}
	catch (InvalidReference& e)
	{
		std::cout << "InvalidReference exception caught: " << e.what() << std::endl;
	}
}
