//
// Created by lenovo on 2021/4/26.
//
#include <iostream>
#include "program.h"
#include "BPT.hpp"
#include "Users.h"
#include "Train.h"
#include "Ticket.h"
using namespace std;

User_Control userSystem;
Train_Control trainSystem;
Ticket_Control ticketSystem;
Order_Control orderSystem;

extern map<int , int> user_Online;//username —— pri: map

//TODO 语句处理(含一个参数)
vector<string> Split(const std::string &cmd , char p){
    vector<string> tmp;
    string t;
    for (int i = 0 ; i < cmd.length() ; ++i){
        if (cmd[i] != p) t += cmd[i];
        else {
            tmp.push_back(t);
            t.clear();
        }
    }
    tmp.push_back(t);
    return tmp;
}

int string_To_int(const std::string &s){
    int sum = 0;
    for (int i = 0 ; i < s.length() ; ++i){
        sum = sum * 10 + s[i] - '0';
    }
    return sum;
}

void Run(std::string &command){
    switch (command[0]) {
        case 'a': {
            if (command[4] == 'u') add_user(command);
            else add_train(command);
            break;
        }
        case 'b':
            buy_ticket(command);
            break;
        case 'c':
            Clean();
            break;
        case 'd':
            delete_train(command);
            break;
        case 'l':{
            if (command[3] == 'i') login(command);
            else logout(command);
            break;
        }
        case 'm':
            modify_profile(command);
            break;
        case 'q':{
            switch (command[9]) {
                case 'f':
                    query_profile(command);
                    break;
                case 'e':
                    query_order(command);
                    break;
                case 'k':
                    query_ticket(command);
                    break;
                case 'i':
                    query_train(command);
                    break;
                default:
                    query_transfer(command);
                    break;
            }
            break;
        }
        case 'r':{
            if (command[2] == 'l') release_train(command);
            else refund_ticket(command);
            break;
        }
        default:
            Exit();
            break;
    }
}

void Init(){
    fstream o;
    o.open("users.dat" , ios::in);
    if (!o){
        o.open("users_BPT.dat" , ios::out);
        o.close();
        o.open("Order_BPT.dat" , ios::out);
        o.close();
        o.open("Order.dat" , ios::out);
        o.close();
        o.open("pendingOrder_BPT.dat" , ios::out);
        o.close();
        o.open("pendingOrder.dat" , ios::out);
        o.close();
        o.open("tickets_BPT.dat" , ios::out);
        o.close();
        o.open("Ticket.dat" , ios::out);
        o.close();
        o.open("train_BPT.dat" , ios::out);
        o.close();
        o.open("Train.dat" , ios::out);
        o.close();
        o.open("trainSeat_BPT.dat" , ios::out);
        o.close();
        o.open("trainSeat.dat" , ios::out);
        o.close();
    }
    o.close();
}

//———————————————————————————————————————————————*****————————————————————————————————————————————————————————————————//

void add_user(std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(9) , ' ');
    String<21> username , cur_user;
    String<31> password;
    String<20> name;
    String<31> mailAddress;
    int pri;
    for (int i = 0 ; i < tmp.size() ; i += 2){
        switch (tmp[i][1]) {
            case 'c':
                cur_user = String<21> (tmp[i + 1]);
                break;
            case 'u':
                username = String<21> (tmp[i + 1]);
                break;
            case 'p':
                password = String<31> (tmp[i + 1]);
                break;
            case 'n':
                name = String<20> (tmp[i + 1]);
                break;
            case 'm':
                mailAddress = String<31> (tmp[i + 1]);
                break;
            default:{
                pri = string_To_int(tmp[i + 1]);
                break;
            }
        }
    }

    if (userSystem.empty()) { //判断是否为第一个用户
        User u(username , password , name , mailAddress , 10);
        userSystem.add_user(u);
        std::cout << 0 << "\n";
    }
    else {
        if (user_Online.find(cur_user.hash_value) == user_Online.end()) throw "error (no login)";
        int cur_pri = user_Online[cur_user.hash_value];
        if (cur_pri <= pri) throw "error (pri)";
        User u(username , password , name , mailAddress , pri);
        userSystem.add_user(u);
        std::cout << 0 << "\n";
    }
}

