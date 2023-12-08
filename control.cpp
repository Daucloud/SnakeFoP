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
char direction,direction1;
snake *tail, *head;
int stop=1;
bool gameover=true;
int score=0;

//同方向和反方向键值对的建立
void _opposite(){
    opposite['a']='d';
    opposite['d']='a';
    opposite['w']='s';
    opposite['s']='w';
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

//主界面&&游戏开始
void _interface(){
    printf("Welcome to SnakeFop!\n");
    printf("Enter \"g\" to begin and \"q\" to end.\n");
    while(true){
        char foo;
        scanf(" %c",&foo);
        if(foo=='g'){
            /*printf("COUNTDOWN:\t");
            Sleep(500);
            for(int i=3;i>0;--i){
            printf("%d\b",i);
            Sleep(500);
            }
            printf(" \nStart!");
            Sleep(500);*/
            system("cls");
            return;
        }
        else if(foo=='q')exit(0);
        else printf("Illegal Input.Please enter \"g\" or \"q\".\n");
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
    x=rand()%(width-2)+1;
    y=rand()%(height-2)+1;
    if(stage[y][x]>=0)break;
    }
    stage[y][x]=ans;
    _gotoxy(x,y);
    printf("\e[%dm@\e[0m",color);
}

//地图打印&&蛇的生成
void _PrintStage(int width,int height){
    stop=1;
    gameover=true;
    score=0;
    memset(stage,0,sizeof(stage));
    for(int i=0;i<width+2;++i){
        for(int j=0;j<height+2;++j){
            if(i==0||i==width+1){printf("—");stage[i][j]=-1;}
            else if(j==0||j==height+1){printf("|");stage[i][j]=-1;}
            else printf(" ");
        }
        printf("\n");
    }
    _gotoxy(width+6,0);
    printf("score:\t0");
    _gotoxy(width/2-2,height/2+1);
    printf("\e[42m***#\e[0m");
    direction='d';
    direction1='d';
    for(int i=width/2-2;i<width/2+2;++i)stage[height/2+1][i]=-1;
    snake *p,*q;
    p=new snake;
    p->coor={width/2-2,height/2+1};
    p->next=NULL;
    tail=p;
    q=p;
    for(int i=1;i<4;++i){
        p=new snake;
        p->coor={width/2-2+i,height/2+1};
        p->next=NULL;
        q->next=p;
        q=p;
    }
    head=q;
    head->next=NULL;
    _PrintFood(width,height);
}

//控制台的刷新
void _refresh(){
    while(true){
    if(!stop)continue;
    else if(stop==-1)break;
    if(direction1==opposite[direction])direction1=direction;
    else direction=direction1;
    coordinate foo={head->coor.x,head->coor.y};
    switch(direction){
        case 'a':
            foo.x-=1;
            break;
        case 'd':
            foo.x+=1;
            break;
        case 's':
            foo.y+=1;
            break;
        case 'w':
            foo.y-=1;
            break;
    }
    bool increase=false;
    if(stage[foo.y][foo.x]>0){
        increase=true;
        score+=stage[foo.y][foo.x];
        stage[foo.y][foo.x]=0;
        _gotoxy(width+12,0);
        printf("\t%d",score);
    }
    if(!increase){
    stage[tail->coor.y][tail->coor.x]=0;
    if(stage[foo.y][foo.x]<0){
        gameover=false;
        snake *curr=tail;
        while(curr->next!=NULL){
            _gotoxy(curr->coor.x,curr->coor.y);
            printf("\e[41m*\e[0m");
            curr=curr->next;
        }
        _gotoxy(curr->coor.x,curr->coor.y);
        printf("\e[41m#\e[0m");
        _gotoxy(width+6,height/2+1);
        printf("\e[41mGame Over!Press any key to return the main menu.\e[0m");
        system("cls");
        break;
    }
    _gotoxy(tail->coor.x,tail->coor.y);
    printf(" ");
    snake *del=tail;
    tail=tail->next;
    delete del;
    }
    _gotoxy(head->coor.x,head->coor.y);
    printf("\e[42m*\e[0m");
    snake *New=new snake;
    New->coor=foo;
    New->next=NULL;
    head->next=New;
    head=New;
    _gotoxy(head->coor.x,head->coor.y);
    printf("\e[42m#\e[0m");
    stage[New->coor.y][New->coor.x]=-1;
    if(increase)_PrintFood(width,height);
    Sleep(500);
    }
}

//根据用户输入更新移动方向
void _GetControl(){
    while(true){
        if(!gameover)break;
        char foo=getch();
        if(foo==' ')stop=(stop+1)%2;
        else if(foo=='a'||foo=='s'||foo=='d'||foo=='w')direction1=foo;
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
    std::thread GetControl(_GetControl);
    std::thread refresh(_refresh);
    GetControl.join();
    refresh.join();
    }
}
