//
// Created by lenovo on 2021/5/22.
//

#include "String.hpp"
#include "date.hpp"
#include "Order.h"
#include "Train.h"


extern Train_System trainControl;

Order::Order(int orderNo ,int pendingOrderNo , int status, const RA::String<21> &trainId, const String<21> &username, const RA::String<40> &from, const RA::String<40> &to,
             const date &leave,  const date &arrive,  int price, int no , int num)
             : OrderNo(orderNo) , PendingOrderNo(pendingOrderNo), status(status), TrainID(trainId), Username(username) , From(from), To(to),
               Arrive(arrive), Leave(leave), Price(price), No(no), Num(num) {}

void Order::changeStatus(int s) {
    status = s;
}

Order &Order::operator=(const Order &o) {
    if (this == &o) return *this;
    OrderNo = o.OrderNo;
    PendingOrderNo = o.PendingOrderNo;
    status = o.status;
    TrainID = o.TrainID;
    Username = o.Username;
    From = o.From;
    To = o.To;
    Arrive = o.Arrive;
    Leave = o.Leave;
    Price = o.Price;
    No = o.No;
    Num = o.Num;
    return *this;
}

bool Order::operator==(const Order &rhs) const {
    return OrderNo == rhs.OrderNo &&
           PendingOrderNo == rhs.PendingOrderNo &&
           status == rhs.status &&
           TrainID == rhs.TrainID &&
           Username == rhs.Username &&
           From == rhs.From &&
           To == rhs.To &&
           Arrive == rhs.Arrive &&
           Leave == rhs.Leave &&
           Price == rhs.Price &&
           Num == rhs.Num;
}

bool Order::operator!=(const Order &rhs) const {
    return !(rhs == *this);
}

void Order::show() const{
    if (status == 1) cout << "[success] ";
    else if (status == 2) cout << "[pending] ";
    else cout << "[refunded] ";
    cout << TrainID << " " << From << " ";
    Leave.show();
    cout << " -> " << To << " ";
    Arrive.show();
    cout << " " << Price << " " << Num << endl;
}

bool Order::operator<(const Order &rhs) const {
    return OrderNo < rhs.OrderNo;
}

bool Order::operator>(const Order &rhs) const {
    return rhs < *this;
}

bool Order::operator<=(const Order &rhs) const {
    return !(rhs < *this);
}

bool Order::operator>=(const Order &rhs) const {
    return !(*this < rhs);
}

int Order::getStatus() const {
    return status;
}

void Order_System::addOrder(const String<21> &username , const Order &o) {
    userOrders_BPT.insert(username , o);
}

void Order_System::refundOrder(const String<21> &username, const Order &o) {
    Order newOrder = o;
    newOrder.changeStatus(3);
    userOrders_BPT.modify(username , o , newOrder);
    if (o.status == 1){
        trainControl.modifySeat(o.TrainID , o.From , o.To , o.No , -o.Num);
    }
    else {
        delPendingOrder(o , o.No , o.PendingOrderNo);
    }
}

void Order_System::addPendingOrder(const Order &o , int no , int pendingNum) {
    pendingOrder.insert(make_pair(o.TrainID , no) , make_pair(pendingNum , o));
}

void Order_System::delPendingOrder(const Order &o , int no , int pendingNum) {
    pendingOrder.erase(make_pair(o.TrainID , no) , make_pair(pendingNum , o));
}

vector<Order> Order_System::findOrder(const String<21> &username) {
    vector<Order> tmp = userOrders_BPT.find(username);
    return tmp;
}

vector<pair<int , Order>> Order_System::findPendingOrder(const pair<String<21>, int> &t) {
    vector<pair<int , Order>> tmp = pendingOrder.find(t);
    return tmp;
}

void Order_System::modifyOrder(const String<21> &username, const Order &old, const Order &New) {
    userOrders_BPT.modify(username , old , New);
}

void Order_System::restart() {
    userOrders_BPT.remake("Order_BPT.dat" , "Order.dat");
    pendingOrder.remake("pendingOrder_BPT.dat" , "pendingOrder.dat");
}
