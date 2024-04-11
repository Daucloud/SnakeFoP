#ifndef EXTENSION_H
#define EXTENSION_H

#include<string>
#include<cctype>

//字符串转整数
int _changetoint(const std::string&);
//字符串转小数
double _changetodouble(const std::string&);
//判断自定义配置文件的合法性
bool _validconfig_i(int,int,int,double,double,double);
//检查文件名称的合法性
bool _valid_filename(std::string&);
//输入i，新建配置
void _config_i();
//输入u，载入配置
void _config_u();
//输入n，新建地图
void _map_n();
//输入m，载入地图
void _map_m();
//地图配置输入判断
int _input_map(std::string &,int,int,int&,int&,int&,int&,std::vector<coordinate>&);
//地图创建时墙壁打印
void _printmap(int,int,int,int,int,int);
//检查地图大小的合法性
bool _valid_mapsize(std::string &,std::string &,int &,int &);
//主界面其他输入处理
bool _other(char);
//保存用户的配置和地图
void _save();
//更新当前配置
bool _update_config(const std::string&);
//更新当前地图
bool _update_map(const std::string&);
//载入上次配置和地图
void _load();
//保存回放
void _record_b();
//载入回放
void _record_r();
//产生给定食物
bool _printfood_given(int,int,int);
//回放刷新
bool _refresh_record();
//回放初始化
void _initialize_record();
//回放读用户输入
void _getcontrol_record(int&);
//帮助手册
void _help();
#endif