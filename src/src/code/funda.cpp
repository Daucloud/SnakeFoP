#include"funda.h"
#include"data.h"
#include"extension.h"

std::map<char,char>opposite;
int stage[maxn][maxn],height,width,stop,score;
food_prob foodprob;
int difficulty,seed,foodquan;
int ww,ws,wa,wd;
char direction,direction1;
snake *tail, *head;
bool gameover=true;
std::string current_config,current_map;

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
    printf("Enter \"g\" to begin and \"q\" to end.Or enter \"help\" to refer to the help manual.\n");
    while(true) {
        std::string temp="\n";
        std::cin>>temp;
        if(temp=="help") {
            _help();
            return false;
        }
        else if(temp[1]) {
            printf("Illegal Input.Please enter again:\n");
            Sleep(1000);
            return false;
        }
        char foo=temp[0];
        if(foo=='g') {
            //_countdown();
            _closecursor(false);
            system("cls");
            return true;
        }
        else if(foo=='q') {
            printf("Thank you for playing!\n");
            Sleep(1000);
            system("cls");
            exit(0);
        }
        else {
            if(_other(foo))return false;
        }
    }
}

void _printfood(int height,int width) {
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
        x=rand()%(height-2)+1;
        y=rand()%(width-2)+1;
    } while(stage[y][x]!=0);
    stage[y][x]=ans;
    _gotoxy(x,y);
    printf("\e[%dm@\e[0m",color);
    tempfile<<x<<" "<<y<<" "<<ans<<" ";
}

void _printstage(int height,int width) {
    for(int i=0; i<height+2; ++i) {
        for(int j=0; j<width+2; ++j) {
            if(i==0) {
                printf("\e[100m_\e[0m");
                int foo=ww?-1:5;
                stage[i][j]=foo;
            }
            else if(i==height+1) {
                printf("\e[100m_\e[0m");
                int foo=ws?-1:6;
                stage[i][j]=foo;
            }
            else if(j==0) {
                printf("\e[100m|\e[0m");
                int foo=wa?-1:7;
                stage[i][j]=foo;
            }
            else if(j==width+1) {
                printf("\e[100m|\e[0m");
                int foo=wd?-1:8;
                stage[i][j]=foo;
            }
            else {
                if(stage[i][j]==-2) {
                    printf("\e[46m!\e[0m");
                }
                else printf(" ");
            }
        }
        printf("\n");
    }
}
//游戏初始化
void _initialize(int width,int height) {
    tempfile.open("../src/record/temp");
    tempfile<<current_config<<"\n"<<current_map<<"\n";
    stop=1;
    gameover=true;
    score=0;
    _update_map(current_map);
    _printstage(height,width);
    _gotoxy(height+8,0);
    printf("\tscore: \t0");
    _gotoxy(height+8,1);
    printf("\tconfig: %s",current_config.c_str());
    _gotoxy(height+8,2);
    printf("\tmap: \t%s",current_map.c_str());
    _gotoxy(height/2-2,width/2+1);
    printf("\e[42m***#\e[0m");
    direction='d';
    direction1='d';
    for(int i=height/2-2; i<height/2+2; ++i)stage[width/2+1][i]=-1;
    snake *p,*q;
    p=new snake;
    p->coor= {height/2-2,width/2+1};
    p->next=NULL;
    tail=p;
    q=p;
    for(int i=1; i<4; ++i) {
        p=new snake;
        p->coor= {height/2-2+i,width/2+1};
        p->next=NULL;
        q->next=p;
        q=p;
    }
    head=q;
    head->next=NULL;
    for(int i=0; i<foodquan; ++i)_printfood(height,width);
    _save();
    tempfile<<"\n";
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
        if(stage[foo.y][foo.x]>0&&stage[foo.y][foo.x]<5) {
            increase=true;
            score+=stage[foo.y][foo.x];
            stage[foo.y][foo.x]=0;
            _gotoxy(height+12,0);
            printf("\t%d",score);
        }
        if(!increase) {
            if(stage[foo.y][foo.x]<0) {
                gameover=false;
                snake *curr=tail;
                while(curr->next!=NULL) {
                    _gotoxy(curr->coor.x,curr->coor.y);
                    printf("\e[41m*\e[0m");
                    stage[curr->coor.x][curr->coor.y]=0;
                    curr=curr->next;
                }
                _gotoxy(curr->coor.x,curr->coor.y);
                printf("\e[41m#\e[0m");
                stage[curr->coor.x][curr->coor.y]=0;
                Sleep(1000);
                system("cls");
                tempfile.close();
                printf("Game Over! Your score is %d. \nPress \"b\" to save your record, or any other key to return to the menu.\n",score);
                _closecursor(true);
                keybd_event(VK_DOWN, 0, 0, 0);
                std::string temp;
                std::cin>>temp;
                if(temp=="b")_record_b();
                else std::remove("../src/record/temp");
                Sleep(500);
                break;
            }
            else if(stage[foo.y][foo.x]==5)foo.y=height;
            else if(stage[foo.y][foo.x]==6)foo.y=1;
            else if(stage[foo.y][foo.x]==7)foo.x=width;
            else if(stage[foo.y][foo.x]==8)foo.x=1;
            stage[tail->coor.y][tail->coor.x]=0;
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
        if(increase) {
            _printfood(height,width);
        }
        else tempfile<<"0 0 0 ";
        tempfile<<direction<<"\n";
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
                tempfile.close();
                std::remove("../src/record/temp");
                system("cls");
                stop=-1;
                break;
            }
    }
}
