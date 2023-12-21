#include"funda.h"
#include"data.h"
#include"extension.h"

std::map<char,char>opposite;
int stage[maxn][maxn]= {0},width=15,height=15,stop=1,score=0;
food_prob foodprob= {0.6,0.3,0.1};
int difficulty=10,seed=time(nullptr),foodquan=1;
char direction,direction1;
snake *tail, *head;
bool gameover=true;

void _opposite() {
    opposite['a']='d';
    opposite['d']='a';
    opposite['w']='s';
    opposite['s']='w';
}

void _gotoxy(SHORT x, SHORT y) {
    COORD coord= {x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void _closecursor(bool ok) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    if(ok==false) {
        GetConsoleCursorInfo(handle, &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo(handle, &cursorInfo);
    }
    else {
        GetConsoleCursorInfo(handle, &cursorInfo);
        cursorInfo.bVisible = true;
        SetConsoleCursorInfo(handle, &cursorInfo);
    }
}

void _countdown() {
    printf("COUNTDOWN:\t");
    Sleep(1000);
    for(int i=3; i>0; --i) {
        printf("%d\b",i);
        Sleep(1000);
    }
    printf(" \nStart!");
    Sleep(1000);
}

bool _interface() {
    _closecursor(true);
    system("cls");
    printf("Welcome to SnakeFop!\n");
    printf("Enter \"g\" to begin and \"q\" to end.\n");
    while(true) {
        char foo;
        scanf(" %c",&foo);
        if(foo=='g') {
            //_countdown();
            _closecursor(false);
            system("cls");
            return true;
        }
        else if(foo=='q')exit(0);
        else {
            if(_other(foo))return false;
        }
    }
}

void _printfood(int width,int height) {
    double foo1=(rand()%1000)/1000.0;
    int ans=0,color=40;
    if(foo1>=0&&foo1<=foodprob.a)ans=1;
    else if(foo1>foodprob.a&&foo1<=foodprob.a+foodprob.b)ans=2;
    else if(foo1>foodprob.a+foodprob.b&&foo1<=foodprob.a+foodprob.b+foodprob.c)ans=3;
    switch(ans) {
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
    do {
        x=rand()%(width-2)+1;
        y=rand()%(height-2)+1;
    } while(stage[y][x]!=0);
    stage[y][x]=ans;
    _gotoxy(x,y);
    printf("\e[%dm@\e[0m",color);
}

void _printstage(int width,int height) {
    stop=1;
    gameover=true;
    score=0;
    memset(stage,0,sizeof(stage));
    for(int i=0; i<width+2; ++i) {
        for(int j=0; j<height+2; ++j) {
            if(i==0||i==width+1) {
                printf("—");
                stage[i][j]=-1;
            }
            else if(j==0||j==height+1) {
                printf("|");
                stage[i][j]=-1;
            }
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
    for(int i=width/2-2; i<width/2+2; ++i)stage[height/2+1][i]=-1;
    snake *p,*q;
    p=new snake;
    p->coor= {width/2-2,height/2+1};
    p->next=NULL;
    tail=p;
    q=p;
    for(int i=1; i<4; ++i) {
        p=new snake;
        p->coor= {width/2-2+i,height/2+1};
        p->next=NULL;
        q->next=p;
        q=p;
    }
    head=q;
    head->next=NULL;
    for(int i=0; i<foodquan; ++i)_printfood(width,height);
}

void _refresh() {
    while(true) {
        if(!stop)continue;
        else if(stop==-1)break;
        if(direction1==opposite[direction])direction1=direction;
        else direction=direction1;
        coordinate foo= {head->coor.x,head->coor.y};
        switch(direction) {
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
        if(stage[foo.y][foo.x]>0) {
            increase=true;
            score+=stage[foo.y][foo.x];
            stage[foo.y][foo.x]=0;
            _gotoxy(width+12,0);
            printf("\t%d",score);
        }
        if(!increase) {
            stage[tail->coor.y][tail->coor.x]=0;
            if(stage[foo.y][foo.x]<0) {
                gameover=false;
                snake *curr=tail;
                while(curr->next!=NULL) {
                    _gotoxy(curr->coor.x,curr->coor.y);
                    printf("\e[41m*\e[0m");
                    curr=curr->next;
                }
                _gotoxy(curr->coor.x,curr->coor.y);
                printf("\e[41m#\e[0m");
                Sleep(1000);
                system("cls");
                printf("Game Over! Your score is %d. \nPress any key to return to the menu.",score);
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
        if(increase)_printfood(width,height);
        Sleep((double)1000/difficulty);
    }
}

void _getcontrol() {
    while(true) {
        if(!gameover)break;
        char foo=getch();
        if(foo==' ')stop=(stop+1)%2;
        else if(foo=='a'||foo=='s'||foo=='d'||foo=='w')direction1=foo;
        else if(foo=='q')
            if(!stop) {
                system("cls");
                stop=-1;
                break;
            }
    }
}
