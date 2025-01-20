#include <iostream>
#include <conio.h>
#include <windows.h>
#include <chrono>
#include <thread>

using namespace std;

class Stopwatch
{
public:
    void start()
    {
        start_time = chrono::high_resolution_clock::now();
        running = true;
    }

    void stop()
    {
        end_time = chrono::high_resolution_clock::now();
        running = false;
    }

    double elapsedMilliseconds()
    {
        chrono::time_point<chrono::high_resolution_clock> end;
        if (running)
        {
            end = chrono::high_resolution_clock::now();
        }
        else
        {
            end = end_time;
        }
        return chrono::duration_cast<chrono::milliseconds>(end - start_time).count();
    }

    double elapsedSeconds()
    {
        return elapsedMilliseconds() / 1000.0;
    }

private:
    chrono::time_point<chrono::high_resolution_clock> start_time;
    chrono::time_point<chrono::high_resolution_clock> end_time;
    bool running = false;
};

struct Point
{
    int x;
    int y;
    Point()
    {
        x = 0;
        y = 0;
    }
    Point(int xinput, int yinput)
    {
        x = xinput;
        y = yinput;
    }
};
struct Ship
{
    Point position;
    int health;
};
struct Laser
{
    Point pos;
    int speed;
    int power;
    Laser()
    {
        pos = Point(0, 0);
        speed = 0;
        power = 0;
    }
    Laser(Point posi, int spee, int pow)
    {
        pos.x = posi.x;
        pos.y = posi.y;
        speed = spee;
        power = pow;
    }
};

void filllasers(int *laser);

void changecurser(Point position);
void mainGame();
bool endgame();
void keyboardInput(Ship &our, int i, bool &space);
// lasers
void newPlaceForLasers(Laser *&lasers, int &laserCount);
void moveLasers(Laser *&lasers, int &laserCount, int *laserPos, bool enemy);
void createNewLaser(Laser *&lasers, int &laserIndex, int &maxCount, Laser create, int *laserPos);
void lasersClosion(Laser *&enemy, Laser *&playerSide, int *&enemyRow, int *&playerRow);
void showlasers(Laser &lasers, bool firsttime);
const double frameTime = 0.25;
Point boardSize = Point(60, 40);

int main()
{
    mainGame();
    // cout << getch();
    SetConsoleOutputCP(CP_UTF8);

    cout << '\u0021';
    return 0;
}

void changecurser(Point position)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    int x = position.x, y = position.y;
    COORD screen;
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    screen.X = x;
    screen.Y = y;
    SetConsoleCursorPosition(hOutput, screen);
}

void mainGame()
{
    system("cls");
    Stopwatch sw;
    Ship our;
    our.position = Point(boardSize.x / 2, boardSize.y);
    int enemylaserscount = 0, playerlaserscount = 0, eli = 1, pli = 1;
    int enemyLaserlast[boardSize.x], playerLaserlast[boardSize.x];
    int *enemyLaserlane = enemyLaserlast, *playerLaserlane = playerLaserlast;
    bool space = false;
    filllasers(enemyLaserlane);
    filllasers(playerLaserlane);
    Laser *playerlasers = new Laser[pli];
    // Laser *enemylasers = new Laser[eli-1];
    int cycle = 1, playerlaserpower = 10, playerlaserspeed = -1;

    while (true)
    {
        sw.start();
        // if (endgame())
        // {
        //     return;
        // }

        thread t1(keyboardInput, ref(our), cycle, ref(space));
        // print , exp , health bar , time ...

        moveLasers(playerlasers, playerlaserscount, playerLaserlane, false); // mvoe lasers each frame
        // moveLasers(enemylasers, enemylaserscount, enemyLaserlane, true);
        //

        t1.join();
        if (space) // if space clicked in the func -> create new laser for player
        {
            createNewLaser(playerlasers, playerlaserscount, pli, Laser(Point(our.position.x, our.position.y - 1), playerlaserspeed, playerlaserpower), playerLaserlane);
            space = false;
        }
        // lasersClosion(enemylasers, playerlasers, enemyLaserlane, playerLaserlane); // lasres colision each frame from the last lasers on each culomn

        // after all the process stops the timer
        sw.stop();
        if (sw.elapsedSeconds() < frameTime)
        {
            Sleep((frameTime - sw.elapsedSeconds()) * 1000);
        }

        // print the number of the frame
        // changecurser(Point(0, 1));
        // cout << cycle;
        // cycle++;
    }
}

void filllasers(int *laser)
{
    for (int i = 0; i < boardSize.x; i++)
        laser[i] = -1;
}

