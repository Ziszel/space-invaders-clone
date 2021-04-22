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

void setupGame(Player *Player, std::vector<Enemy> &enemies, std::vector<Bullet> &bullets)
{
    Player->x = GetScreenWidth() / 2 - 60;
    Player->y = GetScreenHeight() - 40;
    Player->score = 0;
    enemies = spawnEnemies();
    Enemy::dir = left;
    Enemy::y = 40;
    bullets.clear();
}

int main()
{
    // Initialization
    int screenWidth = 1000;
    int screenHeight = 650;
    float fps = 60.0f;
    float deltaTime;
    float movementTimer = 40;
    gameState gs = mainMenu;

    raylib::Color textColor(LIGHTGRAY);
    raylib::Window w(screenWidth, screenHeight, "Space Invaders");
    InitAudioDevice();
    SetTargetFPS(fps);

    // Load resources
    Sound shot = LoadSound("res/sfx/laserShot.wav");
    Sound enemyExplosion = LoadSound("res/sfx/enemyExplosion.wav");
    Sound playerExplosion = LoadSound("res/sfx/playerExplosion.wav");
    Music gameMusic = LoadMusicStream("res/music/space-invaders-tune.wav");
    PlayMusicStream(gameMusic);

    // create game objects
    Player *player = new Player();
    std::vector<Enemy> enemies = spawnEnemies();
    std::vector<Bullet> bullets;

    SetWindowPosition(800, 800);

    while (!w.ShouldClose())
    {
        UpdateMusicStream(gameMusic);

        if (gs == mainMenu)
        {   

            if (IsKeyPressed(KEY_SPACE))
            {
                setupGame(player, enemies, bullets);
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
            while (enemies.size() > 0)
            {
                deltaTime = GetFrameTime();
                UpdateMusicStream(gameMusic);

                if (IsKeyDown(KEY_A) && player->x > 0)
                {
                    player->x -= player->speed * deltaTime;
                }
                else if (IsKeyDown(KEY_D) && player->x < screenWidth - player->w)
                {
                    player->x += player->speed * deltaTime;
                }

                if (IsKeyPressed(KEY_SPACE) && bullets.size() < 1)
                {
                    PlaySound(shot);
                    Bullet bullet(player->x + player->w / 2, player->y);
                    bullets.push_back(bullet);
                }

                if (movementTimer <= 0)
                {
                    if (Enemy::dir == left && (enemies.front().x - enemies.front().speed * deltaTime) < 0)
                    {
                        Enemy::y += 40;
                        Enemy::dir = right;
                    }
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
                    break;
                }

                // Draw
                BeginDrawing();
                ClearBackground(BLACK);

                DrawText(TextFormat("Score: %i", player->score), screenWidth - 120, 10, 24, WHITE);

                DrawRectangle(player->x, player->y, player->w, player->h, WHITE);

                for (auto &enemy : enemies)
                {
                    DrawRectangle(enemy.x, enemy.y, enemy.w, enemy.h, DARKGREEN);
                }

                for (auto bullet : bullets)
                {
                    DrawRectangle(bullet.x, bullet.y, bullet.w, bullet.h, DARKBLUE);
                }

                EndDrawing();
            }

            gs = gameOver;
        }
        else if (gs == gameOver)
        {
            if (IsKeyPressed(KEY_SPACE))
            {
                gs = mainMenu;
            }
            else if (IsKeyPressed(KEY_C))
            {
                break;
            }

            BeginDrawing();
            ClearBackground(BLACK);

            DrawText("GAME OVER, PRESS SPACE TO PLAY AGAIN", screenWidth / 2 - 200, screenHeight / 2, 24, WHITE);
            DrawText("PRESS C TO CLOSE THE GAME", screenWidth / 2 - 200, screenHeight / 2 + 200, 24, WHITE);

            EndDrawing();
        }
    }

    UnloadSound(shot);

    CloseAudioDevice();

    return 0;
}
