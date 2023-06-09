﻿#ifndef TOWN
#define TOWN

#include <vector>
#include <memory>
#include "Structure.h"
#include "Troop.h"
#include "Lord.h"
#include "Exception.h"

class Market;    // forward deklaracija
class Structure; // forward deklaracija

class Town final
{
public:
    Town();
    Town(char team);
    Town(char team, int housing, double gold, int rations, int wood, int stone, Troop* lord);
    ~Town();

    bool operator>(const Town& other) const;
    bool operator==(const Town& town) const;

    // key metode
    static void addAllTowns(std::vector<Town*>& towns);
    static void wait();
    void const info();
    void const listTroops();
    void const listStructures();
    void buyFromMarket(std::string what, int amount);
    void sellToMarket(std::string what, int amount);
    void destroyStructure(Structure* structure);
    void buildStructure(Structure* structure, int amount);
    void buildHousing(int n = 1);
    bool sufficientRations();
    void trainVillager(int = 1);
    void trainTroop(Troop* troop, int amount);
    void raid(Town* town);
    void attackLord(Town* town);

    // getteri
    static std::vector<Town*>& getAllTowns();
    std::vector<Structure*> getStructures();
    std::vector<Troop*> getArmy();
    char getTeam();
    double getGold();
    int getRations();
    int getHousing();
    int getWood();
    int getStone();
    int getPopulation();
    int getActivePopulation();

    // setteri
    void setTeam(char team);
    void setGold(double gold);
    void setRations(int rations);
    void setHousing(int housing);
    void setWood(int wood);
    void setStone(int stone);
    void setPopulation(int population);
    void setActivePopulation(int activePopulation);

private:
    char team;
    std::vector<Troop*> army;           // vojska grada pohranjena
    std::vector<Structure*> structures; // sve građevine grada pohranjene
    static std::vector<Town*> allTowns;
    Troop* lord;
    double gold;
    int rations;
    int wood;
    int stone;
    int housing;
    int population = 0;
    int activePopulation = 0;
};

#endif