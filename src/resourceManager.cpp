#include "resourceManager.hpp"

void ResourceManager::loadResources()
{
    // load SFX
    this->shot = LoadSound("../res/sfx/laserShot.wav");
    this->enemyExplosion = LoadSound("../res/sfx/enemyExplosion.wav");
    this->playerExplosion = LoadSound("../res/sfx/playerExplosion.wav");
    // load music streams
    this->gameMusic = LoadMusicStream("../res/music/space-invaders-tune.wav");
}

void ResourceManager::unloadResources()
{
    // unload SFX
    UnloadSound(this->shot);
    UnloadSound(this->enemyExplosion);
    UnloadSound(this->playerExplosion);
    // unload music streams
    UnloadMusicStream(this->gameMusic);
}