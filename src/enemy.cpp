#include "../include/enemy.hpp"

Enemy::Enemy(float x)
{
    this->x = x;
}

Enemy::Enemy()
{
    this->x = 100;
}

// Enemy has its own update loop. Simple, but scalable.
void Enemy::Update(float dt, int screenWidth)
{
    Move(dt, screenWidth);
}

// Move dependant on dir enum
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

// Enemy has its own draw loop. Simple, but scalable.
void Enemy::Draw()
{
    DrawTexture(this->texture, this->x, this->y, WHITE);
}