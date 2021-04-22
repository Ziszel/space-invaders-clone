#include "spawnEnemies.hpp"

std::vector<Enemy> spawnEnemies()
{
    std::vector<Enemy> enemies;
    for (int i = 0; i < 10; i++)
    {
        Enemy enemy(80 + (80 * i));
        enemies.push_back(enemy);
    }
    return enemies;
}