void login(std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(6) , ' ');
    String<21> username;
    String<31> password;
    for (int i = 0 ; i < tmp.size() ; i += 2){
        switch (tmp[i][1]) {
            case 'u':
                username = String<21> (tmp[i + 1]);
                break;
            default:
                password = String<31> (tmp[i + 1]);
                break;
        }
    }
    userSystem.login(username , password);
    std::cout << 0 << "\n";
}

void logout(std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(7) , ' ');
    String<21> username(tmp[1]);
    userSystem.logout(username);
    std::cout << 0 << "\n";
}

void query_profile(std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(14) , ' ');
    String<21> cur_user , username;
    for (int i = 0 ; i < tmp.size() ; i += 2){
        switch (tmp[i][1]) {
            case 'c':
                cur_user = String<21> (tmp[i + 1]);
                break;
            default:
                username = String<21> (tmp[i + 1]);
                break;
        }
    }
    if (user_Online.find(cur_user.hash_value) == user_Online.end()) throw "error";
    int cur_pri = user_Online[cur_user.hash_value];
    vector<User> container;
    container.clear();
    container = userSystem.find(username);
    if (container.empty()) throw "error";
    if (cur_pri > container[0].getPrivilege() || cur_user == username){
        userSystem.show_inf_user(container[0]);
    }
    else throw "error";
}

void modify_profile(std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(15) , ' ');
    String<21> cur_user , username;
    String<31> password;
    String<20> name;
    String<31> mailAddress;
    int pri = -1;
    for (int i = 0 ; i < tmp.size() ; i += 2){
        switch (tmp[i][1]) {
            case 'c':
                cur_user = String<21> (tmp[i + 1]);
                break;
            case 'u':
                username = String<21> (tmp[i + 1]);
                break;
            case 'p':
                password = String<31> (tmp[i + 1]);
                break;
            case 'n':
                name = String<20> (tmp[i + 1]);
                break;
            case 'm':
                mailAddress = String<31> (tmp[i + 1]);
                break;
            case 'g':{
                pri = string_To_int(tmp[i + 1]);
                break;
            }
        }
    }
    if (user_Online.find(cur_user.hash_value) == user_Online.end()) throw "error";
    int cur_pri = user_Online[cur_user.hash_value];
    vector<User> container;
    container.clear();
    container = userSystem.find(username);
    if (container.empty()) throw "error";
    if (pri != -1 && cur_pri <= pri) throw "error";
    if (cur_pri > container[0].getPrivilege() || cur_user == username){
        userSystem.modify_user(container[0] , password , name , mailAddress , pri);
    }
    else throw "error";
}

