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
private:
    int OrderNo;
    int PendingOrderNo;
    int status{}; // 1-success 2-pending 3-refunded
    String<21> TrainID;
    String<21> Username;
    String<40> From , To;
    date Leave{} , Arrive{};//leave为离开from的时间 ， arrive为到达to的时间
    int Price{};
    int No; // 车次
    int Num{};
public:
    Order() = default;
    Order(int OrderNo ,int pendingOrderNo , int status, const String<21> &trainId, const String<21> &username , const String<40> &from, const String<40> &to,
          const date &leave, const date &arrive, int price, int no , int num);
    void changeStatus(int s);

    int getStatus() const;

    Order &operator=(const Order &o);

    bool operator==(const Order &rhs) const;

    bool operator!=(const Order &rhs) const;

    bool operator<(const Order &rhs) const;
    bool operator>(const Order &rhs) const;
    bool operator<=(const Order &rhs) const;
    bool operator>=(const Order &rhs) const;

    void show() const;

    friend class Order_System;
    friend class ticket_System;
};



class Order_System{
private:
    BPlusTree<String<21> , Order> userOrders_BPT;
    BPlusTree<pair<String<21> , int> , pair<int , Order>> pendingOrder;

public:
    Order_System(){
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
