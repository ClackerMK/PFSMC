#ifndef MONSTER_H
#define MONSTER_H

#include "statistics.h"

class Monster
{
private:
    int         CR;
    Statistics  base;
public:
    Monster();

    // Setter
    void setCR(int cr);
    void setBaseStats(Statistics stats);

    // Getter
    int getCR();
    Statistics getBaseStats();
    Statistics getStats();
};

#endif // MONSTER_H
