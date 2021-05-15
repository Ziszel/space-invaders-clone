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

                // handle player movement, keeping them within screen limits.
                if (IsKeyDown(KEY_A) && player->x > 0)
                {
                    player->x -= player->speed * deltaTime;
                }
                else if (IsKeyDown(KEY_D) && player->x < screenWidth - player->w)
                {
                    player->x += player->speed * deltaTime;
                }

                // Handle bullet firing (only allows one on screen), includes sound
                if (IsKeyPressed(KEY_SPACE) && bullets.size() < 1)
                {
                    PlaySound(shot);
                    Bullet bullet(player->x + player->w / 2, player->y);
                    bullets.push_back(bullet);
                }

                if (movementTimer <= 0)
                {
                    // see if the front (left most enemy) has hit the side
                    if (Enemy::dir == left && (enemies.front().x - enemies.front().speed * deltaTime) < 0)
                    {
                        Enemy::y += 40;
                        Enemy::dir = right;
                    }
                    // see if the back (right most enemy) has hit the side
                    else if (Enemy::dir == right && (enemies.back().x + enemies.front().speed * deltaTime) > screenWidth - enemies.front().w)
                    {
                        Enemy::y += 40;
                        Enemy::dir = left;
                    }
                    else
                    {
                        for (auto &enemy : enemies)
                        {
                            enemy.Update(deltaTime, screenWidth);
                        }
                    }
                    // if there are more than 2 enemies, speed should be dependant on how many are left, else
                    // make the enemies really fast!
                    if (enemies.size() > 2)
                    {
                        movementTimer = enemies.size() * 1.2 + enemies.size() - 4;
                    }
                    else
                    {
                        movementTimer = 0.5;
                    }
                }
                else
                {
                    movementTimer--;
                }

                for (auto &bullet : bullets)
                {
                    bullet.y -= bullet.speed * deltaTime;
                }

                bulletToAlien(bullets, enemies, player, enemyExplosion);

                for (int i = 0; i < bullets.size(); i++)
                {
                    if (bullets[i].y < 0)
                    {
                        bullets.erase(bullets.begin() + i);
                    }
                }
                

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

                DrawRectangle(player->x, player->y, player->w, player->h, WHITE);

                for (auto &enemy : enemies)
                {
                    enemy.Draw();
                }

                for (auto bullet : bullets)
                {
                    DrawRectangle(bullet.x, bullet.y, bullet.w, bullet.h, DARKBLUE);
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
