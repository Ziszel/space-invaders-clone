#include "player.hpp"
#include <raylib-cpp.hpp>
#include <enemy.hpp>
#include <bullet.hpp>
#include <vector>

enum gameState
{
    mainMenu,
    mainGameLoop,
    gameOver
};

Dir Enemy::dir = left;
float Enemy::y = 40;

// TODO: Fix initial gap between enemy 0 and enemy 1
std::vector<Enemy> spawnEnemies()
{
    std::vector<Enemy> enemies;
    for (int i = 0; i < 10; i++)
    {
        Enemy enemy(80 + (80 * i));
        enemies.push_back(enemy);
    }
    return enemies;
}

// Collision Bounding Box, once working, isolate the code
// TODO: Once entity system is implemented, update this function to take entities
// as params
void bulletToAlien(std::vector<Bullet> &bullets, std::vector<Enemy> &enemies)
{
    int collision = 0;
    int tempI;
    int tempJ;

    for (int i = 0; i < bullets.size(); i++)
    {
        for (int j = 0; j < enemies.size(); j++)
        {
            if (bullets[i].x <= enemies[j].x + enemies[j].w &&
                enemies[j].x <= bullets[i].x + bullets[i].w &&
                bullets[i].y <= enemies[j].y + enemies[j].h &&
                enemies[j].y <= bullets[i].y + bullets[i].h)
            {
                bullets.erase(bullets.begin() + i);
                enemies.erase(enemies.begin() + j);
            }
        }
    }
}

void setupGame(Player *Player, std::vector<Enemy> &enemies, std::vector<Bullet> &bullets)
{
    Player->x = GetScreenWidth() / 2 - 60;
    Player->y = GetScreenHeight() - 40;
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
    float movementTimer = 50;
    gameState gs = mainMenu;

    raylib::Color textColor(LIGHTGRAY);
    raylib::Window w(screenWidth, screenHeight, "Space Invaders");

    SetTargetFPS(fps);

    // create game objects
    Player *player = new Player();
    std::vector<Enemy> enemies = spawnEnemies();
    std::vector<Bullet> bullets;

    SetWindowPosition(800, 800);

    while (!w.ShouldClose())
    {

        deltaTime = GetFrameTime();

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
                std::cout << "Enemy y: ";
                std::cout << (Enemy::y) << "\n";
                if (IsKeyDown(KEY_A) && player->x > 0)
                {
                    player->x -= player->speed * deltaTime;
                }
                else if (IsKeyDown(KEY_D) && player->x < screenWidth - player->w)
                {
                    player->x += player->speed * deltaTime;
                }

                if (IsKeyPressed(KEY_SPACE))
                {
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
                    movementTimer = 5;
                }
                else
                {
                    movementTimer--;
                }

                for (auto &bullet : bullets)
                {
                    bullet.y -= bullet.speed * deltaTime;
                }

                bulletToAlien(bullets, enemies);

                if (Enemy::y > player->y - enemies.front().h)
                {
                    break;
                }

                // Draw
                BeginDrawing();
                ClearBackground(BLACK);

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

    CloseWindow();

    return 0;
}
