#include"data.h"
#include"funda.h"
#include"extension.h"


std::ofstream tempfile;

//字符串转整数
int _changetoint(const std::string& str) {
    try {
        return std::stoi(str);
    } catch (const std::exception&) {
        return -2;
    }
}
//字符串转小数
double _changetodouble(const std::string& str) {
    try {
        return std::stod(str);
    } catch (const std::exception&) {
        return -2;
    }
}
//判断自定义配置文件的合法性
bool _validconfig_i(int c1,int c2,int c3,double c4,double c5,double c6) {
    if(!(c1>=1&&c1<=10))return false;
    if(c2<-1)return false;
    if(!(c3>=1&&c3<=5))return false;
    if(!(c4>=0&&c4<=1))return false;
    if(!(c5>=0&&c5<=1))return false;
    if(!(c6>=0&&c6<=1))return false;
    if(abs(c4+c5+c6-1)>1e-3)return false;
    return true;
}
//检查文件名称的合法性
bool _valid_filename(std::string&str) {
    for(int i=0; str[i]; ++i) {
        if(!isalpha(str[i])&&!isdigit(str[i])&&str[i]!='_')return false;
    }
    return true;
}
//输入i，新建配置
void _config_i() {
    printf("Configuration creating:\n");
    Sleep(1000);
    system("cls");
    printf("Please enter the name of your new configuration:\n");
    std::string foo;
label:
    std::cin>>foo;
    if(foo=="q") {
        printf("Configuration creating ended.\n");
        Sleep(1000);
        return;
    }
    if(!_valid_filename(foo)) {
        printf("Invalid filename! Filename can only be composed of numbers, letters and underscores. Please Enter again:\n");
        goto label;
    }
    printf("Please enter the difficulty,seed,food quantity and food generation probability in the following four lines respectively:\n");
    std::string s1,s2,s3,s4,s5,s6;
    std::cin>>s1>>s2>>s3>>s4>>s5>>s6;
    int c1=_changetoint(s1);
    int c2=_changetoint(s2);
    int c3=_changetoint(s3);
    double c4=_changetodouble(s4);
    double c5=_changetodouble(s5);
    double c6=_changetodouble(s6);
    if(!_validconfig_i(c1,c2,c3,c4,c5,c6)) {
        printf("Invalid configuration! Please enter again.\n");
        goto label;
    }
    std::string foo1="../src/config/"+foo+".config";
    std::ofstream file(foo1);
    file<<c1<<"\n"<<c2<<"\n"<<c3<<"\n"<<c4<<" "<<c5<<" "<<c6<<"\n";
    file.close();
    printf("Configuration created successfully!\n");
    Sleep(1000);
}
//输入u，载入配置
void _config_u() {
    printf("Configuration Loading:\n");
    Sleep(1000);
    system("cls");
    printf("Please enter the name of the configuration:\n");
    std::string foo;
label2:
    std::cin>>foo;
    if(foo=="q") {
        printf("Configuration loading ended.\n");
        Sleep(1000);
        return;
    }
    bool temp=_update_config(foo);
    if(!temp) {
        printf("No such configuration! Please enter again:\n");
        goto label2;
    }
    else printf("Configuration loaded successfully!\n");
    Sleep(1000);
}
//输入n，新建地图
void _map_n() {
    printf("Map creating:\n");
    Sleep(1000);
    system("cls");
    printf("Please enter the name of your new map:\n");
    std::string foo;
label2:
    std::cin>>foo;
    if(foo=="q") {
        printf("Map creating ended.\n");
        Sleep(1000);
        return;
    }
    if(!_valid_filename(foo)) {
        printf("Invalid filename! Filename can only be composed of numbers, letters and underscores. Please Enter again:\n");
        goto label2;
    }
    memset(stage,0,sizeof(stage));
    std::string name="../src/maps/"+foo+".map";
    std::ofstream file(name);
    std::string s1,s2;
    int width,height;
label3:
    printf("Please enter the width and the height of your map:\n");
    std::cin>>s1>>s2;
    if(!_valid_mapsize(s1,s2,width,height)) goto label3;
    file<<width<<" "<<height<<"\n";
    system("cls");
    int w=1,s=1,a=1,d=1;
    _printmap(width,height,w,s,a,d);
    std::cin.ignore();
    std::vector<coordinate>obstacle;
    while(true) {
        std::string instr="\n";
        std::getline(std::cin,instr);
        int key=_input_map(instr,width,height,w,s,a,d,obstacle);
        if(key<0) {
            printf("Invalid instruction! Refer to help for legal instructions. Please enter again:\n");
            Sleep(1000);
            system("cls");
            _printmap(width,height,w,s,a,d);
        }
        else if(key==0) {
            file<<w<<" "<<s<<" "<<a<<" "<<d<<"\n"<<obstacle.size()<<"\n";
            for(int i=0; i<obstacle.size(); ++i)file<<obstacle[i].x<<" "<<obstacle[i].y<<"\n";
            file.close();
            break;
        }
        else if(key==1) {
            system("cls");
            _printmap(width,height,w,s,a,d);
        }
        else if(key==2) {
            file.close();
            remove(name.c_str());
            return;
        }
    }
}
//地图配置输入判断
int _input_map(std::string &str,int w,int h,int&w1,int&s,int&a,int&d,std::vector<coordinate>&obstacle) {
    if(str=="f")return 0;
    if(str=="q")return 2;
    if(str[0]=='o') {
        if(obstacle.size()==15) {
            printf("There are at most 15 obstacles!\n");
            Sleep(1000);
            return 1;
        }
        std::vector<int>num;
        std::string str1=str.substr(1),temp;
        std::istringstream iss(str1);
        int num1;
        while(iss>>temp) {
            num1=_changetoint(temp);
            num.push_back(num1);
        }
        if(num.size()>2)return -1;
        if(num[0]<0||num[0]>w)return -1;
        if(num[1]<0||num[1]>h)return -1;
        stage[num[0]][num[1]]=-2;
        obstacle.push_back({num[0],num[1]});
        return 1;
    }
    else if(str[0]=='p') {
        std::vector<int>num;
        std::string str1=str.substr(1),temp;
        std::istringstream iss(str1);
        int num1;
        while(iss>>temp) {
            num1=_changetoint(temp);
            num.push_back(num1);
        }
        if(num.size()>2)return -1;
        if(num[0]<0||num[0]>w)return -1;
        if(num[1]<0||num[1]>h)return -1;
        stage[num[0]][num[1]]=0;
        coordinate foo= {num[0],num[1]};
        auto it=std::find(obstacle.begin(),obstacle.end(),foo);
        if(it!=obstacle.end())obstacle.erase(it);
        return 1;
    }
    else if(str[0]=='d') {
        std::vector<int>num;
        std::string str1=str.substr(1),temp;
        std::istringstream iss(str1);
        int num1;
        while(iss>>temp) {
            num1=_changetoint(temp);
            num.push_back(num1);
        }
        if(num.size()>1)return -1;
        if(num[0]!=0&&num[0]!=1)return -1;
        d=num[0];
        return 1;
    }
    else if(str[0]=='w') {
        std::vector<int>num;
        std::string str1=str.substr(1),temp;
        std::istringstream iss(str1);
        int num1;
        while(iss>>temp) {
            num1=_changetoint(temp);
            num.push_back(num1);
        }
        if(num.size()>1)return -1;
        if(num[0]!=0&&num[0]!=1)return -1;
        w1=num[0];
        return 1;
    }
    else if(str[0]=='s') {
        std::vector<int>num;
        std::string str1=str.substr(1),temp;
        std::istringstream iss(str1);
        int num1;
        while(iss>>temp) {
            num1=_changetoint(temp);
            num.push_back(num1);
        }
        if(num.size()>1)return -1;
        if(num[0]!=0&&num[0]!=1)return -1;
        s=num[0];
        return 1;
    }
    else if(str[0]=='a') {
        std::vector<int>num;
        std::string str1=str.substr(1),temp;
        std::istringstream iss(str1);
        int num1;
        while(iss>>temp) {
            num1=_changetoint(temp);
            num.push_back(num1);
        }
        if(num.size()>1)return -1;
        if(num[0]!=0&&num[0]!=1)return -1;
        a=num[0];
        return 1;
    }
    else return -1;
}
//检查地图大小输入的合法性
bool _valid_mapsize(std::string &str1,std::string &str2,int &a,int &b) {
    a=_changetoint(str1);
    b=_changetoint(str2);
    if(a<0||b<0) {
        printf("Invalid input!. Please enter numbers:\n");
        return false;
    }
    if(a>20||b>20||a<8||b<8) {
        printf("Invalid size! The width and height are expected to be between 8 and 20(inclusive). Please enter again:\n");
        return false;
    }
    return true;
}
//游戏墙壁打印
void _printmap(int width,int height,int w,int s,int a,int d) {
    for(int i=0; i<height+2; ++i) {
        for(int j=0; j<width+2; ++j) {
            if(i==0) {
                int foo=w?41:42;
                printf("\e[%dm",foo);
                printf("_");
                printf("\e[0m");
            }
            else if(i==height+1) {
                int foo=s?41:42;
                printf("\e[%dm",foo);
                printf("_");
                printf("\e[0m");
            }
            else if(j==0) {
                int foo=a?41:42;
                printf("\e[%dm",foo);
                printf("|");
                printf("\e[0m");
            }
            else if(j==width+1) {
                int foo=d?41:42;
                printf("\e[%dm",foo);
                printf("|");
                printf("\e[0m");
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
//对于各种周边功能输入的判定
bool _other(char c) {
    if(c=='i') {
        _config_i();
        return true;
    }
    else if(c=='u') {
        _config_u();
        return true;
    }
    else if(c=='n') {
        _map_n();
        return true;
    }
    else if(c=='m') {
        _map_m();
        return true;
    }
    else if(c=='r') {
        _record_r();
        return true;
    }
    else {
        printf("Illegal Input.Please enter again:\n");
        Sleep(1000);
        return false;
    }
}
//保存用户的最后配置
void _save() {
    std::ofstream file("../src/last.txt");
    file<<current_config<<"\n"<<current_map;
    file.close();
}
//输入m，载入地图
void _map_m() {
    printf("Map loading:\n");
    Sleep(1000);
    system("cls");
    printf("Please enter the name of the map:\n");
    std::string foo;
label2:
    std::cin>>foo;
    if(foo=="q") {
        printf("Map loading ended.\n");
        Sleep(1000);
        return;
    }
    bool temp=_update_map(foo);
    if(!temp) {
        printf("No such map! Please enter again:\n");
        goto label2;
    }
    else printf("Map loaded successfully!\n");
    Sleep(1000);
}

bool _update_config(const std::string&str) {
    std::string name="../src/config/"+str+".config";
    std::ifstream file(name);
    if(!file) return false;
    file>>difficulty>>seed>>foodquan>>foodprob.a>>foodprob.b>>foodprob.c;
    file.close();
    current_config=str;
    return true;
}

bool _update_map(const std::string&str) {
    std::string name="../src/maps/"+str+".map";
    std::ifstream file(name);
    if(!file) return false;
    memset(stage,0,sizeof(stage));
    int temp;
    file>>width>>height>>ww>>ws>>wa>>wd>>temp;
    for(int i=0; i<temp; ++i) {
        int x,y;
        file>>x>>y;
        stage[x][y]=-2;
    }
    file.close();
    current_map=str;
    return true;
}

void _load() {
    std::ifstream file("../src/last.txt");
    file>>current_config>>current_map;
    file.close();
}

void _record_b() {
    printf("Record saving:\n");
    Sleep(1000);
    system("cls");
    printf("Please enter the name of your new record:\n");
    std::string foo;
label:
    std::cin>>foo;
    if(foo=="q") {
        printf("Record saving ended.\n");
        Sleep(1000);
        return;
    }
    if(!_valid_filename(foo)) {
        printf("Invalid filename! Filename can only be composed of numbers, letters and underscores. Please Enter again:\n");
        goto label;
    }
    std::string newname="../src/record/"+foo+".rec";
    std::string oldname="../src/record/temp";
    std::ofstream file(newname);
    if(file) {
        file.close();
        std::remove(newname.c_str());
    }
    const char* newname_cstr = newname.c_str();
    const char* oldname_cstr = oldname.c_str();
    std::rename(oldname_cstr, newname_cstr);
}

bool _printfood_given(int x,int y,int v) {
    if(v==0)return true;
    int color;
    switch(v) {
    case 1:
        color=44;
        break;
    case 2:
        color=45;
        break;
    case 3:
        color=43;
        break;
    default:
        return false;
    }
    if((x>width)||(x<0)||(y>height)||(y<0))return false;
    stage[y][x]=v;
    _gotoxy(x,y);
    printf("\e[%dm@\e[0m",color);
    return true;
}

bool _refresh_record() {
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
    default:
        return false;
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
        if(stage[foo.y][foo.x]==5)foo.y=height;
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
    return true;
}

void _record_r() {
    printf("Record Playing:\n");
    Sleep(1000);
    system("cls");
    printf("Please enter the name of the record:\n");
    std::string foo;
label2:
    std::cin>>foo;
    if(foo=="q") {
        printf("Record playing ended.\n");
        Sleep(1000);
        return;
    }
    std::string name="../src/record/"+foo+".rec";
    std::ifstream file(name);
    if(!file) {
        printf("No such record! Please enter again:\n");
        goto label2;
    }
    else printf("Record Playing start!\n");
    Sleep(1000);
    system("cls");
    file>>current_config>>current_map;
    if(!_update_config(current_config)) {
        printf("The configuaration of this record does not exists!\n");
        Sleep(1000);
        return ;
    }
    if(!_update_map(current_map)) {
        printf("The map of this record does not exists!\n");
        Sleep(1000);
        return ;
    }
    _initialize_record();
    int x,y,v;
    for(int i=0; i<foodquan; ++i) {
        file>>x>>y>>v;
        if(!_printfood_given(x,y,v)) {
            system("cls");
            printf("There is some illegal data in the record file. Please have a check.\n");
            Sleep(1000);
            return;
        }
    }
    int ok=0;
    std::thread Thread(_getcontrol_record,std::ref(ok));
    Thread.detach();
    while(true) {
        if(ok==1) {
            Sleep(1000);
            system("cls");
            printf("Replaying ended.\n");
            Sleep(1000);
            break;
        }
        if(!(file>>x>>y>>v>>direction)) {
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
            printf("Game over! Replaying ended.\n");
            Sleep(1000);
            system("cls");
            break;
        }
        if(!(_refresh_record())||!(_printfood_given(x,y,v))) {
            system("cls");
            printf("There is some illegal data in the record file. Please have a check.\n");
            Sleep(1000);
            return;
        }
        Sleep((double)1000/difficulty);
    }
}

void _getcontrol_record(int&ok) {
    std::cin.ignore();
    while(true) {
        int foo=0;
        foo=getch();
        if(foo=='q')ok=1;
        break;
    }
}

void _initialize_record() {
    stop=1;
    gameover=true;
    score=0;
    _closecursor(false);
    _printstage(height,width);
    _gotoxy(height+6,0);
    printf("score:\t0");
    _gotoxy(height+6,1);
    printf("config:\t%s",current_config.c_str());
    _gotoxy(height+6,2);
    printf("map:\t\t%s",current_map.c_str());
    _gotoxy(height/2-2,width/2+1);
    printf("\e[42m***#\e[0m");
    direction='d';
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
}

void _help() {
    while(true) {
        system("cls");
        printf("There are some additional functions to customize your game.\n");
        printf("Enter \"c\" to refer to the configuration manual, enter \"m\" to refer to the map manual. Or Press any other key to return to the menu:\n");
        char foo;
        std::cin>>foo;
        if(foo=='c') {
            printf("In the menu, you can enter \"i\" to create a new configuration, which contains the difficulty, random seed, food quantity and food generation probabilty.\n\n");
            printf("If you would like to load a created configuraiton, enter \"u\". The name of the default configutarion is \"default\".\n\n");
            printf("Press any key to return to the help manual.\n");
            while(!kbhit());
            getch();
        }
        else if(foo=='m') {
            printf("In the menu, you can enter \"n\" to create a new map. In the visual creating interface, you can:\n\n");
            printf("1. Enter the instruct \"o x y\" to create a obstacle in the entry(x,y);\n");
            printf("2. Enter the instruct \"p x y\" to remove a obstacle from the entry(x,y);\n");
            printf("3. Enter the instruct \"w/s/a/d 0\" to set the corresponding wall passable, and \"w/s/a/d 1\" to set it unpassable.\n\n");
            printf("If you would like to load a created map, enter \"m\". The name of the default map is \"default\".\n\n");
            printf("Press any key to return to the help manual.\n");
            while(!kbhit());
            getch();
        }
        else return;
    }
}