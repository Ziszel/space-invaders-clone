#include <iostream>
#include <vector>
#include <raylib-cpp.hpp>
#pragma once

enum Dir {
    left,
    right
};

class Enemy
{
    
public:
    float x;
    static float y;
    int w = 50;
    int h = 50;
    float speed = 1600;
    bool isAlive = true;
    Texture2D texture = LoadTexture("../res/images/invader.png");
    static Dir dir;

    Enemy(float x);

    Enemy();

    // DEBUG: Lets me know when an enemy is being destroyed
    ~Enemy() {
        std::cout << "Enemy destroyed!" << std::endl;
    }

    void Update(float dt, int screenWidth);

    void Move(float dt, int screenWidth);

    void Draw();
};
