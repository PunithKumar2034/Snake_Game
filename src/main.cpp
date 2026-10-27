
#include "raylib.h"
#include <deque>
#include <random>

constexpr int CELL_SIZE = 20;
constexpr int GRID_WIDTH = 40;
constexpr int GRID_HEIGHT = 30;

constexpr int SCREEN_WIDTH = CELL_SIZE * GRID_WIDTH;
constexpr int SCREEN_HEIGHT = CELL_SIZE * GRID_HEIGHT;
constexpr float MOVE_INTERVAL = 0.12f;

struct Cell
{
    int x;
    int y;
};

bool SameCell(const Cell& a, const Cell& b)
{
    return a.x == b.x && a.y == b.y;
}

Cell CreateFood(const std::deque<Cell>& snake,
    std::mt19937& rng)
{
    std::uniform_int_distribution<int> xDist(0, GRID_WIDTH - 1);
    std::uniform_int_distribution<int> yDist(0, GRID_HEIGHT - 1);

    Cell food;

    do
    {
        food = { xDist(rng), yDist(rng) };

        bool occupied = false;

        for (const Cell& segment : snake)
        {
            if (SameCell(food, segment))
            {
                occupied = true;
                break;
            }
        }

        if (!occupied)
            return food;

    } while (true);
}

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake Game");
    SetTargetFPS(60);

    std::random_device rd;
    std::mt19937 rng(rd());

    std::deque<Cell> snake = {
        { 20, 15 },
        { 19, 15 },
        { 18, 15 }
    };

    Cell direction = { 1, 0 };
    Cell food = CreateFood(snake, rng);

    float moveTimer = 0.0f;
    int score = 0;
    bool gameOver = false;

    while (!WindowShouldClose())
    {
        if (!gameOver)
        {
            float deltaTime = GetFrameTime();
            moveTimer += deltaTime;

            // Read movement input.
            if (IsKeyPressed(KEY_UP) && direction.y == 0)
                direction = { 0, -1 };
            else if (IsKeyPressed(KEY_DOWN) && direction.y == 0)
                direction = { 0, 1 };
            else if (IsKeyPressed(KEY_LEFT) && direction.x == 0)
                direction = { -1, 0 };
            else if (IsKeyPressed(KEY_RIGHT) && direction.x == 0)
                direction = { 1, 0 };

            if (moveTimer >= MOVE_INTERVAL)
            {
                Cell newHead = {
                    snake.front().x + direction.x,
                    snake.front().y + direction.y
                };

                // Check wall collision.
                if (newHead.x < 0 || newHead.x >= GRID_WIDTH ||
                    newHead.y < 0 || newHead.y >= GRID_HEIGHT)
                {
                    gameOver = true;
                }
                else
                {
                    bool eating = SameCell(newHead, food);

                    // Check collision with the body.
                    // If not eating, the tail moves away this turn.
                    for (int i = 0;
                        i < static_cast<int>(snake.size()); ++i)
                    {
                        if (!eating &&
                            i == static_cast<int>(snake.size()) - 1)
                            continue;

                        if (SameCell(newHead, snake[i]))
                        {
                            gameOver = true;
                            break;
                        }
                    }

                    if (!gameOver)
                    {
                        snake.push_front(newHead);

                        if (eating)
                        {
                            ++score;
                            food = CreateFood(snake, rng);
                        }
                        else
                        {
                            snake.pop_back();
                        }
                    }
                }

                moveTimer = 0.0f;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        // Draw food.
        DrawRectangle(
            food.x * CELL_SIZE,
            food.y * CELL_SIZE,
            CELL_SIZE - 1,
            CELL_SIZE - 1,
            RED
        );

        // Draw snake.
        for (const Cell& segment : snake)
        {
            DrawRectangle(
                segment.x * CELL_SIZE,
                segment.y * CELL_SIZE,
                CELL_SIZE - 1,
                CELL_SIZE - 1,
                GREEN
            );
        }

        DrawText("SNAKE GAME", 10, 10, 20, RAYWHITE);
        DrawText(TextFormat("Score: %d", score),
            10, 35, 16, RAYWHITE);

        if (gameOver)
        {
            DrawText("GAME OVER", 330, 260, 30, RED);
            DrawText("Press ESC to exit",
                340, 300, 16, RAYWHITE);
        }
        else
        {
            DrawText("Arrow keys: Move",
                10, 55, 16, GRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
