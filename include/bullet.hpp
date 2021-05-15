#include <raylib-cpp.hpp>
#include <vector>
#pragma once

class Bullet
{
private:
    
public:
    float x;
    float y;
    int w = 20;
    int h = 20;
    int speed = 400;

    Bullet(float x, float y);

    void Draw();

    void Update(std::vector<Bullet> &bullets, float deltaTime);
};
