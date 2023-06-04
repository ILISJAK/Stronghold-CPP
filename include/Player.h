#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Town.h"

class Player
{
private:
    char identifier;
    Town* town;

public:
    Player(Town* town);

    char getIdentifier() const;
    Town* getTown() const;
};

#endif // PLAYER_HPP