void add_train(std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(10) , ' ');
    String<21> trainID;
    string stations , prices , startTime , travelTimes , stopoverTimes , saleDate , type;
    int stationNum , seatNum;
    date StartTime{};
    for (int i = 0 ; i < tmp.size() ; i += 2){
        switch (tmp[i][1]) {
            case 'i':
                trainID = String<21> (tmp[i + 1]);
                break;
            case 'n': {
                stationNum = string_To_int(tmp[i + 1]);
                break;
            }
            case 'm':{
                seatNum = string_To_int(tmp[i + 1]);
                break;
            }
            case 's':
                stations = tmp[i + 1];
                break;
            case 'p':
                prices = tmp[i + 1];
                break;
            case 'x': {
                startTime = tmp[i + 1];
                vector<string> s = Split(startTime , ':');
                int h , m;
                h = string_To_int(s[0]);
                m = string_To_int(s[1]);
                StartTime.reset(0 , 0 , h , m);
                break;
            }
            case 't':
                travelTimes = tmp[i + 1];
                break;
            case 'o':
                stopoverTimes = tmp[i + 1];
                break;
            case 'd':
                saleDate = tmp[i + 1];
                break;
            default:
                type = tmp[i + 1];
                break;
        }
    }
    String<40> Stations[stationNum + 1];
    int Prices[stationNum + 1] , TravelTimes[stationNum + 1] , StopoverTimes[stationNum];

    tmp.clear();
    //处理车站
    tmp = Split(stations , '|');
    pair<int , int> hash[stationNum + 1];
    for (int i = 0 ; i < tmp.size() ; ++i) {
        Stations[i + 1] = String<40> (tmp[i]);
        hash[i + 1] = make_pair(Stations[i + 1].hash_value , i + 1);
    }
    StationHashArray stationHash(hash , stationNum);
    tmp.clear();
    //处理票价
    tmp = Split(prices , '|');
    Prices[1] = 0;
    for (int i = 0 ; i < tmp.size() ; ++i){
        Prices[i + 2] = string_To_int(tmp[i]);
        Prices[i + 2] += Prices[i + 1];
    }
    tmp.clear();
    //处理两站之间时间
    tmp = Split(travelTimes , '|');
    TravelTimes[0] = TravelTimes[1] = 0;
    for (int i = 0 ; i < tmp.size() ; ++i){
        TravelTimes[i + 2] = string_To_int(tmp[i]);
        TravelTimes[i + 2] += TravelTimes[i + 1];
    }
    tmp.clear();
    //处理站台停留时间（只有两站的特判）
    StopoverTimes[0] = StopoverTimes[1] = 0;
    if (stationNum != 2){
        tmp = Split(stopoverTimes , '|');
        for (int i = 0 ; i < tmp.size() ; ++i){
            StopoverTimes[i + 2] = string_To_int(tmp[i]);
            StopoverTimes[i + 2] += StopoverTimes[i + 1];
        }
    }
    tmp.clear();

    tmp = Split(saleDate , '|');
    string sale_start = tmp[0] , sale_end = tmp[1];
    vector<string> s;
    int mon , d;
    s = Split(sale_start , '-');
    mon = string_To_int(s[0]);
    d = string_To_int(s[1]);
    date saleStart(mon , d , 0 , 0);
    s = Split(sale_end , '-');
    mon = string_To_int(s[0]);
    d = string_To_int(s[1]);
    date saleEnd(mon , d , 0 , 0);
    Train t(trainID , Stations , stationNum , seatNum , Prices , char (type[0]) , TravelTimes , StopoverTimes , StartTime , saleStart , saleEnd , stationHash , 0);
    trainSystem.addTrain(t);
    std::cout << 0 << "\n";
}

void release_train(std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(14) , ' ');
    String<21> trainID(tmp[1]);
    vector<Train> exist_train;
    exist_train = trainSystem.findTrain(trainID);
    if (exist_train.empty()) throw "error";
    Train r_train = exist_train[0];
    trainSystem.releaseTrain(r_train);
    ticketSystem.addTicket(r_train);
    std::cout << 0 << "\n";
}

void query_train(std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(12) , ' ');
    String<21> trainID;
    string Time;
    date time{};
    for (int i = 0 ; i < tmp.size() ; i += 2){
        switch (tmp[i][1]) {
            case 'i':
                trainID = String<21> (tmp[i + 1]);
                break;
            default: {
                Time = tmp[i + 1];
                vector<string> s = Split(Time , '-');
                int M = string_To_int(s[0]) , d = string_To_int(s[1]);
                time.reset(M , d , 0 , 0);
                break;
            }
        }
    }
    vector<Train> exist_train = trainSystem.findTrain(trainID);
    if (exist_train.empty()) throw "no findTrain";
    trainSystem.queryTrain(exist_train[0] , time);
}

void delete_train(std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(13) , ' ');
    String<21> trainID(tmp[1]);
    trainSystem.deleteTrain(trainID);
    std::cout << 0 << "\n";
}

void query_ticket(const std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(13) , ' ');
    string Time , st , ed;
    int type = 0; // 0 for time ; 1 for cost
    date q_time{};
    for (int i = 0 ; i < tmp.size() ; i += 2){
        switch (tmp[i][1]) {
            case 's':
                st = tmp[i + 1];
                break;
            case 't':
                ed = tmp[i + 1];
                break;
            case 'd': {
                Time = tmp[i + 1];
                vector<string> s = Split(Time , '-');
                int M = string_To_int(s[0]) , d = string_To_int(s[1]);
                q_time.reset(M , d , 0 , 0);
                break;
            }
            default:{
                if (tmp[i + 1] == "time") type = 0;
                else type = 1;
                break;
            }
        }
    }
    if (st == ed) {
        cout << 0 << "\n";
        return;
    }
    ticketSystem.queryTicket(String<40> (st) , String<40> (ed) , q_time , type);
}

