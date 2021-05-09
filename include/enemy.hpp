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
private:
    
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

    ~Enemy() {
        std::cout << "Enemy destroyed!" << std::endl;
    }

    void Update(float dt, int screenWidth);

    void Move(float dt, int screenWidth);
};
