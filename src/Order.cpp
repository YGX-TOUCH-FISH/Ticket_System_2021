//
// Created by lenovo on 2021/5/22.
//

#include "String.hpp"
#include "date.hpp"
#include "Order.h"
#include "Train.h"

extern Train_Control trainSystem;

//TODO——————————————————————————————————————————Order类函数————————————————————————————————————————————————————————————//
Order::Order(int orderNo , int pendingOrderNo , int status, const RA::String<21> &trainId, const String<21> &username,
             const RA::String<40> &from, const RA::String<40> &to,
             const date &leave, const date &arrive, int price, int stationNo , int ticketNum)
             : userOrderNo(orderNo) , PendingOrderNo(pendingOrderNo), Status(status), TrainID(trainId), Username(username),
               From(from), To(to), Arrive(arrive), Leave(leave), Price(price), StationNo(stationNo), TicketNum(ticketNum) {}

void Order::changeStatus(int s) {
    Status = s;
}

Order &Order::operator=(const Order &o) {
    if (this == &o) return *this;
    userOrderNo = o.userOrderNo;
    PendingOrderNo = o.PendingOrderNo;
    Status = o.Status;
    TrainID = o.TrainID;
    Username = o.Username;
    From = o.From;
    To = o.To;
    Arrive = o.Arrive;
    Leave = o.Leave;
    Price = o.Price;
    StationNo = o.StationNo;
    TicketNum = o.TicketNum;
    return *this;
}

bool Order::operator==(const Order &rhs) const {
    return userOrderNo == rhs.userOrderNo &&
           PendingOrderNo == rhs.PendingOrderNo &&
           Status == rhs.Status &&
           TrainID == rhs.TrainID &&
           Username == rhs.Username &&
           From == rhs.From &&
           To == rhs.To &&
           Arrive == rhs.Arrive &&
           Leave == rhs.Leave &&
           Price == rhs.Price &&
           TicketNum == rhs.TicketNum;
}

bool Order::operator!=(const Order &rhs) const {
    return !(rhs == *this);
}

void Order::show() const{
    if (Status == 1) cout << "[success] ";
    else if (Status == 2) cout << "[pending] ";
    else cout << "[refunded] ";
    cout << TrainID << " " << From << " ";
    Leave.show();
    cout << " -> " << To << " ";
    Arrive.show();
    cout << " " << Price << " " << TicketNum << '\n';
}

bool Order::operator<(const Order &rhs) const {
    return userOrderNo < rhs.userOrderNo;
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
    return Status;
}


//TODO—————————————————————————————————————Order_Control类函数—————————————————————————————————————————————————————————//
void Order_Control::addOrder(const String<21> &username , const Order &o) {
    userOrders_BPT.insert(username.hash_value , o);
}

void Order_Control::refundOrder(const String<21> &username, const Order &o) {
    Order newOrder = o;
    newOrder.changeStatus(3);
    userOrders_BPT.modify(username.hash_value , o , newOrder);
    if (o.Status == 1){
        trainSystem.modifySeat(o.TrainID , o.From , o.To , o.StationNo , -o.TicketNum);
    }
    else {
        delPendingOrder(o , o.StationNo , o.PendingOrderNo);
    }
}

void Order_Control::addPendingOrder(const Order &o , int no , int pendingNum) {
    pendingOrder.insert(make_pair(o.TrainID.hash_value , no) , make_pair(pendingNum , o));
}

void Order_Control::delPendingOrder(const Order &o , int no , int pendingNum) {
    pendingOrder.erase(make_pair(o.TrainID.hash_value , no) , make_pair(pendingNum , o));
}

vector<Order> Order_Control::findOrder(const String<21> &username) {
    vector<Order> tmp = userOrders_BPT.find(username.hash_value);
    return tmp;
}

vector<pair<int , Order>> Order_Control::findPendingOrder(const pair<int, int> &t) {
    vector<pair<int , Order>> tmp = pendingOrder.find(t);
    return tmp;
}

void Order_Control::modifyOrder(const String<21> &username, const Order &old, const Order &New) {
    userOrders_BPT.modify(username.hash_value , old , New);
}

void Order_Control::restart() {
    userOrders_BPT.remake("Order_BPT.dat" , "Order.dat");
    pendingOrder.remake("pendingOrder_BPT.dat" , "pendingOrder.dat");
}