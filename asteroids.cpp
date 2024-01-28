#include <iostream>
#include <vector>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <algorithm>
#include <cstdio>

using namespace std;

// Function to clear the screen
void clearScreen()
{
    COORD topLeft = {0, 0};
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(
        console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    SetConsoleCursorPosition(console, topLeft);
}

// Function to hide the cursor
void hideCursor()
{
    CONSOLE_CURSOR_INFO cursor;
    cursor.dwSize = 100;
    cursor.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

// Function to display game name
void Displayname()
{
    cout << "\033[1m" << "\033[3m" << "\033[5m";
    cout << R"(

                 _          _  _  _  _   _  _  _  _  _  _  _  _  _  _  _  _  _  _       _  _  _  _   _  _  _  _  _  _  _        _  _  _  _
               _(_)_      _(_)(_)(_)(_)_(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_) _  _(_)(_)(_)(_)_(_)(_)(_)(_)(_)(_)(__)    _(_)(_)(_)(_)_
             _(_) (_)_   (_)          (_)     (_)      (_)            (_)         (_)(_)          (_)  (_)    (_)      (_)_ (_)          (_)
           _(_)     (_)_ (_)_  _  _  _        (_)      (_) _  _       (_) _  _  _ (_)(_)          (_)  (_)    (_)        (_)(_)_  _  _  _
          (_) _  _  _ (_)  (_)(_)(_)(_)_      (_)      (_)(_)(_)      (_)(_)(_)(_)   (_)          (_)  (_)    (_)        (_)  (_)(_)(_)(_)_
          (_)(_)(_)(_)(_) _           (_)     (_)      (_)            (_)   (_) _    (_)          (_)  (_)    (_)       _(_) _           (_)
          (_)         (_)(_)_  _  _  _(_)     (_)      (_) _  _  _  _ (_)      (_) _ (_)_  _  _  _(_)_ (_) _  (_)_  _  (_)  (_)_  _  _  _(_)
          (_)         (_)  (_)(_)(_)(_)       (_)      (_)(_)(_)(_)(_)(_)         (_)  (_)(_)(_)(_) (_)(_)(_)(_)(_)(_)(_)     (_)(_)(_)(_)

        )";
    cout << "\033[0m" << "\033[3m";
    cout << R"(
                                        ALERT: Galactic Traveler!

                                        Mission Control here. You are entering the dangerous asteroid belt.
                                        Prepare for a challenging journey. Your spaceship is equipped with advanced
                                        laser technology. Engage hostile asteroids using 'space' to fire bullets.
                                        Use 'a' and 'd' to move left and right.

                                        Your mission: Navigate through the asteroid field and survive at all costs.
                                        Each asteroid destroyed contributes to your score.

                                        Remember, a direct collision with an asteroid is catastrophic. Stay sharp!)";
    hideCursor();
    Sleep(8000);
    cout << "\033[0m";
}

// Function to set the console window size
void setConsoleWindowSize(int width, int height)
{
    COORD coord;
    coord.X = width;
    coord.Y = height;

    SMALL_RECT Rect;
    Rect.Top = 0;
    Rect.Left = 0;
    Rect.Bottom = height - 1;
    Rect.Right = width - 1;

    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleScreenBufferSize(Handle, coord);
    SetConsoleWindowInfo(Handle, TRUE, &Rect);
}

// Function to draw the player's ship
void drawShip(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    cout << "  " << char(241) << endl;
}

// Function to draw an asteroid
void drawAsteroid(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    cout << char(167) << endl;
}

// Function to draw the border of the playable region
void drawBorder(int startX, int startY, int width, int height)
{
    COORD coord;
    for (int i = 0; i < width; ++i)
    {
        coord.X = startX + i;
        coord.Y = startY;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        cout << char(219);
        coord.Y = startY + height - 1;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        cout << char(219);
    }

    for (int i = 0; i < height; ++i)
    {
        coord.X = startX;
        coord.Y = startY + i;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        cout << char(219);
        coord.X = startX + width - 1;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        cout << char(219);
    }
}

