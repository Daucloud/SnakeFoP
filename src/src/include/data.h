#ifndef DATA_H
#define DATA_H

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
#include <sstream>
#include<vector>
#include<algorithm>
#include <functional>

//地图最大值
const int maxn=30;
//坐标
struct coordinate {
    int x,y;
    bool operator==(const coordinate &other){
        return x==other.x&&y==other.y;
    }
};
//蛇的位置用链表进行存储
struct snake {
    coordinate coor;
    snake*next;
};
//方便存储食物概率
struct food_prob {
    double a,b,c;
};
//反方向的定义
extern std::map<char,char>opposite;
//分别是地图、宽、高、是否暂停、当前得分
extern int stage[maxn][maxn],width,height,stop,score;
//分别是游戏难度、随机种子和食物数量
extern int difficulty,seed,foodquan;
//四面墙的虚实
extern int ww,ws,wa,wd;
//当前方向和用户输入的方向
extern char direction,direction1;
//判定游戏是否结束
extern bool gameover;
//蛇尾和蛇头
extern snake *tail, *head;
//食物出现概率
extern food_prob foodprob;
//用于存储当前的配置和地图文件
extern std::string current_config,current_map;
//存储游戏到此为止的游戏信息
extern std::ofstream tempfile;
#endif