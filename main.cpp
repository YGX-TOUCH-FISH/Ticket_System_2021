#include <iostream>
#include "program.h"
using namespace std;

int main() {
//    freopen("data/basic_6/1.in" , "r" , stdin);
//    freopen("data/basic_6/1ans.txt" , "w" , stdout);

    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
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
