#include "Player.h"
#include <vector>

class TurnManager
{
public:
    TurnManager(const std::vector<Player *> &players);

    Player *getCurrentPlayer();

    void nextTurn();

    int getTurnNumber() const;

    void reset();

private:
    std::vector<Player *> players;
    size_t currentPlayerIndex;
    int turnNumber;
};
