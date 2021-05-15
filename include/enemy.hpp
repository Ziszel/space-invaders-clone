#include "resourceManager.hpp"
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
    Texture2D texture;
    static Dir dir;

    Enemy(float x, Texture2D texture);

    Enemy();

    // DEBUG: Lets me know when an enemy is being destroyed
    ~Enemy() {
        std::cout << "Enemy destroyed!" << std::endl;
    }

    void Update(float &movementTimer, float dt, int screenWidth, std::vector<Enemy> &enemies);

    void Move(float dt, int screenWidth);

    void Draw();
};
