#include <iostream>
#include "program.h"
using namespace std;

int main() {
    std::string command;

    Init();
    while(getline(cin , command)){
        try{
            Run(command);
        }
        catch (...){
            std::cout << "-1\n";
        }
    }
}
