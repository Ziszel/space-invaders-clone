#include "player.hpp"
#include "spawnEnemies.hpp"
#include "enemy.hpp"
#include "bullet.hpp"
#include "bulletToAlien.hpp"
#include "resourceManager.hpp"
#include <raylib-cpp.hpp>
#include <vector>

enum gameState
{
    mainMenu,
    mainGameLoop,
    gameOver
};

Dir Enemy::dir = left;
float Enemy::y = 40;

// Initialise global resource variables
ResourceManager rm;

// Allows the user to quit the game prematurely
bool checkIfQuit(gameState &gs, float &win)
{
    if(IsKeyPressed(KEY_Q))
    {
        win = 1;
        return true;
    }
    return false;
}

// set all parameters to default for game init and replayability.
void setupGame(Player *Player, std::vector<Enemy> &enemies, std::vector<Bullet> &bullets, float &win)
{
    Player->x = GetScreenWidth() / 2 - 60;
    Player->y = GetScreenHeight() - 40;
    Player->score = 0;
    enemies = spawnEnemies(rm.enemyTexture);
    Enemy::dir = left;
    Enemy::y = 40;
    bullets.clear();
    win = 0;
}

int main()
{
    // Initialization
    int screenWidth = 1000;
    int screenHeight = 650;
    float fps = 60.0f;
    float deltaTime;
    float movementTimer = 40;
    float win = 0;
    gameState gs = mainMenu;
    raylib::Color textColor(LIGHTGRAY);

    // initialise graphics and audio devices
    raylib::Window w(screenWidth, screenHeight, "Space Invaders");
    InitAudioDevice();
    
    // load resources
    rm.loadResources();

    SetTargetFPS(fps);

    PlayMusicStream(rm.gameMusic);

    // create game objects
    Player *player = new Player();
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;

    // Makes sure the default window position is in a playable position regardless of the primary monitor used.
    SetWindowPosition(GetMonitorWidth(0) / 2 - screenWidth / 2, GetMonitorHeight(0) / 2);

    while (!w.ShouldClose())
    {
        // no matter what state we are in, the music should play
        UpdateMusicStream(rm.gameMusic);

        if (gs == mainMenu)
        {   

            if (IsKeyPressed(KEY_SPACE))
            {
                setupGame(player, enemies, bullets, win);
                gs = mainGameLoop;
            }
            else if (IsKeyPressed(KEY_C))
            {
                break;
            }

            BeginDrawing();
            ClearBackground(BLACK);

            DrawText("SPACE INVADERS: CALIN CLEMENT DERMOTT, 2021", screenWidth / 2 - 300, screenHeight / 2 - 100, 24, WHITE);
            DrawText("PRESS C TO CLOSE THE GAME", screenWidth / 2 - 175, screenHeight / 2 + 250, 24, WHITE);
            DrawText("PRESS SPACE TO START", screenWidth / 2 - 150, screenHeight / 2 + 200, 24, WHITE);

            EndDrawing();
        }
        else if (gs == mainGameLoop)
        {
            // main game loop
            while (enemies.size() > 0)
            {
                if (checkIfQuit(gs, win)) { break; }

                // update
                deltaTime = GetFrameTime();
                UpdateMusicStream(rm.gameMusic);

                // update player movement
                player->Update(deltaTime, screenWidth);

                // Handle bullet firing (only allows one on screen), includes sound
                if (IsKeyPressed(KEY_SPACE) && bullets.size() < 1)
                {
                    PlaySound(rm.shot);
                    // magic number (10) is half of a bullets width
                    // this centres the bullet to the player.
                    Bullet bullet(player->x + player->w / 2 - 10, player->y);
                    bullets.push_back(bullet);
                }

                // use the enemy at the front of the list to call the update function
                // (which in turn updates all of them)
                enemies.front().Update(movementTimer, deltaTime, screenWidth, enemies);

                for (auto bullet : bullets)
                {
                    bullet.Update(bullets, deltaTime);
                }

                // handles bullet to enemy collision
                bulletToAlien(bullets, enemies, player, rm.enemyExplosion);

                if (Enemy::y > player->y - enemies.front().h)
                {
                    PlaySound(rm.playerExplosion);
                    win = 1;
                    break;
                }

                // Draw
                BeginDrawing();
                ClearBackground(BLACK);

                DrawText(TextFormat("Score: %i", player->score), screenWidth - 120, 10, 24, WHITE);

                player->Draw();

                for (auto &enemy : enemies)
                {
                    enemy.Draw();
                }

                for (auto bullet : bullets)
                {
                    bullet.Draw();
                }

                EndDrawing();
            }
            gs = gameOver;
        }
        // Game Over screen/state
        else if (gs == gameOver)
        {
            // If space is pressed, change to main menu
            if (IsKeyPressed(KEY_SPACE))
            {
                gs = mainMenu;
            }
            // If C is pressed, quit game
            else if (IsKeyPressed(KEY_C))
            {
                break;
            }

            BeginDrawing();
            ClearBackground(BLACK);

            // Shows text on screen dependant on whether or not the player shot all the aliens
            if (win == 1)
            {
                DrawText("You Lose!", screenWidth / 2 - 65, screenHeight / 2 - 50, 24, WHITE);
                DrawText("GAME OVER, PRESS SPACE TO PLAY AGAIN", screenWidth / 2 - 265, screenHeight / 2, 24, WHITE);
                DrawText("PRESS C TO CLOSE THE GAME", screenWidth / 2 - 200, screenHeight / 2 + 200, 24, WHITE);
                DrawText(TextFormat("Score: %i", player->score), screenWidth - 120, 10, 24, WHITE);
            }
            else
            {
                DrawText("You Win!", screenWidth / 2 - 65, screenHeight / 2 - 50, 24, WHITE);
                DrawText("GAME OVER, PRESS SPACE TO PLAY AGAIN", screenWidth / 2 - 265, screenHeight / 2, 24, WHITE);
                DrawText("PRESS C TO CLOSE THE GAME", screenWidth / 2 - 200, screenHeight / 2 + 200, 24, WHITE);
                DrawText(TextFormat("Score: %i", player->score), screenWidth - 120, 10, 24, WHITE);
            }

            EndDrawing();
        }
    }

    // Unload audio and close devices to ensure safe termination of application.
    rm.unloadResources();

    CloseAudioDevice();

    return 0;
}