void query_transfer(const std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(15) , ' ');
    string Time , st , ed;
    int type = 0; // 0 for time ; 1 for cost
    date q_time{};
    for (int i = 0 ; i < tmp.size() ; i += 2){
        switch (tmp[i][1]) {
            case 's':
                st = tmp[i + 1];
                break;
            case 't':
                ed = tmp[i + 1];
                break;
            case 'd': {
                Time = tmp[i + 1];
                vector<string> s = Split(Time , '-');
                int M = string_To_int(s[0]) , d = string_To_int(s[1]);
                q_time.reset(M , d , 0 , 0);
                break;
            }
            default:{
                if (tmp[i + 1] == "time") type = 0;
                else type = 1;
                break;
            }
        }
    }
    ticketSystem.queryTransfer(String<40> (st) , String<40> (ed) , q_time , type);
}

void buy_ticket(const std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(11) , ' ');
    String<21> username , trainID;
    string Time , st , ed;
    int num , isQue = 0; // 0 for false ; 1 for true
    date time{};
    for (int i = 0 ; i < tmp.size() ; i += 2){
        switch (tmp[i][1]) {
            case 'u':
                username = String<21> (tmp[i + 1]);
                break;
            case 'i':
                trainID = String<21> (tmp[i + 1]);
                break;
            case 'd':{
                Time = tmp[i + 1];
                vector<string> s = Split(Time , '-');
                int M = string_To_int(s[0]) , d = string_To_int(s[1]);
                time.reset(M , d , 0 , 0);
                break;
            }
            case 'n':{
                num = string_To_int(tmp[i + 1]);
                break;
            }
            case 'f':{
                st = tmp[i + 1];
                break;
            }
            case 't':{
                ed = tmp[i + 1];
                break;
            }
            default:{
                if (tmp[i + 1] == "false") isQue = 0;
                else isQue = 1;
                break;
            }
        }
    }
    if (user_Online.find(username.hash_value) == user_Online.end()) throw "error (no login)";
    if (num <= 0) throw "error";
    int no = userSystem.user_addOrderNum(username);
    ticketSystem.buyTicket(username , trainID , String<40> (st) , String<40> (ed) , time , num , isQue , no);
}

void query_order(const std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(12) , ' ');
    String<21> username(tmp[1]);
    if (user_Online.find(username.hash_value) == user_Online.end()) throw "error";
    vector<Order> exist_Order = orderSystem.findOrder(username);
    if (exist_Order.empty()) cout << 0 << "\n";
    else {
        exist_Order.sort();
        cout << exist_Order.size() << "\n";
        for (int i = (int)exist_Order.size() - 1; i >= 0 ; --i){
            exist_Order[i].show();
        }
    }
}

void refund_ticket(const std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(14) , ' ');
    String<21> username;
    int num = 1;
    for (int i = 0 ; i < tmp.size() ; i += 2){
        switch (tmp[i][1]) {
            case 'u':
                username = String<21> (tmp[i + 1]);
                break;
            default:{
                num = string_To_int(tmp[i + 1]);
                break;
            }
        }
    }
    if (user_Online.find(username.hash_value) == user_Online.end()) throw "error (no login)";
    vector<Order> exist_Order = orderSystem.findOrder(username);
    if (exist_Order.empty()) throw "error";
    exist_Order.sort();
    if (num > exist_Order.size() || num <= 0) throw "error";
    Order r_order = exist_Order[exist_Order.size() - num];
    if (r_order.getStatus() == 1){
        orderSystem.refundOrder(username , r_order);
        ticketSystem.que_BuyTicket(username , r_order);//username没用
    }
    else if (r_order.getStatus() == 2){
        orderSystem.refundOrder(username , r_order);
    }
    else throw "error";
    cout << 0 << "\n";
    //更改order 更改余票信息 查看候补队列是否能够购买
}

void Clean(){
    userSystem.restart();
    ticketSystem.restart();
    trainSystem.restart();
    orderSystem.restart();
    user_Online.clear();
    cout << 0 << '\n';
}

void Exit(){
    cout << "bye\n";
    exit(0);
}