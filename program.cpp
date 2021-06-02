//
// Created by lenovo on 2021/4/26.
//
#include <iostream>
#include <sstream>
#include "program.h"
#include "BPT.hpp"
#include "Users.h"
#include "Train.h"
#include "Ticket.h"

using namespace std;


user_System userControl;
Train_System trainControl;
ticket_System ticketControl;
Order_System orderControl;

extern map<String<21> , int> user_Online;

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


void add_user(std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(9) , ' ');
    string cur_user , username , password , name , mailAddress;
    int pri;
    for (int i = 0 ; i < tmp.size() ; i += 2){
        if (tmp[i] == "-c"){
            cur_user = tmp[i + 1];
        }
        else if (tmp[i] == "-u"){
            username = tmp[i + 1];
        }
        else if (tmp[i] == "-p"){
            password = tmp[i + 1];
        }
        else if (tmp[i] == "-n"){
            name = tmp[i + 1];
        }
        else if (tmp[i] == "-m"){
            mailAddress = tmp[i + 1];
        }
        else if (tmp[i] == "-g"){
            stringstream in;
            in << tmp[i + 1];
            in >> pri;
        }
        else throw "error";
    }
    if (userControl.empty()) {
        User u(String<21>(username) , String<31>(password) , String<20>(name) , String<31>(mailAddress) , 10 , 0);
        userControl.add_user(u);
        std::cout << 0 << "\n";
    }
    else {
        if (user_Online.find(String<21> (cur_user)) == user_Online.end()) throw "error";
        vector<User> container;
        container = userControl.find(String<21> (cur_user));
        User cur = container[0];
        if (cur.getPrivilege() <= pri) throw "error";
        User u(String<21>(username) , String<31>(password) , String<20>(name) , String<31>(mailAddress) , pri , 0);
        userControl.add_user(u);
        std::cout << 0 << "\n";
    }
}

void login(std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(6) , ' ');
    string username , password;
    for (int i = 0 ; i < tmp.size() ; i += 2){
        if (tmp[i] == "-u") username = tmp[i + 1];
        else if (tmp[i] == "-p") password = tmp[i + 1];
        else throw "error";
    }
    userControl.login(String<21> (username) , String<31> (password));
    std::cout << 0 << "\n";
}

void logout(std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(7) , ' ');
    string username;
    for (int i = 0 ; i < tmp.size() ; i += 2){
        if (tmp[i] == "-u") username = tmp[i + 1];
        else throw "error";
    }
    userControl.logout(String<21> (username));
    std::cout << 0 << "\n";
}

void query_profile(std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(14) , ' ');
    string cur_user , username;
    for (int i = 0 ; i < tmp.size() ; i += 2){
        if (tmp[i] == "-c") cur_user = tmp[i + 1];
        else if (tmp[i] == "-u") username = tmp[i + 1];
        else throw "error";
    }
    if (user_Online.find(String<21> (cur_user)) == user_Online.end()) throw "error";
    vector<User> container;
    container = userControl.find(String<21> (cur_user));
    if (container.empty()) throw "error";
    User cur = container[0];
    container.clear();
    container = userControl.find(String<21> (username));
    if (container.empty()) throw "error";
    User q_user = container[0];
    if (cur.getPrivilege() > q_user.getPrivilege() || cur == q_user){
        userControl.query_user(q_user);
    }
    else throw "error";
}

void modify_profile(std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(15) , ' ');
    string cur_user , username , password , name , mailAddress;
    int pri = -1;
    for (int i = 0 ; i < tmp.size() ; i += 2){
        if (tmp[i] == "-c") cur_user = tmp[i + 1];
        else if (tmp[i] == "-u") username = tmp[i + 1];
        else if (tmp[i] == "-p") password = tmp[i + 1];
        else if (tmp[i] == "-n") name = tmp[i + 1];
        else if (tmp[i] == "-m") mailAddress = tmp[i + 1];
        else if (tmp[i] == "-g"){
            stringstream in;
            in << tmp[i + 1];
            in >> pri;
        }
        else throw "error";
    }
    if (user_Online.find(String<21> (cur_user)) == user_Online.end()) throw "error";
    vector<User> container;
    container = userControl.find(String<21> (cur_user));
    if (container.empty()) throw "error";
    User cur = container[0];
    container.clear();
    container = userControl.find(String<21> (username));
    if (container.empty()) throw "error";
    User m_user = container[0];
    if (pri != -1 && cur.getPrivilege() <= pri) throw "error";
    if (cur.getPrivilege() > m_user.getPrivilege() || m_user == cur){
        userControl.modify_user(m_user , String<31> (password) , String<20> (name) , String<31> (mailAddress) , pri);
    }
    else throw "error";
}

