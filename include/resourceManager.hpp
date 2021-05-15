#include <raylib-cpp.hpp>
#pragma once

class ResourceManager
{

public:
    Sound shot;
    Sound enemyExplosion;
    Sound playerExplosion; 
    Music gameMusic;

    ResourceManager()
    {
        loadResources();
    }

    void loadResources();
    void unloadResources();
};