int main()
{
    Displayname();
    Sleep(8000);

    // Set console window size
    setConsoleWindowSize(80, 25);

    const int totalWidth = 80;  // Total width of the console window
    const int totalHeight = 25; // Total height of the console window

    const int gameWidth = 40;  // Width of the playable region
    const int gameHeight = 20; // Height of the playable region

    const int offsetX = (totalWidth - gameWidth) / 2; // X offset to center the playable region
    const int offsetY = (totalHeight - gameHeight) / 2; // Y offset to center the playable region

    int playerX = offsetX + gameWidth / 2 - 2; // Initial X position of the player (centered)
    int playerY = offsetY + gameHeight - 4;     // Initial Y position of the player

    vector<pair<int, int>> asteroids; // Vector to store asteroid positions
    vector<pair<int, int>> bullets;   // Vector to store bullet positions

    char input; // To store user input
    int score = 0; // Score variable

    srand(static_cast<unsigned>(time(nullptr))); // Seed the random number generator

    while (true)
    {
        clearScreen();

        // Draw the border of the playable region
        drawBorder(offsetX - 1, offsetY - 1, gameWidth + 2, gameHeight + 2);

        // Draw asteroids
        for (const auto &asteroid : asteroids)
        {
            drawAsteroid(asteroid.first, asteroid.second);
        }

        // Draw the player's ship
        drawShip(playerX, playerY);

        // Draw bullets
        for (const auto &bullet : bullets)
        {
            COORD coord;
            coord.X = bullet.first;
            coord.Y = bullet.second;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            cout << "|";
        }

        // Move bullets
        for (auto &bullet : bullets)
        {
            bullet.second--;
        }

        // Remove bullets that have reached the top
        bullets.erase(remove_if(bullets.begin(), bullets.end(), [offsetY](const auto &bullet) { return bullet.second < offsetY; }), bullets.end());

        // Move the asteroids
        for (auto &asteroid : asteroids)
        {
            // Move asteroid down
            asteroid.second++;

            // Check if asteroid is at the bottom border, remove it
            if (asteroid.second >= offsetY + gameHeight)
            {
                asteroid = make_pair(rand() % gameWidth + offsetX, offsetY);
            }
        }

        // Add new asteroids from the top
        if (rand() % 10 == 0)
        {
            asteroids.push_back({rand() % gameWidth + offsetX, offsetY});
        }

        // Check for user input
        if (_kbhit())
        {
            input = _getch();
            // Move the player's ship
            switch (input)
            {
            case 'a':
                playerX = max(playerX - 1, offsetX); //move spaceship left
                break;
            case 'd':
                playerX = min(playerX + 1, offsetX + gameWidth - 4); //move spaceship right
                break;
            case 'q':
                return 0; // Quit the game
            case ' ':
                bullets.push_back({playerX + 2, playerY - 1}); // Shoot bullet
                break;
            }
        }

        // Check for collision with asteroids
        for (auto itAsteroid = asteroids.begin(); itAsteroid != asteroids.end();)
        {
            bool asteroidToRemove = false;

            // Check if any bullet hits the asteroid
            for (auto itBullet = bullets.begin(); itBullet != bullets.end();)
            {
                if (itBullet->first >= itAsteroid->first && itBullet->first < itAsteroid->first + 3 &&
                    itBullet->second == itAsteroid->second)
                {
                    // Remove bullet upon collision
                    itBullet = bullets.erase(itBullet);
                    // Mark the asteroid for removal
                    asteroidToRemove = true;
                    // Increment score
                    score++;
                    break; // Break to avoid modifying the vector during iteration
                } else
                {
                    ++itBullet;
                }
            }

            // Check if the asteroid hits the player
            if (itAsteroid->first >= playerX && itAsteroid->first < playerX + 3 && itAsteroid->second == playerY)
            {
                clearScreen();
                cout << "\033[3m" << "Game Over! Your spaceship collided with an asteroid \n\n";
                cout << "Score: " << score << "\n";
               // cout << "High Score: " << highScore << "\n";
                return 0;
            }

            if (asteroidToRemove)
            {
                // Remove marked asteroid
                itAsteroid = asteroids.erase(itAsteroid);
            } else
            {
                ++itAsteroid;
            }
        }


        // Display score 
        COORD scoreCoord;
        scoreCoord.X = totalWidth - 15;
        scoreCoord.Y = 0;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), scoreCoord);
        cout << "Score: " << score;


        Sleep(175);
        hideCursor();
    }

    return 0;
}