void add_train(std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(10) , ' ');
    string trainID ,stations , prices , startTime , travelTimes , stopoverTimes , saleDate , type;
    int stationNum , seatNum;
    date StartTime{};
    for (int i = 0 ; i < tmp.size() ; i += 2){
        if (tmp[i] == "-i") trainID = tmp[i + 1];
        else if (tmp[i] == "-n"){
            stringstream in;
            in << tmp[i + 1];
            in >> stationNum;
        }
        else if (tmp[i] == "-m"){
            stringstream in;
            in << tmp[i + 1];
            in >> seatNum;
        }
        else if (tmp[i] == "-s") stations = tmp[i + 1];
        else if (tmp[i] == "-p") prices = tmp[i + 1];
        else if (tmp[i] == "-x") {
            startTime = tmp[i + 1];
            vector<string> s = Split(startTime , ':');
            int h , m;
            stringstream in;
            in << s[0] ; in >> h;
            in.clear();
            in << s[1] ; in >> m;
            StartTime.reset(0 , 0 , h , m);
        }
        else if (tmp[i] == "-t") travelTimes = tmp[i + 1];
        else if (tmp[i] == "-o") stopoverTimes = tmp[i + 1];
        else if (tmp[i] == "-d") saleDate = tmp[i + 1];
        else if (tmp[i] == "-y") type = tmp[i + 1];
        else throw "error";
    }
    String<40> Stations[stationNum + 1];
    int Prices[stationNum + 1] , TravelTimes[stationNum + 1] , StopoverTimes[stationNum];
    tmp.clear();
    //处理车站
    tmp = Split(stations , '|');
    for (int i = 0 ; i < tmp.size() ; ++i) Stations[i + 1] = String<40> (tmp[i]);
    tmp.clear();
    //处理票价
    tmp = Split(prices , '|');
    Prices[1] = 0;
    for (int i = 0 ; i < tmp.size() ; ++i){
        stringstream in;
        in << tmp[i];
        in >> Prices[i + 2];
        Prices[i + 2] += Prices[i + 1];
    }
    tmp.clear();
    //处理两站之间时间
    tmp = Split(travelTimes , '|');
    TravelTimes[0] = TravelTimes[1] = 0;
    for (int i = 0 ; i < tmp.size() ; ++i){
        stringstream in;
        in << tmp[i];
        in >> TravelTimes[i + 2];
        TravelTimes[i + 2] += TravelTimes[i + 1];
    }
    tmp.clear();
    //处理站台停留时间（只有两站的特判）
    StopoverTimes[0] = StopoverTimes[1] = 0;
    if (stationNum != 2){
        tmp = Split(stopoverTimes , '|');
        for (int i = 0 ; i < tmp.size() ; ++i){
            stringstream in;
            in << tmp[i];
            in >> StopoverTimes[i + 2];
            StopoverTimes[i + 2] += StopoverTimes[i + 1];
        }
    }
    tmp.clear();

    tmp = Split(saleDate , '|');
    string sale_start = tmp[0] , sale_end = tmp[1];
    vector<string> s;
    int mon , d;
    stringstream in;
    s = Split(sale_start , '-');
    in << s[0] ; in >> mon;
    in.clear();
    in << s[1] ; in >> d;
    in.clear();
    date saleStart(mon , d , 0 , 0);
    s = Split(sale_end , '-');
    in << s[0] ; in >> mon;
    in.clear();
    in << s[1] ; in >> d;
    in.clear();
    date saleEnd(mon , d , 0 , 0);

    Train t(String<21> (trainID) , Stations , stationNum , seatNum , Prices , char (type[0]) , TravelTimes , StopoverTimes , StartTime , saleStart , saleEnd , 0);
    trainControl.addTrain(t);
    trainControl.addTrainSeat(String<21> (trainID) , seatNum);
    std::cout << 0 << "\n";
}

