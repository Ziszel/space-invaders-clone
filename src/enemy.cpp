#include "../include/enemy.hpp"

Enemy::Enemy(float x, Texture2D texture)
{
    this->x = x;
    this->texture = texture;
}

Enemy::Enemy()
{
    this->x = 100;
}

// Enemy has its own update loop. Simple, but scalable.
void Enemy::Update(float &movementTimer, float deltaTime, int screenWidth, std::vector<Enemy> &enemies)
{
    if (movementTimer <= 0)
    {
        // see if the front (left most enemy) has hit the side
        if (Enemy::dir == left && (enemies.front().x - enemies.front().speed * deltaTime) < 0)
        {
            Enemy::y += 40;
            Enemy::dir = right;
        }
        // see if the back (right most enemy) has hit the side
        else if (Enemy::dir == right && (enemies.back().x + enemies.front().speed * deltaTime) > screenWidth - enemies.front().w)
        {
            Enemy::y += 40;
            Enemy::dir = left;
        }
        else
        {
            for (auto &enemy : enemies)
            {
                enemy.Move(deltaTime, screenWidth);
            }
        }
        if (enemies.size() > 2)
        {
            movementTimer = enemies.size() * 1.2 + enemies.size() - 4;
        }
        else
        {
            movementTimer = 0.5;
        }
    }
    else
    {
        movementTimer--;
    }
}

// Move dependant on dir enum
void Enemy::Move(float deltaTime, int screenWidth)
{
    if (this->dir == left)
    {
        this->x -= this->speed * deltaTime;
    }
    else if (this->dir == right)
    {
        this->x += this->speed * deltaTime;
    }
}

// Enemy has its own draw loop. Simple, but scalable.
void Enemy::Draw()
{
    DrawTexture(this->texture, this->x, this->y, WHITE);
}