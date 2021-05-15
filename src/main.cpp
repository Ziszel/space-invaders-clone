#include "player.hpp"
#include "spawnEnemies.hpp"
#include "enemy.hpp"
#include "bullet.hpp"
#include "bulletToAlien.hpp"
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

// set all parameters to default for game init and replayability.
void setupGame(Player *Player, std::vector<Enemy> &enemies, std::vector<Bullet> &bullets, float &win)
{
    Player->x = GetScreenWidth() / 2 - 60;
    Player->y = GetScreenHeight() - 40;
    Player->score = 0;
    enemies = spawnEnemies();
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
    raylib::Window w(screenWidth, screenHeight, "Space Invaders");
    InitAudioDevice();
    SetTargetFPS(fps);

    // Load resources
    Sound shot = LoadSound("../res/sfx/laserShot.wav");
    Sound enemyExplosion = LoadSound("../res/sfx/enemyExplosion.wav");
    Sound playerExplosion = LoadSound("../res/sfx/playerExplosion.wav");
    Music gameMusic = LoadMusicStream("../res/music/space-invaders-tune.wav");
    PlayMusicStream(gameMusic);

    // create game objects
    Player *player = new Player();
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;

    SetWindowPosition(800, 800);

    while (!w.ShouldClose())
    {
        // no matter what state we are in, the music should play
        UpdateMusicStream(gameMusic);

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
                // update
                deltaTime = GetFrameTime();
                UpdateMusicStream(gameMusic);

                // update player movement
                player->Update(deltaTime, screenWidth);

                // Handle bullet firing (only allows one on screen), includes sound
                if (IsKeyPressed(KEY_SPACE) && bullets.size() < 1)
                {
                    PlaySound(shot);
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
                bulletToAlien(bullets, enemies, player, enemyExplosion);

                if (Enemy::y > player->y - enemies.front().h)
                {
                    PlaySound(playerExplosion);
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
    UnloadSound(shot);

    CloseAudioDevice();

    return 0;
}
