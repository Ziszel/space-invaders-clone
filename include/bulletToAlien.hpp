#include "enemy.hpp"
#include "bullet.hpp"
#include <vector>
#include <raylib-cpp.hpp>
#include "player.hpp"
#pragma once

void bulletToAlien(std::vector<Bullet> &bullets, std::vector<Enemy> &enemies, Player *player, Sound enemyExplosion);