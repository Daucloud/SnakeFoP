#include"data.h"
#include"funda.h"
#include"extension.h"

int main() {
    _opposite();
    srand(seed);
    SetConsoleOutputCP(CP_UTF8);
    while(true) {
        if(_interface()) {
            _printstage(15,15);
            std::thread GetControl(_getcontrol);
            std::thread refresh(_refresh);
            GetControl.join();
            refresh.join();
        }
    }
    return 0;
}