void release_train(std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(14) , ' ');
    String<21> trainID(tmp[1]);
    vector<Train> exist_train;
    exist_train = trainControl.find(trainID);
    if (exist_train.empty()) throw "error";
    Train r_train = exist_train[0];
    trainControl.releaseTrain(r_train);
    ticketControl.addTicket(r_train);
    std::cout << 0 << "\n";
}

void query_train(std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(12) , ' ');
    string trainID , Time;
    date time{};
    for (int i = 0 ; i < tmp.size() ; i += 2){
        if (tmp[i] == "-i") trainID = tmp[i + 1];
        else if (tmp[i] == "-d"){
            Time = tmp[i + 1];
            vector<string> s = Split(Time , '-');
            int M , d;
            stringstream in;
            in << s[0] ; in >> M;
            in.clear();
            in << s[1] ; in >> d;
            in.clear();
            time.reset(M , d , 0 , 0);
        }
        else throw "error";
    }
    vector<Train> exist_train = trainControl.find(String<21> (trainID));
    if (exist_train.empty()) throw "no find";
    Train q_train = exist_train[0];
    trainControl.queryTrain(q_train , time);
}

void delete_train(std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(13) , ' ');
    String<21> trainID(tmp[1]);
    trainControl.deleteTrain(trainID);
    std::cout << 0 << "\n";
}

void query_ticket(const std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(13) , ' ');
    string Time , st , ed;
    int type = 0; // 0 for time ; 1 for cost
    date q_time{};
    for (int i = 0 ; i < tmp.size() ; i += 2){
        if (tmp[i] == "-s") st = tmp[i + 1];
        else if (tmp[i] == "-t") ed = tmp[i + 1];
        else if (tmp[i] == "-d") {
            Time = tmp[i + 1];
            vector<string> s = Split(Time , '-');
            int M , d;
            stringstream in;
            in << s[0] ; in >> M;
            in.clear();
            in << s[1] ; in >> d;
            in.clear();
            q_time.reset(M , d , 0 , 0);
        }
        else if (tmp[i] == "-p"){
            if (tmp[i + 1] == "time") type = 0;
            else type = 1;
        }
        else throw "error";
    }
    if (st == ed) {
        cout << 0 << "\n";
        return;
    }
    vector<Ticket> begin_station = ticketControl.find(String<40> (st));
    vector<Ticket> end_station = ticketControl.find(String<40> (ed));
    begin_station.sort();
    end_station.sort();

    vector<Ticket> begin_ans , end_ans;
    int p1 = 0 , p2 = 0;
    while (p1 < begin_station.size() && p2 < end_station.size()){
        if (begin_station[p1] == end_station[p2]) {
            if (begin_station[p1].StationNo >= end_station[p2].StationNo) {
                p1++ ; p2++;
                continue;
            }
            date salebegin = begin_station[p1].StartDay + begin_station[p1].StartDayTime + date(0,0,0,begin_station[p1].LeaveTime);
            date saleend = begin_station[p1].EndDay + begin_station[p1].StartDayTime + date(0,0,0,begin_station[p1].LeaveTime);
            if (cmp(salebegin , q_time) && cmp(q_time , saleend)){
                begin_ans.push_back(begin_station[p1]);
                end_ans.push_back(end_station[p2]);
            }
            p1++ ; p2++;
        }
        else if (begin_station[p1] < end_station[p2]) p1++;
        else p2++;
    }

    vector<pair<pair<int , String<21>> , int>> ans_container;
    if (type == 0){//time
        for (int i = 0 ; i < begin_ans.size() ; ++i){
            ans_container.push_back(make_pair(make_pair(end_ans[i].ArrivalTime - begin_ans[i].LeaveTime , begin_ans[i].TrainID) , i));
        }
        ans_container.sort();
        cout << ans_container.size() << "\n";
        for (int i = 0 ; i < ans_container.size() ; ++i){
            int num =  ans_container[i].second;
            cout << begin_ans[num].TrainID << " " << st << " ";
            date tmp1 = begin_ans[num].StartDay; tmp1 += begin_ans[num].StartDayTime;
            tmp1 += date(0 , 0 , 0 , begin_ans[num].LeaveTime);
            int no = q_time - tmp1;
            tmp1 += date(0 , no - 1 , 0 , 0);
            tmp1.show();
            cout << " -> " << ed << " ";
            date tmp2 = end_ans[num].StartDay; tmp2 += end_ans[num].StartDayTime;
            tmp2 += date(0 , 0 , 0 , end_ans[num].ArrivalTime);
            tmp2 += date(0 , no - 1 , 0 , 0);
            tmp2.show();
            cout << " " << end_ans[num].Price - begin_ans[num].Price;
            cout << " " << trainControl.getSeatNum(begin_ans[num].TrainID , String<40> (st) , String<40> (ed) , no) << "\n";
        }
    }
    else {//cost
        for (int i = 0 ; i < begin_ans.size() ; ++i){
            ans_container.push_back(make_pair(make_pair(end_ans[i].Price - begin_ans[i].Price , begin_ans[i].TrainID) , i));
        }
        ans_container.sort();
        cout << ans_container.size() << "\n";
        for (int i = 0 ; i < ans_container.size() ; ++i){
            int num =  ans_container[i].second;
            cout << begin_ans[num].TrainID << " " << st << " ";
            date tmp1 = begin_ans[num].StartDay; tmp1 += begin_ans[num].StartDayTime;
            tmp1 += date(0 , 0 , 0 , begin_ans[num].LeaveTime);
            int no = q_time - tmp1;
            tmp1 += date(0 , no - 1 , 0 , 0);
            tmp1.show();
            cout << " -> " << ed << " ";
            date tmp2 = end_ans[num].StartDay; tmp2 += end_ans[num].StartDayTime;
            tmp2 += date(0 , 0 , 0 , end_ans[num].ArrivalTime);
            tmp2 += date(0 , no - 1 , 0 , 0);
            tmp2.show();
            cout << " " << end_ans[num].Price - begin_ans[num].Price;
            cout << " " << trainControl.getSeatNum(begin_ans[num].TrainID , String<40> (st) , String<40> (ed) , no) << "\n";
        }
    }
}

