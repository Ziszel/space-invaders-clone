#include "bulletToAlien.hpp"

// TODO: Once entity system is implemented, update this function to take entities
// as params
// Bounding Box Collision
void bulletToAlien(std::vector<Bullet> &bullets, std::vector<Enemy> &enemies, Player *player, Sound enemyExplosion)
{
    int collision = 0;
    int tempI;
    int tempJ;

    for (int i = 0; i < bullets.size(); i++)
    {
        for (int j = 0; j < enemies.size(); j++)
        {
            if (bullets[i].x <= enemies[j].x + enemies[j].w &&
                enemies[j].x <= bullets[i].x + bullets[i].w &&
                bullets[i].y <= enemies[j].y + enemies[j].h &&
                enemies[j].y <= bullets[i].y + bullets[i].h)
            {
                player->score += 20;
                PlaySound(enemyExplosion);
                bullets.erase(bullets.begin() + i);
                enemies.erase(enemies.begin() + j);
            }
        }
    }
}