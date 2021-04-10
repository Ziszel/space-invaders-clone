#include "../include/enemy.hpp"

Enemy::Enemy(float x)
{
    this->x = x;
}

Enemy::Enemy()
{
    this->x = 100;
}

void Enemy::Update(float dt, int screenWidth)
{
    Move(dt, screenWidth);
}

void Enemy::Move(float dt, int screenWidth)
{
    if (this->dir == left)
    {
        this->x -= this->speed * dt;
    }
    else if (this->dir == right)
    {
        this->x += this->speed * dt;
    }
}