void query_transfer(const std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(15) , ' ');
    string Time , st , ed;
    int type = 0; // 0 for time ; 1 for cost
    date q_time{};
    for (int i = 0 ; i < tmp.size() ; i += 2){
        if (tmp[i] == "-s") st = tmp[i + 1];
        else if (tmp[i] == "-t") ed = tmp[i + 1];
        else if (tmp[i] == "-d") {
            Time = tmp[i + 1];
            vector<string> s = Split(Time , '-');
            int M , d;
            stringstream in;
            in << s[0] ; in >> M;
            in.clear();
            in << s[1] ; in >> d;
            in.clear();
            q_time.reset(M , d , 0 , 0);
        }
        else if (tmp[i] == "-p"){
            if (tmp[i + 1] == "time") type = 0;
            else type = 1;
        }
        else throw "error";
    }
    ticketControl.queryTransfer(String<40> (st) , String<40> (ed) , q_time , type);
}

void buy_ticket(const std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(11) , ' ');
    string username , trainID , Time , st , ed;
    int num , isQue = 0; // 0 for false ; 1 for true
    date time{};
    for (int i = 0 ; i < tmp.size() ; i += 2){
        if (tmp[i] == "-u") username = tmp[i + 1];
        else if (tmp[i] == "-i") trainID = tmp[i + 1];
        else if (tmp[i] == "-d") {
            Time = tmp[i + 1];
            vector<string> s = Split(Time , '-');
            int M , d;
            stringstream in;
            in << s[0] ; in >> M;
            in.clear();
            in << s[1] ; in >> d;
            in.clear();
            time.reset(M , d , 0 , 0);
        }
        else if (tmp[i] == "-n"){
            stringstream in;
            in << tmp[i + 1];
            in >> num;
        }
        else if (tmp[i] == "-f") st = tmp[i + 1];
        else if (tmp[i] == "-t") ed = tmp[i + 1];
        else if (tmp[i] == "-q"){
            if (tmp[i + 1] == "false") isQue = 0;
            else isQue = 1;
        }
        else throw "error";
    }
    if (user_Online.find(String<21> (username)) == user_Online.end()) throw "error (no login)";
    if (num <= 0) throw "error";
    vector<Train> exist_train;
    exist_train = trainControl.find(String<21> (trainID));
    if (exist_train.empty()) throw "error";
    int no = userControl.user_addOrder(String<21> (username));
    ticketControl.buyTicket(String<21> (username) , exist_train[0] , String<40> (st) , String<40> (ed) , time , num , isQue , no);
}

