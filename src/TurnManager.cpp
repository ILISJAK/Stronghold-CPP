#include "TurnManager.h"

TurnManager::TurnManager(const std::vector<Player *> &players)
    : players(players), currentPlayerIndex(0), turnNumber(1)
{
    std::cout << "Current turn: " << getTurnNumber() << " Player " << getCurrentPlayer()->getIdentifier() << std::endl;
}

Player *TurnManager::getCurrentPlayer()
{
    return players[currentPlayerIndex];
}

void TurnManager::nextTurn()
{
    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    if (currentPlayerIndex == 0)
    {
        turnNumber++;
    }
    std::cout << "Current turn: " << getTurnNumber() << " Player " << getCurrentPlayer()->getIdentifier() << std::endl;
}

int TurnManager::getTurnNumber() const
{
    return turnNumber;
}

void TurnManager::reset()
{
    currentPlayerIndex = 0;
    turnNumber = 1;
}
