#include <SFML/Graphics.hpp>
#include <iostream>
#include <conio.h>
#include <SFML/Window.hpp>
#include <time.h>
#include<thread>
#include<Windows.h>
#include <SFML/Audio.hpp>
#include <fstream>
#include<string>
using namespace sf;
using namespace std;

void HighScoreRecord(string lvl[], string scorename[][5], int score[][5])
{
    ifstream fin("highScore.txt");
    int i = 0;
    while (fin.eof() == 0)
    {
        fin >> lvl[i];
        for (int j = 0; j < 5; j++)
        {
            fin >> scorename[i][j];
            fin >> score[i][j];
        }
        i++;
    }
}
void DisplayHighScoreRecord(string lvl[], string scorename[][5], int score[][5])
{
    cout << "-----------------------------| High Score |-----------------------------------------" << endl;
    for (int i = 0; i < 3; i++)
    {
        cout << " - " << lvl[i] << " Mode - " << endl;
        for (int j = 0; j < 2; j++)
        {
            cout << "        --->   " << scorename[i][j] << "   " << score[i][j] << endl;
        }
        cout << "-------------------------------------------------------------------------------" << endl;
    }
}
class grid {
    int rows;
    int colums;
public:
    grid() {
        rows = 0;
        colums = 0;
    }
    grid(int r, int c)
    {
        rows = r;
        colums = c;
    }
    int getrow() { return rows; }
    int getcol() { return colums; }
};
class cell :public grid {
    int x_co;
    int y_co;
    int value;
public:
    cell() {}
    void setcell(int x, int y)
    {
        x_co = x;
        y_co = y;
    }
    void setval(int v) { value = v; }
    int getvale() { return value; }
    int getx_co() { return x_co; }
    int gety_co() { return y_co; }
    void operator=(cell& c1)
    {
        x_co = c1.x_co;
        y_co = c1.y_co;
        value = c1.value;
    }
    virtual ~cell()
    {
        x_co = 0;
        y_co = 0;

    }
};
class mine :public cell {
    int nofmines;
public:
    mine() { nofmines = 0; }
    void setmines(int m) { nofmines = m; }
    int getmine() { return nofmines; }
    void gridmines(cell**& bgrid, int total, grid& g)
    {
        int r1 = 0;
        int c1 = 0;
        for (int i = 1; i <= total; i++)
        {
            r1 = rand() % g.getrow();
            c1 = rand() % g.getcol();
            bgrid[r1][c1].setval(9);
        }
    }
    virtual ~mine() {

    }
};
class flags {
    int noflags;
public:
    flags() { noflags = 0; }
    void setflags(int t)
    {
        noflags = t;
    }
    int getflag() { return noflags; }
};
void expose(int x, int y, cell**& bgrid, cell**& sgrid, grid& g)
{
    sgrid[x][y].setval(bgrid[x][y].getvale());
    if (x + 1 < g.getrow())
    {
        if (sgrid[x + 1][y].getvale() == 10 && bgrid[x + 1][y].getvale() == 0)
            expose(x + 1, y, bgrid, sgrid, g);
        else
            sgrid[x + 1][y].setval(bgrid[x + 1][y].getvale());
    }
    if (x - 1 >= 0 && y - 1 >= 0)
    {
        if (sgrid[x - 1][y - 1].getvale() == 10 && bgrid[x - 1][y - 1].getvale() == 0)
            expose(x - 1, y - 1, bgrid, sgrid, g);
        else
            sgrid[x - 1][y - 1].setval(bgrid[x - 1][y - 1].getvale());
    }
    if (x - 1 >= 0)
    {
        if (sgrid[x - 1][y].getvale() == 10 && bgrid[x - 1][y].getvale() == 0)
            expose(x - 1, y, bgrid, sgrid, g);
        else
            sgrid[x - 1][y].setval(bgrid[x - 1][y].getvale());
    }
    if (y - 1 >= 0)
    {
        if (sgrid[x][y - 1].getvale() == 10 && bgrid[x][y - 1].getvale() == 0)
            expose(x, y - 1, bgrid, sgrid, g);
        else
            sgrid[x][y - 1].setval(bgrid[x][y - 1].getvale());
    }
    if ((x + 1) < g.getrow() && (y - 1) >= 0)
    {
        if (sgrid[x + 1][y - 1].getvale() == 10 && bgrid[x + 1][y - 1].getvale() == 0)
            expose(x + 1, y - 1, bgrid, sgrid, g);
        else
            sgrid[x + 1][y - 1].setval(bgrid[x + 1][y - 1].getvale());
    }
    if ((x + 1) < g.getrow() && (y + 1) < g.getcol())
    {
        if (sgrid[x + 1][y + 1].getvale() == 10 && bgrid[x + 1][y + 1].getvale() == 0)
            expose(x + 1, y + 1, bgrid, sgrid, g);
        else
            sgrid[x + 1][y + 1].setval(bgrid[x + 1][y + 1].getvale());
    }
    if (x - 1 >= 0 && y + 1 < g.getcol())
    {
        if (sgrid[x - 1][y + 1].getvale() == 10 && bgrid[x - 1][y + 1].getvale() == 0)
            expose(x - 1, y + 1, bgrid, sgrid, g);
        else
            sgrid[x - 1][y + 1].setval(bgrid[x - 1][y + 1].getvale());
    }
    if (y + 1 < g.getcol())
    {
        if (sgrid[x][y + 1].getvale() == 10 && bgrid[x][y + 1].getvale() == 0)
            expose(x, y + 1, bgrid, sgrid, g);
        else
            sgrid[x][y + 1].setval(bgrid[x][y + 1].getvale());
    }
}
void set_grid(grid& g, cell**& bgrid)
{
    for (int i = 0; i < g.getrow(); i++)
        for (int j = 0; j < g.getcol(); j++) {
            int n = 0;//num of mine
            if (bgrid[i][j].getvale() == 9) continue;
            if (i + 1 < g.getrow())
                if (bgrid[i + 1][j].getvale() == 9)
                    n++;
            if (j + 1 < g.getcol())
                if (bgrid[i][j + 1].getvale() == 9)
                    n++;
            if (i - 1 >= 0)
                if (bgrid[i - 1][j].getvale() == 9)
                    n++;
            if (j - 1 >= 0)
                if (bgrid[i][j - 1].getvale() == 9)
                    n++;
            if (i + 1 < g.getrow() && j + 1 < g.getcol())
                if (bgrid[i + 1][j + 1].getvale() == 9)
                    n++;
            if (i - 1 >= 0 && j - 1 >= 0)
                if (bgrid[i - 1][j - 1].getvale() == 9)
                    n++;
            if (i - 1 >= 0 && j + 1 < g.getcol())
                if (bgrid[i - 1][j + 1].getvale() == 9)
                    n++;
            if (i + 1 < g.getrow() && j - 1 >= 0)
                if (bgrid[i + 1][j - 1].getvale() == 9)
                    n++;
            bgrid[i][j].setval(n);
        }
}
bool finish(cell**& sgrid, cell**& bgrid, grid& g)
{
    bool flag = true;
    int count = 0;
    for (int i = 0; i < g.getrow(); i++) {
        for (int j = 0; j < g.getcol(); j++)
        {
            if (sgrid[i][j].getvale() == 11 && bgrid[i][j].getvale() != 9)
                flag = false;
        }
    }
    return flag;
}
char menu()
{
    char choice = 0;
    cout << "-----------------MINESWEEPER-------------\n\n";
    cout << "1-  EASY\n\n";
    cout << "2-  MEDIUM\n\n";
    cout << "3-  HARD\n\n";
    choice = _getch();
    return choice;
}
void timer(int& sec, int& control)
{
    while (control != 0)
    {
        Sleep(1000);
        sec++;
    }
}
int main()
{
    char choice = menu();
    mine tmines;
    flags totalflag;
    int r = 0, c = 0;
    if (choice == '1')
    {
        r = 9;
        c = 9;
        tmines.setmines(10);
        totalflag.setflags(10);
    }
    if (choice == '2')
    {
        r = 16;
        c = 16;
        tmines.setmines(40);
        totalflag.setflags(40);
    }
    if (choice == '3')
    {
        r = 30;
        c = 16;
        tmines.setmines(99);
        totalflag.setflags(99);
    }
    grid g(r, c);
    cell** bgrid = nullptr;
    cell** sgrid = nullptr;
    srand(time(0));
    RenderWindow app(VideoMode(g.getrow() * 50, g.getcol() * 50), "Minesweeper");
    int w = 39.5;
    Texture t;
    t.loadFromFile("image/tiles6.JPG");
    Sprite s(t);
    Font f;
    int sec = 0, control = 1;
    thread t1(&timer, ref(sec), ref(control));
    t1.detach();
    f.loadFromFile("SunnyspellsRegular-MV9ze.otf");
    Text time;
    time.setFont(f);
    int flagtime = 0;

    sgrid = new cell * [g.getrow()];
    bgrid = new cell * [g.getrow()];
    for (int j = 0; j < g.getrow(); j++)
    {
        sgrid[j] = new cell[g.getcol()];
        bgrid[j] = new cell[g.getcol()];
    }
    tmines.gridmines(bgrid, tmines.getmine(), g);
    for (int i = 0; i < g.getrow(); i++)
    {
        for (int j = 0; j < g.getcol(); j++)
        {
            sgrid[i][j].setcell(i, j);
            bgrid[i][j].setcell(i, j);
            sgrid[i][j].setval(10);
            if (bgrid[i][j].getvale() != 9)
                bgrid[i][j].setval(0);
        }
    }
    set_grid(g, bgrid);
    int count = 0;
    while (app.isOpen())
    {
        Vector2i pos = Mouse::getPosition(app);
        int x = (pos.x - 75) / w;
        int y = (pos.y - 75) / w;
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();
            if (e.type == Event::MouseButtonPressed)
                if (e.key.code == Mouse::Left)
                {
                    if (flagtime == 0)
                    {
                        flagtime = 1;
                        sec = 0;
                    }
                    if (x >= 0 && x < g.getrow() && y >= 0 && y < g.getcol())
                    {
                        sgrid[x][y] = bgrid[x][y];
                        if (sgrid[x][y].getvale() == 0)
                            expose(x, y, bgrid, sgrid, g);
                    }
                }
                else if (e.key.code == Mouse::Right)
                {
                    if (count < totalflag.getflag() && sgrid[x][y].getvale() != 11)
                    {
                        sgrid[x][y].setval(11);
                        count++;
                    }
                    else
                    {
                        if (sgrid[x][y].getvale() == 11 && count >= 0)
                        {
                            count--;
                            sgrid[x][y].setval(10);
                        }
                    }
                    if (count == totalflag.getflag())
                    {
                        app.close();
                        if (finish(sgrid, bgrid, g))
                        {
                            cout << "        WINNER WINNER CHICKER DINNER";
                            cout << "time: " << sec;
                        }
                        else {
                            cout << "         YOU LOST!!!!";
                        }
                    }
                }
        }
        app.clear(Color::Black);
        for (int i = 0; i < g.getrow(); i++)
            for (int j = 0; j < g.getcol(); j++) {

                if (x >= 0 && x < g.getrow() && y >= 0 && y < g.getcol())
                {
                    if (sgrid[x][y].getvale() == 9)
                    {
                        sgrid[i][j].setval(bgrid[i][j].getvale());
                        control = 0;
                    }
                }
                s.setTextureRect(IntRect(sgrid[i][j].getvale() * w, 0, w, w));
                s.setPosition((i * w) + 75, (j * w) + 75);
                app.draw(s);
            }
        if (flagtime == 1)
            time.setString(to_string(sec));
        else
            time.setString('0');
        time.setCharacterSize(50);
        time.setFillColor(Color::Blue);
        app.draw(time);
        app.display();
    }
    int score[3][5];
    string level[3], scorename[3][5];
    HighScoreRecord(level, scorename, score);
    DisplayHighScoreRecord(level, scorename, score);

}