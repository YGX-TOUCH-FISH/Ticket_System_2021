//
// Created by lenovo on 2021/5/22.
//

#include "String.hpp"
#include "date.hpp"
#include "Order.h"
#include "Train.h"


extern Train_System trainControl;

Order::Order(const date &buyTime , int status, const RA::String<21> &trainId, const RA::String<40> &from, const RA::String<40> &to,
             const date &leave,  const date &arrive,  int price, int no , int num)
             : buy_Time(buyTime) , status(status), TrainID(trainId), From(from), To(to),
               Arrive(arrive), Leave(leave), Price(price), No(no), Num(num) {}

void Order::changeStatus(int s) {
    status = s;
}

Order &Order::operator=(const Order &o) {
    if (this == &o) return *this;
    buy_Time = o.buy_Time;
    status = o.status;
    TrainID = o.TrainID;
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
    return buy_Time == rhs.buy_Time &&
           status == rhs.status &&
           TrainID == rhs.TrainID &&
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
    return buy_Time < rhs.buy_Time;
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

void Order_System::addOrder(const String<21> &username , const Order &o) {
    userOrders_BPT.insert(username , o);
}

void Order_System::refundOrder(const String<21> &username, const Order &o) {
    if (o.status == 2 || o.status == 3) throw "error";
    Order newOrder = o;
    newOrder.changeStatus(2);
    userOrders_BPT.modify(username , o , newOrder);
    trainControl.modifySeat(o.TrainID , o.From , o.To , o.No , o.Num);
}

void Order_System::addPendingOrder(const String<21> &username, const Order &o , int no) {
    pendingOrder.insert(make_pair(o.TrainID , no) , make_pair(username , o));
}

void Order_System::delPendingOrder(const String<21> &username, const Order &o , int no) {
    pendingOrder.erase(make_pair(o.TrainID , no) , make_pair(username , o));
}

vector<Order> Order_System::findOrder(const String<21> &username) {
    vector<Order> tmp = userOrders_BPT.find(username);
    return tmp;
}

vector<pair<String<21>, Order>> Order_System::findPendingOrder(const pair<String<21>, int> &t) {
    vector<pair<String<21>, Order>> tmp = pendingOrder.find(t);
    return tmp;
}
