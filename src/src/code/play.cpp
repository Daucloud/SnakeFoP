#include"data.h"
#include"funda.h"
#include"extension.h"

int main() {
    system("title SnakeFoP");
    system("mode con cols=100 lines=30");
    _load();
    _update_config(current_config);
    _update_map(current_map);
    _opposite();
    srand(seed);
    SetConsoleOutputCP(CP_UTF8);
    while(true) {
        if(_interface()) {
            _initialize(width,height);
            std::thread GetControl(_getcontrol);
            std::thread refresh(_refresh);
            GetControl.join();
            refresh.join();
        }
    }
    return 0;
}