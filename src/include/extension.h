#ifndef EXTENSION_H
#define EXTENSION_H

#include<string>

//字符串转整数
int _changetoint(const std::string& str);
//字符串转小数
double _changetodouble(const std::string& str);
//判断自定义配置文件的合法性
bool _validconfig_i(int c1,int c2,int c3,double c4,double c5,double c6);
//输入i，新建配置
void _config_i();
//输入u，载入配置
void _config_u();
//输入n，新建地图
void _map_n();
//输入m，载入地图
void _map_m();
//主界面其他输入处理
bool _other(char c);

#endif