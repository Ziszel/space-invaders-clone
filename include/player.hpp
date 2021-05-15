#include <raylib-cpp.hpp>
#pragma once

class Player
{
private:
    
public:
    float x;
    float y;
    const int w = 120;
    const int h = 20;
    const int speed = 200;
    int score = 0;

    Player();

    void Update(float deltaTime, int screenWidth);
    void Draw();
};
