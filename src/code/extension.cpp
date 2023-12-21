#include"data.h"
#include"funda.h"
#include"extension.h"
#include"play.h"

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
//输入i，新建配置
void _config_i() {
    system("cls");
    printf("Please enter the name of your new configuration file:\n");
    std::string foo;
label:
    std::cin>>foo;
    if(foo=="q") {
        printf("Configuration creating ended.\n");
        return;
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
    std::string foo1="../src/config/"+foo;
    std::ofstream file(foo1);
    file<<c1<<"\n"<<c2<<"\n"<<c3<<"\n"<<c4<<" "<<c5<<" "<<c6<<"\n";
    file.close();
    printf("Configuration created successfully!\n");
    Sleep(1000);
}
//输入u，载入配置
void _config_u() {
    system("cls");
    printf("Please enter the name of configuration:\n");
    std::string foo;
label2:
    std::cin>>foo;
    if(foo=="q") {
        printf("Configuration loading ended.\n");
        return;
    }
    std::string name="../src/config/"+foo;
    std::ifstream file(name);
    if(!file) {
        printf("No such configuration! Please enter again:\n");
        goto label2;
    }
    file>>difficulty>>seed>>foodquan>>foodprob.a>>foodprob.b>>foodprob.c;
    file.close();
    printf("Configuration loaded successfully!\n");
    Sleep(1000);
}
//输入m，新建地图
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
    else {
        printf("Illegal Input.Please enter \"g\" or \"q\".\n");
        return false;
    }
}
