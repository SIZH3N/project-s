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

void changecurser(Point pos);
void colorize(bool resetcolor);
void print(Botton click, bool resetcolor);

const Point matsize = Point(5, 5);
int main()
{
    string data[5]{""};
    system("cls");
    int mat[matsize.y][matsize.x];
    int getIndex[matsize.y]{0};

    /*for (int i = 0; i < matsize.y; i++)
    {
        for (int j = 0; j < matsize.x; j++)
        {
            mat[i][j] = j;
            cout << mat[i][j] << " ";
        }
        cout << endl;
    }*/

    // botton
    Botton items[5];
    for (int i = 0; i < 5; i++)
    {
        items[i].size = Point(7, 3);
        items[i].start.x = 0;
        items[i].start.y = (i * (items[i].size.y + 1));
        // items[i].text = data[i];
        items[i].text = " ";
    }
    // sample botton for hover
    Botton sample;
    sample.size = Point(3, 3);
    sample.start = Point(9, 0);

    for (int i = 0; i < 5; i++)
    {
        print(items[i], i);
        for (int j = 0; j < 5; j++)
        {
            Botton output = sample;
            output.start.x += j * (output.size.x + 1);
            output.start.y = items[i].start.y;
            print(output, false);
        }
    }

    Point curentPos = Point(0, 0);
    changecurser(Point(curentPos.x * 2, curentPos.y));
    while ((true))
    {
        char input = getch();
        switch (input)
        {
        case 'A':
        case 'a':
        {
            curentPos.x += matsize.x - 1;
            curentPos.x = curentPos.x % matsize.x;
            break;
        }
        case 'D':
        case 'd':
        {
            curentPos.x += 1;
            curentPos.x = curentPos.x % matsize.x;
            break;
        }
        case 'W':
        case 'w':
        {
            if (curentPos.x == 0)
            {
                curentPos.y += matsize.y - 1;
                curentPos.y = curentPos.y % matsize.y;
            }
            break;
        }
        case 'S':
        case 's':
        {
            if (curentPos.x == 0)
            {
                curentPos.y += 1;
                curentPos.y = curentPos.y % matsize.y;
            }
            break;
        }
        case ' ':
        {
            if (getIndex[curentPos.y] < mat[curentPos.y][curentPos.x])
            {
                for (int i = 1; i <= curentPos.x; i++)
                {
                    changecurser(Point(i * 2, curentPos.y));
                    cout << i + 1;
                }
            }
            else if (getIndex[curentPos.y] > mat[curentPos.y][curentPos.x])
            {
                for (int i = curentPos.x + 1; i <= getIndex[curentPos.y]; i++)
                {
                    changecurser(Point(i * 2, curentPos.y));
                    cout << i;
                }
            }
            getIndex[curentPos.y] = mat[curentPos.y][curentPos.x];
            break;
        }
        default:
        {
            cout << "  9 " << input;
            return 0;
            break;
        }
        }

        changecurser(Point(curentPos.x * 2, curentPos.y));
    }
    return 0;
}

void changecurser(Point pos)
{
    // HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD screen;
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    screen.X = pos.x;
    screen.Y = pos.y;
    SetConsoleCursorPosition(hOutput, screen);
    // CloseHandle(hOutput);
}

void colorize(bool resetcolor)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (resetcolor)
        SetConsoleTextAttribute(h, 650);
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
                colorize(true);
                cout << " ";
            }
        }
    }
    changecurser(Point(click.start.x + 1, click.start.y + 1));
    colorize(true);
    cout << click.text;
}
