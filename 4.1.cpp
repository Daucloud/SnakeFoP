#include <iostream>
#include <cstdio>
#include <conio.h>
#include <cstdlib>
#include <windows.h>
#include <ctime>

using namespace std;

const int maxn = 30;

struct coordinate {
   int x;
   int y;
};

struct snake {
   coordinate coor;
   snake* next;
};

char direction, direction1;

int stage[maxn][maxn] = {0};
int width = 15, height = 15;

snake *tail, *head;

int stop = 1;
bool gameover = true;
int score = 0;

map<char, char> opposite;

void _opposite() {
   opposite['w'] = 's';
   opposite['s'] = 'w';
   opposite['a'] = 'd';
   opposite['d'] = 'a';
}

void _gotoxy(SHORT x, SHORT y) {
   COORD pos = {x, y};
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void _CloseCursor(bool ok) {
   HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO cci;
   GetConsoleCursorInfo(h, &cci);
   cci.bVisible = !ok;
   SetConsoleCursorInfo(h, &cci);
}

void _interface() {
   system("cls");
   for (int i = 0; i < height + 2; i++) {
       cout << "#";
   }
   cout << endl;
   for (int i = 0; i < height; i++) {
       for (int j = 0; j < width; j++) {
           if (i == head->coor.y && j == head->coor.x) {
               cout << "O";
           } else if (i == tail->coor.y && j == tail->coor.x) {
               cout << "*";
           } else if (stage[i][j] == 1) {
               cout << ".";
           } else {
               cout << " ";
           }
       }
       cout << endl;
   }
   for (int i = 0; i < height + 2; i++) {
       cout << "#";
   }
   cout << endl;
   cout << "Score: " << score << endl;
   cout << "Steps: " << (int)sqrt(score) << endl;
}

void _PrintFood(int width, int height) {
   while (true) {
       int x = rand() % width;
       int y = rand() % height;
       if (stage[y][x] == 0) {
           stage[y][x] = 2;
           break;
       }
   }
}

void _PrintStage(int width, int height) {
   for (int i = 0; i < height; i++) {
       for (int j = 0; j < width; j++) {
           if (stage[i][j] == 1) {
               cout << "*";
           } else if (stage[i][j] == 2) {
               cout << ".";
           } else {
               cout << "#";
           }
       }
       cout << endl;
   }
}

void _refresh() {
   _CloseCursor(false);
   _PrintStage(width, height);
   _gotoxy(head->coor.x + 1, head->coor.y + 1);
   cout << opposite[direction];
   _CloseCursor(true);
}

void _GetControl() {
   if (_kbhit()) {
       direction = _getch();
       if (direction == 'v') {
           stop = 0;
       }
