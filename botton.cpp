#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;

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

struct Botton
{
    Point start;
    Point size;
    string text;
};

struct Field
{
    Botton background;
    Botton click;
};

void changecurser(Point position);
void colorize(bool resetcolor);

bool endgame();

void ifon(Botton click[]);
void ifon(Field banner[]);
void print(Field banner, bool resetcolor = false);
void print(Botton click, bool resetcolor = false);
void printAll(Botton all[], int number);

int main()
{
    system("cls");
    colorize(true);
    Botton b;
    Botton all[5];

    b.size = Point(8, 3);
    b.start = Point(1, 2);
    b.text = "hi";

    for (int i = 0; i < 5; i++)
    {
        all[i].size.x = b.size.x;
        all[i].size.y = b.size.y;
        all[i].start.x = i * b.size.x + i;
        all[i].start.y = 0;
        all[i].text = b.text;
    }
    int startingpoint = 0;
    printAll(all, startingpoint);
    while (true)
    {
        char inpt = getch();
        switch (inpt)
        {
        case 'D':
        case 'd':
        {
            print(all[startingpoint], true);
            startingpoint++;
            startingpoint = startingpoint % 5;
            print(all[startingpoint], false);
            break;
        }
        case 'A':
        case 'a':
        {
            print(all[startingpoint], true);
            startingpoint--;
            startingpoint += 5;
            startingpoint = startingpoint % 5;
            print(all[startingpoint], false);
            break;
        }
        default:
            return 0;
        }
        colorize(true);
    }

    // colorize(true);
    getch();
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

void colorize(bool resetcolor)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (resetcolor)
        SetConsoleTextAttribute(h, 7);
    else
        SetConsoleTextAttribute(h, 281);
}

void print(Botton click, bool resetcolor)
{
    for (int i = click.start.y; i < click.start.y + click.size.y; i++)
    {
        int j = click.start.x;
        changecurser(Point(j, i));
        for (; j < click.size.x + click.start.x; j++)
        {
            if ((i == click.start.y || i == click.start.y + click.size.y - 1) || (j == click.start.x || j == click.start.x + click.size.x - 1))
            {
                colorize(resetcolor);
                cout << " ";
            }
            else
            {
                //colorize(true);
                cout << " ";
            }
        }
    }
    changecurser(Point(click.start.x + 1, click.start.y + 1));
    colorize(true);
    cout << click.text;
}

void printAll(Botton all[], int number)
{
    for (int i = 0; i < 5; i++)
    {
        if (number == i)
        {
            print(all[i], false);
            colorize(true);
        }
        else
            print(all[i], true);
    }
}
bool endgame()
{
    return false;
}