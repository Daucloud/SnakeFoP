#include<iostream>
#include<cstdio>
#include<conio.h>
#include <cstdlib>
#include<windows.h>
#include<ctime>
#include<map>
#include<thread>
#include<fstream>
#include<string>

const int maxn=30;

struct coordinate {
    int x;
    int y;
};

//蛇的位置用链表进行存储
struct snake {
    coordinate coor;
    snake* next;
};
//方便存储食物概率
struct food_prob {
    double a,b,c;
};

std::map<char,char>opposite;
int stage[maxn][maxn]= {0},width=15,height=15,stop=1,score=0;
food_prob foodprob= {0.6,0.3,0.1};
int difficulty=1,seed=time(nullptr),foodquan=1;
char direction,direction1;
snake *tail, *head;
bool gameover=true;

//整数转换
int changetoint(const std::string& str);
//小数转换
double changetodouble(const std::string& str);
//判断配置文件输入合法性
bool validconfig(int c1,int c2,int c3,double c4,double c5,double c6);
//同方向和反方向键值对的建立
void _opposite();
//光标移动
void _gotoxy(SHORT x, SHORT y);
//关闭光标显示
void _CloseCursor(bool ok);
//主界面&&游戏开始
void _interface();
//食物生成
void _PrintFood(int width,int height);
//地图打印&&蛇的生成
void _PrintStage(int width,int height);
//控制台的刷新
void _refresh();
//根据用户输入更新移动方向
void _GetControl();

int main() {
    _opposite();
    srand(seed);
    SetConsoleOutputCP(CP_UTF8);
    _CloseCursor(false);
    while(true) {
        _interface();
        _PrintStage(15,15);
        std::thread GetControl(_GetControl);
        std::thread refresh(_refresh);
        GetControl.join();
        refresh.join();
    }
}

void _opposite() {
    opposite['a']='d';
    opposite['d']='a';
    opposite['w']='s';
    opposite['s']='w';
}

int changetoint(const std::string& str) {
    try {
        return std::stoi(str);
    } catch (const std::exception&) {
        return -2;
    }
}

double changetodouble(const std::string& str) {
    try {
        return std::stod(str);
    } catch (const std::exception&) {
        return -2;
    }
}

bool validconfig(int c1,int c2,int c3,double c4,double c5,double c6) {
    if(!(c1>=1&&c1<=10))return false;
    if(c2<-1)return false;
    if(!(c3>=1&&c3<=5))return false;
    if(!(c4>=0&&c4<=1))return false;
    if(!(c5>=0&&c5<=1))return false;
    if(!(c6>=0&&c6<=1))return false;
    if(abs(c4+c5+c6-1)>1e-3)return false;
    return true;
}

void _gotoxy(SHORT x, SHORT y) {
    COORD coord= {x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void _CloseCursor(bool ok) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    if(ok==false) {
        GetConsoleCursorInfo(handle, &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo(handle, &cursorInfo);
    }
    else {
        cursorInfo.bVisible = true;
        SetConsoleCursorInfo(handle, &cursorInfo);
    }
}

void _interface() {
    printf("Welcome to SnakeFop!\n");
    printf("Enter \"g\" to begin and \"q\" to end.\n");
    while(true) {
        char foo;
        scanf(" %c",&foo);
        if(foo=='g') {
            /*printf("COUNTDOWN:\t");
            Sleep(1000/);
            for(int i=3;i>0;--i){
            printf("%d\b",i);
            Sleep(1000/);
            }
            printf(" \nStart!");
            Sleep(1000);*/
            system("cls");
            return;
        }
        else if(foo=='i') {
            printf("Please enter the name of your new configuration file:\n");
            std::string foo;
label:
            std::cin>>foo;
            if(foo=="q")continue;
            std::string foo1="./config/"+foo;
            std::ofstream file(foo1);
            printf("Please enter the difficulty,seed,food quantity and food probability in the following four lines respectively:\n");
            std::string s1,s2,s3,s4,s5,s6;
            std::cin>>s1>>s2>>s3>>s4>>s5>>s6;
            int c1=changetoint(s1);
            int c2=changetoint(s2);
            int c3=changetoint(s3);
            double c4=changetodouble(s4);
            double c5=changetodouble(s5);
            double c6=changetodouble(s6);
            if(!validconfig(c1,c2,c3,c4,c5,c6)) {
                printf("Invalid configuration! Please enter again.\n");
                goto label;
            }
            file<<c1<<"\n"<<c2<<"\n"<<c3<<"\n"<<c4<<" "<<c5<<" "<<c6<<"\n";
            file.close();
            printf("Configuration created successfully!\n");
            printf("Enter \"g\" to begin and \"q\" to end.\n");
        }
        else if(foo=='u') {
            printf("Please enter the name of configuration:\n");
            std::string foo;
label2:
            std::cin>>foo;
            if(foo=="q")continue;
            std::string name="./config/"+foo;
            std::ifstream file(name);
            if(!file) {
                printf("No such configuration! Please enter again:\n");
                goto label2;
            }
            file>>difficulty>>seed>>foodquan>>foodprob.a>>foodprob.b>>foodprob.c;
            file.close();
            printf("Configuration loaded successfully!\n");
            printf("Enter \"g\" to begin and \"q\" to end.\n");
        }
        else if(foo=='q')exit(0);
        else printf("Illegal Input.Please enter \"g\" or \"q\".\n");
    }
}

void _PrintFood(int width,int height) {
    double foo1=(rand()%1000)/1000;
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

void _PrintStage(int width,int height) {
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
    for(int i=0; i<foodquan; ++i)_PrintFood(width,height);
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
        Sleep((double)1000/difficulty);
    }
}

void _GetControl() {
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
