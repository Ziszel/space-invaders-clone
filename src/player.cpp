#include "../include/player.hpp"

Player::Player()
{
}

void Player::Draw()
{
    DrawRectangle(this->x, this->y, this->w, this->h, WHITE);
}

void Player::Update(float deltaTime, int screenWidth)
{
    // handle player movement, keeping them within screen limits.
    if (IsKeyDown(KEY_A) && this->x > 0)
    {
        this->x -= this->speed * deltaTime;
    }
    else if (IsKeyDown(KEY_D) && this->x < screenWidth - this->w)
    {
        this->x += this->speed * deltaTime;
    }
}