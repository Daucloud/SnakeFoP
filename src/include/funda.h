#ifndef FUNDA_H
#define FUNDA_H

#include<Windows.h>

//同方向和反方向键值对的建立
void _opposite();
//光标移动
void _gotoxy(SHORT x, SHORT y);
//关闭光标显示
void _closecursor(bool ok);
//主界面&&游戏开始
bool _interface();
//倒计时
void _countdown();
//食物生成
void _printfood(int width,int height);
//地图打印&&蛇的生成
void _printstage(int width,int height);
//控制台的刷新
void _refresh();
//根据用户输入更新移动方向
void _getcontrol();

#endif