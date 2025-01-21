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
struct Invader
{
    int timeToLaser;
    int hp;
    int exp;
    Invader()
    {
        timeToLaser = 0;
        hp = 0;
        exp = 0;
    }
    Invader(int time, int helth, int point)
    {
        timeToLaser = time;
        hp = helth;
        exp = point;
    }
};
struct Wave
{
    Point pos;
    int speed;
    int version;
    int NumberEachColumn[8];
    Invader enemy[4][8];
};

void filllasers(int *laser);

void changecurser(Point position);
void mainGame();
bool endgame();
void keyboardInput(Ship &our, int i, int &space);
// lasers
void newPlaceForLasers(Laser *&lasers, int &laserCount);
void moveLasers(Laser *&lasers, int &laserCount, int *laserPos, bool enemy);
void createNewLaser(Laser *&lasers, int &laserIndex, int &maxCount, Laser create, int *laserPos);
void lasersClosion(Laser *&enemy, Laser *&playerSide, int *&enemyRow, int *&playerRow);
void showlasers(Laser &lasers, bool firsttime);

// to do: create enemy ships from the plan & add the red ship from amirhossein
// enemy
void enemyInit(Wave &invaders, int model);
void moveWave(Wave &invaders);
void checkForFireLasers(Wave &invaders, int framesCount, Laser *&lasers, int &laserIndex, int &maxCount, int *laserPos);
void printWave(Wave &enemy, bool show, int &leftest);
// colorize
void colorize(int input, bool resetcolor);

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
    Wave enemy;

    our.position = Point(boardSize.x / 2, boardSize.y);
    int enemylaserscount = 0, playerlaserscount = 0, eli = 1, pli = 1;
    int enemyWaveModel = 0; // enemywave model
    int enemyLaserlast[boardSize.x];
    int playerLaserlast[boardSize.x];
    int *enemyLaserlane = enemyLaserlast, *playerLaserlane = playerLaserlast;
    int space = 0; // the number of lasers form the keyboard input
    int countFrame = 0;

    enemyInit(enemy, enemyWaveModel);
    filllasers(enemyLaserlane);
    filllasers(playerLaserlane);
    Laser *playerlasers = new Laser[pli];
    Laser *enemylasers = new Laser[eli];

    // player powers
    int cycle = 1, playerlaserpower = 10, playerlaserspeed = -1;

    while (true)
    {
        sw.start();
        if (endgame())
        {
            return;
        }

        keyboardInput(our, cycle, space);

        // todo:  print , exp , health bar , time ...
        moveWave(enemy);
        checkForFireLasers(enemy, countFrame % 70, enemylasers, enemylaserscount, eli, enemyLaserlane);

        moveLasers(playerlasers, playerlaserscount, playerLaserlane, false); // mvoe lasers each frame
        moveLasers(enemylasers, enemylaserscount, enemyLaserlane, true);

        if (space >= 1) // if space clicked in the func -> create new laser for player
        {
            for (int i = -(space / 2); i <= space / 2; i++)
            {
                if (our.position.x + i >= 0 && our.position.x + i < boardSize.x)
                    createNewLaser(playerlasers, playerlaserscount, pli, Laser(Point(our.position.x + i, our.position.y - 1 - ((space / 2) - abs(i))), playerlaserspeed, playerlaserpower), playerLaserlane);
            }
            // createNewLaser(playerlasers, playerlaserscount, pli, Laser(Point(our.position.x, our.position.y - 1), playerlaserspeed, playerlaserpower), playerLaserlane);
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
        countFrame++;
    }
}

void filllasers(int *laser)
{
    for (int i = 0; i < boardSize.x; i++)
        laser[i] = -1;
}

