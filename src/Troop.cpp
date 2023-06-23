#include "Troop.h"
#include <iostream>

#define IDENTIFIER "abc"
#define MAXHEALTH 1
#define DAMAGE 1
#define DEFENSE 1
#define AGILITY 1
#define COST 1
#define MOUNTED false
#define RANGED false

Troop::Troop() : Troop::Troop(IDENTIFIER, MAXHEALTH, DAMAGE, DEFENSE, AGILITY, COST, MOUNTED, RANGED) {}
Troop::Troop(std::string identifier, double maxHealth, double damage, double defense, double agility, int cost, bool mounted, bool ranged)
{
	this->troopIdentifier = identifier;
	this->maxHealth = maxHealth;
	this->health = maxHealth;
	this->damage = damage;
	this->defense = defense;
	this->agility = agility;
	this->cost = cost;
	this->mounted = mounted;
	this->ranged = ranged;
}
Troop::~Troop() {}

Troop::Troop(const Troop &troop) {}

void Troop::attack(Troop *target)
{
	if (target == nullptr)
	{
		std::cout << "target is nullptr" << std::endl;
		return;
	}

	// Calculate damage dealt by the attacker
	int attackerDamage = this->damage - target->defense;
	if (attackerDamage < 0)
	{
		attackerDamage = 0; // Ensure non-negative damage
	}

	// Calculate dodge chance based on the agility difference
	double dodgeChance = (target->agility - this->agility) * 0.01;
	if (dodgeChance < 0)
	{
		dodgeChance = 0; // Ensure non-negative dodge chance
	}

	// Apply dodge chance to the damage dealt by the attacker
	double randomValue = static_cast<double>(rand()) / RAND_MAX; // Generate random value between 0 and 1
	if (randomValue <= dodgeChance)
	{
		attackerDamage = 0; // Attacker's damage is dodged
		std::cout << "Target dodged the attack!" << std::endl;
	}

	// Reduce target's health by the calculated damage
	target->health -= attackerDamage;
}

void const Troop::info()
{
	std::cout << troopIdentifier << " ";
	std::cout << "HP: " << health << "/" << maxHealth << std::endl;
}
void const Troop::stats()
{
	info();
	std::cout << "DMG: " << damage << std::endl;
	std::cout << "DEF: " << defense << std::endl;
	std::cout << "AGI: " << agility << std::endl;
	std::cout << "MOUNTED: " << mounted << std::endl;
	std::cout << "RANGED: " << ranged << std::endl;
}

// getteri
double Troop::getHealth()
{
	return health;
}
double Troop::getDamage()
{
	return damage;
}
double Troop::getDefense()
{
	return defense;
}
double Troop::getAgility()
{
	return agility;
}
int Troop::getCost()
{
	return cost;
}
bool Troop::getMounted()
{
	return mounted;
}
bool Troop::getRanged()
{
	return ranged;
}

std::string Troop::getIdentifier()
{
	return troopIdentifier;
}

// setteri
void Troop::setHealth(double health)
{
	this->health = health;
}
void Troop::setDamage(double damage)
{
	this->damage = damage;
}
void Troop::setDefense(double defense)
{
	this->defense = defense;
}
void Troop::setAgility(double defense)
{
	this->agility = agility;
}
void Troop::setCost(int cost)
{
	this->cost = cost;
}
void Troop::setMounted(bool mounted)
{
	this->mounted = mounted;
}
void Troop::setRanged(bool ranged)
{
	this->ranged = ranged;
}