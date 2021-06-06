//
// Created by lenovo on 2021/5/22.
//

#ifndef TRAINSTATION_TICKET_H
#define TRAINSTATION_TICKET_H
#include "String.hpp"
#include "BPT.hpp"
#include "date.hpp"
#include "Train.h"
#include "Map.hpp"
#include "Order.h"
#include <algorithm>
using namespace RA;

extern Train_Control trainSystem;
extern Order_Control orderSystem;

class Ticket{
    friend class Ticket_Control;

public:
    String<21> TrainID;
    int StationNo{};
    date FirstTrain_leaveDate{} , LastTrain_leaveDate{};
    int Price{};

    Ticket() = default;
    Ticket(const String<21> &trainID , int stationNo, const date &firstTrain_leaveDate , const date &lastTrain_leaveDate, int price);
    Ticket(const Ticket &t);
    Ticket &operator=(const Ticket &t);

    bool operator==(const Ticket &rhs) const;

    bool operator!=(const Ticket &rhs) const;

    bool operator<(const Ticket &t) const;
    bool operator>(const Ticket &t) const;
    bool operator<=(const Ticket &rhs) const;
    bool operator>=(const Ticket &rhs) const;
};

class inf_ticket{
    friend class Ticket_Control;
private:
    int Value{};
    int Cost{};
    String<21> TrainID;
    String<40> From , To;
    int SeatNum{};
    date LeaveTime{} , ArriveTime{};
public:
    inf_ticket() = default;
    inf_ticket(int value, int cost, const String<21> &trainId, const String<40> &from, const String<40> &to, int seatNum, const date &leaveTime, const date &arriveTime);

    void show();

    bool operator<(const inf_ticket &rhs) const;

    bool operator>(const inf_ticket &rhs) const;

    bool operator<=(const inf_ticket &rhs) const;

    bool operator>=(const inf_ticket &rhs) const;
};

class Ticket_Control{
    friend class Train;
private:
    BPlusTree<int , Ticket, 100, 5000, 0> ticket_BPT;// pair(车站，发车日期) —— 车票

    void addTicket(const String<40> &station , const Ticket &t);
    void delTicket(const String<40> &station , const Ticket &t);
public:
    Ticket_Control(){
        ticket_BPT.initialize("tickets_BPT.dat" , "Ticket.dat");
    };

    void restart();
    vector<Ticket> find(const String<40> &trainID);

    void addTicket(const Train &t);
    void buyTicket(const String<21> &username , const String<21> &trainID , const String<40> &st , const String<40> &ed , const date &d , int Stationnum , int isQue , int OrderNo);
    void queryTicket(const String<40> &st , const String<40> &ed , const date &d , int type);
    void queryTransfer(const String<40> &st , const String<40> &ed , const date &d , int type);
    void que_BuyTicket(const String<21> &username , const Order &refund_o);
};
#endif //TRAINSTATION_TICKET_H