void keyboardInput(Ship &our, int i, bool &space)
{
    if (!kbhit())
        return;

    char input = getch();
    changecurser(Point(our.position.x, our.position.y));
    cout << " ";

    switch (input)
    {
    case 'A':
    case 'a':
    {
        if (our.position.x > 0)
        {
            our.position.x--;
        }
        break;
    }
    case 'D':
    case 'd':
    {
        if (our.position.x < boardSize.x)
        {
            our.position.x++;
        }
        break;
    }
    case ' ':
    {
        // createNewLaser()
        // changecurser(Point(0, 0));
        // cout << i;
        space = true;
        break;
    }
    default:
        exit(1);
    }
    // changecurser(Point(0, 0));
    // cout << i;
    changecurser(Point(our.position.x, our.position.y));
    // print ship

    cout << "a";
}
bool endgame()
{
    return false;
}

//
// lasers
void newPlaceForLasers(Laser *&lasers, int &laserCount)
{
    Laser *newLaser = new Laser[laserCount * 2];
    if (newLaser)
    {
        for (int i = 0; i < laserCount; i++)
            newLaser[i] = lasers[i];
        delete[] lasers;
        lasers = newLaser;
        laserCount *= 2;
        return;
    }
    newPlaceForLasers(lasers, laserCount);
    return;
}
void moveLasers(Laser *&lasers, int &laserCount, int *laserPos, bool enemy)
{
    for (int i = 0; i < laserCount; i++)
    {
        showlasers(lasers[i], true);
        lasers[i].pos.y += lasers[i].speed;
        if (lasers[i].pos.y == 0)
        {
            lasers[i].power = 0;
            laserPos[lasers[i].pos.x] = -1;
            continue;
        }

        if (enemy)
            if (lasers[laserPos[lasers[i].pos.x]].pos.y < lasers[i].pos.y || laserPos[lasers[i].pos.x] == -1)
            {
                laserPos[lasers[i].pos.x] = i;
            }
            else
                ;
        else if (lasers[laserPos[lasers[i].pos.x]].pos.y > lasers[i].pos.y || laserPos[lasers[i].pos.x] == -1)
        {
            laserPos[lasers[i].pos.x] = i;
        }
        showlasers(lasers[i], false);
    }
    return;
}
void createNewLaser(Laser *&lasers, int &laserIndex, int &maxCount, Laser create, int *laserPos)
{
    for (int i = 0; i < laserIndex; i++)
    {
        if (lasers[i].power == 0)
        {
            lasers[i].pos = create.pos;
            lasers[i].power = create.power;
            lasers[i].speed = create.speed;
            if (laserPos[lasers[i].pos.x] == -1)
                laserPos[lasers[i].pos.x] = i;
            return;
        }
    }
    if (laserIndex == maxCount)
    {
        newPlaceForLasers(lasers, maxCount);
    }
    lasers[laserIndex].pos = create.pos;
    lasers[laserIndex].power = create.power;
    lasers[laserIndex].speed = create.speed;
    if (laserPos[lasers[laserIndex].pos.x] == -1)
        laserPos[lasers[laserIndex].pos.x] = laserIndex;
    laserIndex++;
    return;
}
void lasersClosion(Laser *&enemy, Laser *&playerSide, int *&enemyRow, int *&playerRow)
{
    for (int i = 0; i < boardSize.x; i++)
    {
        int enemyY = 0, playerY = 0;
        if (enemyRow[i] != -1)
        {
            enemyY = enemy[enemyRow[i]].pos.y;
            if (playerRow[i] != -1)
            {
                playerY = playerSide[playerRow[i]].pos.y;
                if (playerY <= enemyY)
                {
                    if (playerSide[playerRow[i]].power >= enemy[enemyRow[i]].power)
                    {
                        playerSide[playerRow[i]].power -= enemy[enemyRow[i]].power;
                        enemy[enemyRow[i]].power = 0;
                    }
                    if (playerSide[playerRow[i]].power <= enemy[enemyRow[i]].power)
                    {
                        enemy[enemyRow[i]].power -= playerSide[playerRow[i]].power;
                        playerSide[playerRow[i]].power = 0;
                    }
                    if (playerSide[playerRow[i]].power == 0)
                        playerRow[i] = -1;
                    if (enemy[enemyRow[i]].power == 0)
                        enemyRow[i] = -1;
                }
            }
        }
    }
}
void showlasers(Laser &lasers, bool firsttime)
{
    changecurser(Point(lasers.pos.x, lasers.pos.y));
    cout << (firsttime ? " " : "|");
}
