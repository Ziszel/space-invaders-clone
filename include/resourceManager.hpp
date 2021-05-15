#include <raylib-cpp.hpp>
#pragma once

class ResourceManager
{

public:
    Sound shot;
    Sound enemyExplosion;
    Sound playerExplosion; 
    Music gameMusic;
    Texture2D enemyTexture;

    ResourceManager();

    void loadResources();
    void unloadResources();
};