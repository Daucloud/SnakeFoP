#include<iostream>
#include<cstdio>
#include<conio.h>
#include <cstdlib>
#include<windows.h> 
#include<ctime>
#include<map>
#include<thread>

const int maxn=30;

struct coordinate{
    int x;
    int y;
};

//蛇的位置用链表进行存储
struct snake{
    coordinate coor;
    snake* next;
};

std::map<char,char>opposite;

int stage[maxn][maxn]={0};
int width=15,height=15;
char direction;
snake *tail, *head;
auto current=time(nullptr);
int stop=1;

//同方向和反方向键值对的建立
void _opposite(){
    opposite['a']='d';
    opposite['d']='a';
    opposite['w']='s';
    opposite['s']='w';
}

//主界面&&游戏开始
void _interface(){
    printf("Welcome to SnakeFop!\n");
    printf("Enter \"g\" to begin and \"q\" to end.\n");
    while(true){
        char foo;
        scanf(" %c",&foo);
        if(foo=='g'){
            printf("COUNTDOWN:\t");
            Sleep(1000);
            for(int i=3;i>0;--i){
            printf("%d\b",i);
            Sleep(1000);
            printf(" \nStart!");
            Sleep(1000);
            system("cls");
            return;
            }
        }
        else if(foo=='q')exit(0);
        else printf("Illegal Input.Please enter \"g\" or \"q\".\n");
    }
}

//光标移动
void _gotoxy(SHORT x, SHORT y) {
    COORD coord={x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//关闭光标显示
void _CloseCursor(bool ok){
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    if(ok==false){
        GetConsoleCursorInfo(handle, &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo(handle, &cursorInfo);
    }
    else{
        cursorInfo.bVisible = true;
        SetConsoleCursorInfo(handle, &cursorInfo);
    }
}

//食物生成
void _PrintFood(int width,int height){
    int foo1=rand()%10,ans=0,color=40;
    if(foo1>=0&&foo1<=5)ans=1;
    else if(foo1>=6&&foo1<=8)ans=2;
    else if(foo1==9)ans=3;
    switch(ans){
        case 1: 
            color=44;
            break;
        case 2:
            color=45;
            break;
        case 3:
            color=43;
            break;
    }
    int x,y;
    while(true){
    x=rand()%(width)+1;
    y=rand()%(height)+1;
    if(stage[x][y]>=0)break;
    }
    stage[x][y]=ans;
    _gotoxy(x,y);
    printf("\e[%dm@\e[0m",color);
}

//地图打印&&蛇的生成
void _PrintStage(int width,int height){
    for(int i=0;i<width+2;++i){
        for(int j=0;j<height+2;++j){
            if(i==0||i==width+1)printf("—");
            else if(j==0||j==height+1)printf("|");
            else printf(" ");
        }
        printf("\n");
    }
    _gotoxy(width/2-3,height/2+1);
    printf("\e[42m***#\e[0m");
    direction='d';
    for(int i=width/2-3;i<width+1;++i)stage[i][height/2+1]=-1;
    snake *p,*q;
    p=new snake;
    p->coor={width/2-3,height/2+1};
    p->next=NULL;
    tail=p;
    q=p;
    for(int i=1;i<4;++i){
        p=new snake;
        p->coor={width/2-3+i,height/2+1};
        p->next=NULL;
        q->next=p;
        q=p;
    }
    head=q;
    head->next=NULL;
    _PrintFood(width,height);
}

//

//控制台的刷新
void _refresh(){
    while(true){
    if(!stop)continue;
    else if(stop==-1)break;
    _gotoxy(tail->coor.x,tail->coor.y);
    printf(" ");
    _gotoxy(head->coor.x,head->coor.y);
    printf("\e[42m*\e[0m");
    snake *del=tail;
    tail=tail->next;
    delete del;
    snake *New=new snake;
    New->coor=head->coor;
    New->next=NULL;
    switch(direction){
        case 'a':
            New->coor.x-=1;
            break;
        case 'd':
            New->coor.x+=1;
            break;
        case 's':
            New->coor.y+=1;
            break;
        case 'w':
            New->coor.y-=1;
            break;
    }
    head->next=New;
    head=New;
    _gotoxy(head->coor.x,head->coor.y);
    printf("\e[42m#\e[0m");
    Sleep(1000);
    }
}

//根据用户输入更新移动方向
void _GetControl(){
    while(true){
        char foo=getch();
        if(foo==' ')stop=(stop+1)%2;
        else if((foo=='a'||foo=='s'||foo=='d'||foo=='w')&&foo!=opposite[direction])direction=foo;
        else if(foo=='q')
            if(!stop){
                system("cls");
                stop=-1;
                break;
            }
    }
}

int main(){
    _opposite();
    srand(time(nullptr));
    SetConsoleOutputCP(CP_UTF8);
    _CloseCursor(false);
    while(true){
    _interface();
    _PrintStage(15,15);
    current=time(nullptr);
    std::thread GetControl(_GetControl);
    std::thread refresh(_refresh);
    GetControl.join();
    refresh.join();
    }
}