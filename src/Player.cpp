#include "Player.h"

Player::Player(Town* town) : identifier(town->getTeam()), town(town) {}

char Player::getIdentifier() const
{
    return identifier;
}

Town* Player::getTown() const
{
    return town;
}