void query_order(const std::string &cmd){
    vector<string> tmp;
    tmp = Split(cmd.substr(12) , ' ');
    String<21> username(tmp[1]);
    if (user_Online.find(username) == user_Online.end()) throw "error";
//    vector<User> exist_user = userControl.find(username);
//    if (exist_user.empty()) throw "error";
    vector<Order> exist_Order = orderControl.findOrder(username);
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
    string username;
    int num = 1;
    for (int i = 0 ; i < tmp.size() ; i += 2){
        if (tmp[i] == "-u") username = tmp[i + 1];
        else if (tmp[i] == "-n"){
            stringstream in;
            in << tmp[i + 1];
            in >> num;
        }
    }
    if (user_Online.find(String<21> (username)) == user_Online.end()) throw "error (no login)";
    vector<Order> exist_Order = orderControl.findOrder(String<21> (username));
    if (exist_Order.empty()) throw "error";
    exist_Order.sort();
    if (num > exist_Order.size() || num <= 0) throw "error";
    Order r_order = exist_Order[exist_Order.size() - num];
    if (r_order.getStatus() == 1){
        orderControl.refundOrder(String<21> (username) , r_order);
        ticketControl.que_BuyTicket(String<21> (username) , r_order);//username没用
    }
    else if (r_order.getStatus() == 2){
        orderControl.refundOrder(String<21> (username) , r_order);
    }
    else throw "error";
    cout << 0 << "\n";
    //更改order 更改余票信息 查看候补队列是否能够购买
}

void Run(std::string &command){
    if (command.substr(0 , 8) == "add_user") add_user(command);
    else if (command.substr(0 , 5) == "login") login(command);
    else if (command.substr(0 , 6) == "logout") logout(command);
    else if (command.substr(0 , 13) == "query_profile") query_profile(command);
    else if (command.substr(0 , 14) == "modify_profile") modify_profile(command);
    else if (command.substr(0 , 9) == "add_train") add_train(command);
    else if (command.substr(0 , 13) == "release_train") release_train(command);
    else if (command.substr(0 , 11) == "query_train") query_train(command);
    else if (command.substr(0 , 12) == "delete_train") delete_train(command);
    else if (command.substr(0 , 12) == "query_ticket") query_ticket(command);
    else if (command.substr(0 , 14) == "query_transfer") query_transfer(command);
    else if (command.substr(0 , 10) == "buy_ticket") buy_ticket(command);
    else if (command.substr(0 , 11) == "query_order") query_order(command);
    else if (command.substr(0 , 13) == "refund_ticket") refund_ticket(command);
    else if (command.substr(0 , 5) == "clean") Clean();
    else if (command.substr(0 , 4) == "exit") Exit();
    //to do
}

void Clean(){
//    fstream o;
//    o.open("users.dat" , ios::out);
//    o.close();
//    o.open("users_BPT.dat" , ios::out);
//    o.close();
//    o.open("Order_BPT.dat" , ios::out);
//    o.close();
//    o.open("Order.dat" , ios::out);
//    o.close();
//    o.open("pendingOrder_BPT.dat" , ios::out);
//    o.close();
//    o.open("pendingOrder.dat" , ios::out);
//    o.close();
//    o.open("tickets_BPT.dat" , ios::out);
//    o.close();
//    o.open("Ticket.dat" , ios::out);
//    o.close();
//    o.open("train_BPT.dat" , ios::out);
//    o.close();
//    o.open("Train.dat" , ios::out);
//    o.close();
//    o.open("trainSeat_BPT.dat" , ios::out);
//    o.close();
//    o.open("trainSeat.dat" , ios::out);
//    o.close();
    userControl.restart();
    ticketControl.restart();
    trainControl.restart();
    orderControl.restart();
    user_Online.clear();
    cout << 0 << endl;
}

void Exit(){
    cout << "bye\n";
    exit(0);
}