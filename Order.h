//
// Created by lenovo on 2021/5/22.
//

#ifndef TRAINSTATION_ORDER_H
#define TRAINSTATION_ORDER_H
#include "String.hpp"
#include "date.hpp"
#include "BPT.hpp"
using namespace RA;

class Order{
    friend class Order_Control;
    friend class Ticket_Control;
private:
    int userOrderNo; // 记录用户购买顺序的tag
    int PendingOrderNo; // 记录候补队列顺序的tag
    int Status{}; // 1-success 2-pending 3-refunded
    String<21> TrainID;
    String<21> Username;
    String<40> From , To;
    date Leave{} , Arrive{};//leave为离开from的时间 ， arrive为到达to的时间
    int Price{};
    int StationNo; // 车次
    int TicketNum{};

public:
    Order() = default;
    Order(int OrderNo, int pendingOrderNo, int status, const String<21> &trainId, const String<21> &username, const String<40> &from,
          const String<40> &to, const date &leave, const date &arrive, int price, int stationNo, int ticketNum);

    Order &operator=(const Order &o);

    bool operator==(const Order &rhs) const;
    bool operator!=(const Order &rhs) const;

    bool operator<(const Order &rhs) const;
    bool operator>(const Order &rhs) const;
    bool operator<=(const Order &rhs) const;
    bool operator>=(const Order &rhs) const;

    int getStatus() const;
    void changeStatus(int s);

    void show() const;
};



class Order_Control{
private:
    BPlusTree<int , Order, 200, 0, 0> userOrders_BPT;
    BPlusTree<pair<String<21> , int> , pair<int , Order>, 200, 0, 0> pendingOrder;

public:
    Order_Control(){
        userOrders_BPT.initialize("Order_BPT.dat" , "Order.dat");
        pendingOrder.initialize("pendingOrder_BPT.dat" , "pendingOrder.dat");//pendingOrder_BPT pair(TrainID , 班次数) —— pair(pendingNum , Order)
    }
    void restart();
    vector<Order> findOrder(const String<21> &username);
    vector<pair<int , Order>> findPendingOrder(const pair<String<21> , int> &t);

    void addOrder(const String<21> &username, const Order &o);
    void modifyOrder(const String<21> &username, const Order &old , const Order &New);
    void refundOrder(const String<21> &username, const Order &o);
    void addPendingOrder(const Order &o , int no , int pendingNum);
    void delPendingOrder(const Order &o , int no , int pendingNum);
};

#endif //TRAINSTATION_ORDER_H
