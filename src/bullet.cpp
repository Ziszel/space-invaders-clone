#include "../include/bullet.hpp"

Bullet::Bullet(float x, float y)
{
    this->x = x;
    this->y = y;
}

void Bullet::Draw()
{
    DrawRectangle(this->x, this->y, this->w, this->h, DARKBLUE);
}

void Bullet::Update(std::vector<Bullet> &bullets, float deltaTime)
{
    // moves bullet each frame (takes into account deltaTime from last frame)
    // to stop issues arising with varying speeds of machines.
    for (auto &bullet : bullets)
    {
        bullet.y -= bullet.speed * deltaTime;
    }

    for (int i = 0; i < bullets.size(); i++)
    {
        if (bullets[i].y < 0)
        {
            bullets.erase(bullets.begin() + i);
        }
    }
}