#include "spawnEnemies.hpp"

// create 10 enemy objects (with custom spacing) and return them as a vector.
std::vector<Enemy> spawnEnemies(Texture2D texture)
{
    std::vector<Enemy> enemies;
    for (int i = 0; i < 10; i++)
    {
        Enemy enemy(80 + (80 * i), texture);
        enemies.push_back(enemy);
    }
    return enemies;
}