void keyboardInput(Ship &our, int i, int &space)
{
    changecurser(Point(our.position.x, our.position.y));
    if (!kbhit())
        return;

    char input = getch();

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
        space = 1;
        break;
    }
    case 'w':
    {
        space = 3;
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
        if (lasers[i].power == 0)
        {
            // showlasers(lasers[i], true);
            continue;
        }
        showlasers(lasers[i], true);
        lasers[i].pos.y += lasers[i].speed;

        if (lasers[i].pos.y <= 0 || lasers[i].pos.y > boardSize.y)
        {
            lasers[i].power = 0;
            lasers[i].pos.y = 0;
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

        if (laserPos[lasers[i].pos.x] == i)
        {
            colorize(2, false);
            showlasers(lasers[i], false);
            colorize(1, true);
        }
        else
        {
            showlasers(lasers[i], false);
        }
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

//
// colorize
void colorize(int input, bool resetcolor)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (resetcolor)
    {
        SetConsoleTextAttribute(h, 7);
        return;
    }
    switch (input)
    {
    case 1:
    {
        SetConsoleTextAttribute(h, 7);

        break;
    }
    case 2:
    {
        SetConsoleTextAttribute(h, 9);
        break;
    }
    default:
    {
        break;
    }
    }
}

// enemy
void enemyInit(Wave &invaders, int model)
{
    srand(static_cast<unsigned int>(time(nullptr)));
    int time = 70 - (4 * model);
    Invader bot = Invader();
    invaders.version = model;
    invaders.pos = Point(0, 0);
    invaders.speed = 1 + model;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            invaders.NumberEachColumn[j] = i + 1;
            invaders.enemy[i][j].timeToLaser = rand() % (time) + 0;
            invaders.enemy[i][j].hp = 10 * (model + (4 - i));
            invaders.enemy[i][j].exp = 10 * (model + (4 - i));
        }
    }
}
void moveWave(Wave &invaders)
{

    int rightest = 0, leftest = 7;
    bool dontcheckright = false, dontcheckleft = false;
    for (int i = 0; i < 8; i++)
    {
        if (invaders.NumberEachColumn[i] == 0 && !dontcheckleft)
            rightest++;
        else
            dontcheckleft = true;
        if (invaders.NumberEachColumn[7 - i] == 0 && !dontcheckright)
            leftest--;
        else
            dontcheckright = true;
    }
    for(int i=0;i<8;i++)
    {
        cout << invaders.NumberEachColumn[i] << "  ";
    }
    exit(1);
    printWave(invaders, false, leftest);

    if (invaders.speed > 0)
    {
        if (invaders.pos.x + leftest + invaders.speed < boardSize.x) // if there is a space between each enemy space in every line this line should change
            invaders.pos.x += invaders.speed;
        else
        {
            invaders.pos.x = boardSize.x - leftest;
            invaders.speed *= -1;
            invaders.pos.y++;
        }
    }
    else
    {
        if (invaders.pos.x + rightest + invaders.speed >= 0) // if there is a space between each enemy space in every line this line should change
            invaders.pos.x += invaders.speed;
        else
        {
            invaders.pos.x = -rightest;
            invaders.speed *= -1;
            invaders.pos.y++;
        }
    }
    printWave(invaders, true, leftest);
}
void checkForFireLasers(Wave &invaders, int framesCount, Laser *&lasers, int &laserIndex, int &maxCount, int *laserPos)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (invaders.enemy[i][j].timeToLaser == framesCount)
            {
                Laser crate = Laser(Point(j + invaders.pos.x, i + invaders.pos.y + 1), (invaders.version + 1), (4 - i) * 10);
                createNewLaser(lasers, laserIndex, maxCount, crate, laserPos);
            }
        }
    }
}
void printWave(Wave &enemy, bool show, int &leftest)
{
    for (int i = 0; i < 4; i++)
    {
        changecurser(Point(enemy.pos.x + leftest, enemy.pos.y + i));
        for (int j = 0; j < 8; j++)
        {
            if (show)
                if (enemy.enemy[i][j].hp != 0)
                {
                    cout << "o";
                }
                else
                    cout << " ";
            else
                cout << " ";
        }
    }
}
