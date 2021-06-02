//
// Created by lenovo on 2021/5/22.
//
#include "Ticket.h"

Ticket::Ticket(const String<21> &trainID , int stationNo, const date &startDayTime , const date &startDay, const date &endDay , int arrivalTime , int leaveTime , int price) {
    TrainID = trainID;
    StationNo = stationNo;
    StartDayTime = startDayTime;
    StartDay = startDay;
    EndDay = endDay;
    Price = price;
    ArrivalTime = arrivalTime;
    LeaveTime = leaveTime;
}

Ticket &Ticket::operator=(const Ticket &t){
    if (this == &t) return *this;
    TrainID = t.TrainID;
    StationNo = t.StationNo;
    StartDayTime = t.StartDayTime;
    StartDay = t.StartDay;
    EndDay = t.EndDay;
    Price = t.Price;
    ArrivalTime = t.ArrivalTime;
    LeaveTime = t.LeaveTime;
    return *this;
}

Ticket::Ticket(const Ticket &t) {
    TrainID = t.TrainID;
    StationNo = t.StationNo;
    StartDayTime = t.StartDayTime;
    StartDay = t.StartDay;
    EndDay = t.EndDay;
    Price = t.Price;
    ArrivalTime = t.ArrivalTime;
    LeaveTime = t.LeaveTime;
}

bool Ticket::operator<(const Ticket &t) const {
    return TrainID < t.TrainID;
}
bool Ticket::operator>(const Ticket &t) const {
    return TrainID > t.TrainID;
}

bool Ticket::operator==(const Ticket &rhs) const {
    return TrainID == rhs.TrainID;
}

bool Ticket::operator!=(const Ticket &rhs) const {
    return !(rhs == *this);
}

bool Ticket::operator<=(const Ticket &rhs) const {
    return !(rhs < *this);
}

bool Ticket::operator>=(const Ticket &rhs) const {
    return !(*this < rhs);
}


void ticket_System::addTicket(const String<40> &station, const Ticket &t) {
    ticket_BPT.insert(station , t);
}

void ticket_System::delTicket(const String<40> &station, const Ticket &t) {
    ticket_BPT.erase(station , t);
}

void ticket_System::addTicket(const Train &t) {
    addTicket(t.Stations[1] ,Ticket (t.TrainID , 1 , t.StartDayTime ,  t.SaleDate_begin , t.SaleDate_end , 0 , 0 , 0));
    for (int i = 2 ; i <= t.StationNum ; ++i){
        Ticket tmp(t.TrainID , i ,  t.StartDayTime , t.SaleDate_begin , t.SaleDate_end , t.TravelTimeSum[i] + t.StopoverTimeSum[i - 1]
                   , t.TravelTimeSum[i] + t.StopoverTimeSum[i] , t.PriceSum[i]);
        addTicket(t.Stations[i] , tmp);
    }
}

void ticket_System::buyTicket(const String<21> &username , const Train &t, const String<40> &st, const String<40> &ed, const date &d,
                         int num, int isQue , int OrderNo) {//num为车票数
    if (t.IsRelease == 0) throw "error";
    if (num > t.SeatNum) throw "error";
    int s = 0 , e = 0;
    for (int i = 1 ; i <= t.StationNum ; ++i){
        if (t.Stations[i] == st) s = i;
        else if (t.Stations[i] == ed) e = i;
    }
    date salebegin = t.SaleDate_begin + t.StartDayTime + date(0,0,0,t.TravelTimeSum[s] + t.StopoverTimeSum[s]);
    date saleend = t.SaleDate_end + t.StartDayTime + date(0,0,0,t.TravelTimeSum[s] + t.StopoverTimeSum[s]);
    if (cmp(salebegin , d) && cmp(d , saleend)){
        if (s == 0 || e == 0) throw "error";
        if (s >= e) throw "error";
        date leave = t.SaleDate_begin; leave += t.StartDayTime;//leave为到st的时间
        date arrive = leave;
        arrive += date(0 , 0 , 0 , t.TravelTimeSum[e] + t.StopoverTimeSum[e - 1]);//arrive为到达ed的时间
        leave += date (0 , 0 , 0 , t.TravelTimeSum[s] + t.StopoverTimeSum[s]);

        int no = d - leave;
        if (no <= 0 || no > t.SaleDate_end - t.SaleDate_begin) throw "error";
        int l_ticketNum = trainControl.getSeatNum(t.TrainID , s , e , no);
        if (l_ticketNum >= num) {
            trainControl.modifySeat(t.TrainID , s , e , no , num);
            long long cost = (t.PriceSum[e] - t.PriceSum[s]) * num;
            cout << cost << "\n";
            Order o(OrderNo ,0 , 1 , t.TrainID , username , st , ed , leave + date(0, no - 1, 0, 0) , arrive + date(0, no - 1, 0, 0)
                    , t.PriceSum[e] - t.PriceSum[s] , no , num);
            orderControl.addOrder(username , o);
        }
        else if (isQue == 0) {
            throw "error no seat";
        }
        else {
            int pendingNum = trainControl.addPendingOrderNum(t , no);
            Order o(OrderNo , pendingNum , 2 , t.TrainID , username , st , ed , leave + date(0, no - 1, 0, 0) , arrive + date(0, no - 1, 0, 0)
                    , t.PriceSum[e] - t.PriceSum[s] , no , num);
            orderControl.addOrder(username , o);
            orderControl.addPendingOrder(o , no , pendingNum);
            cout << "queue\n";
        }
    }
    else throw "error";
}

void ticket_System::que_BuyTicket(const String<21> &username , const Order &refund_o) {
    vector<pair<int , Order>> tmp = orderControl.findPendingOrder(make_pair(refund_o.TrainID , refund_o.No));
    tmp.sort();
    for (int i = 0 ; i < tmp.size() ; ++i){
        pair<int , Order> t = tmp[i];
        Order o = t.second;
        int left = trainControl.getSeatNum(o.TrainID , o.From , o.To , o.No);
        if (o.Num <= left) {
            trainControl.modifySeat(o.TrainID , o.From , o.To , o.No , o.Num);
            orderControl.delPendingOrder(o , o.No , t.first);
            Order tmpo = o;
            tmpo.changeStatus(1);
            orderControl.modifyOrder(tmpo.Username , o , tmpo);
        }
    }
}

vector<pair<Ticket, Ticket>> ticket_System::queryTicket(const String<40> &st , const String<40> &ed , const date &d) {
    vector<Ticket> begin_station = ticket_BPT.find(String<40> (st));
    vector<Ticket> end_station = ticket_BPT.find(String<40> (ed));
    begin_station.sort();
    end_station.sort();

    vector<pair<Ticket , Ticket>> ans;
    int p1 = 0 , p2 = 0;
    while (p1 < begin_station.size() && p2 < end_station.size()){
        if (begin_station[p1] == end_station[p2]) {
            if (begin_station[p1].StationNo >= end_station[p2].StationNo) {
                p1++ ; p2++;
                continue;
            }
            date saleend = begin_station[p1].EndDay + begin_station[p1].StartDayTime + date(0,0,0,begin_station[p1].LeaveTime);
            if (d <= saleend){
                ans.push_back(make_pair(begin_station[p1] , end_station[p2]));
            }
            p1++ ; p2++;
        }
        else if (begin_station[p1] < end_station[p2]) p1++;
        else p2++;
    }
    return ans;
}

vector<pair<pair<int , int> , pair<Ticket , Ticket>>> ticket_System::queryTransfer(const Train &t , const String<40> &st , const String<40> &ed , const date &d , int type) {
    vector<pair<pair<int , int> , pair<Ticket , Ticket>>> ans;
    int start = 0;
    int lowest = 1e9 + 7;
    int lowestFirstTrainTime = 1e9 + 7;
    for (int i = 1 ; i <= t.StationNum ; ++i){//枚举中转站
        if (t.Stations[i] == st) {
            start = i;
            continue;
        }
        if (start == 0) continue;
        vector<pair<Ticket , Ticket>> tmp1;
        if (t.Stations[i] != ed){
            date leaveStart = t.StartDayTime + t.SaleDate_begin + date (0,0,0,t.TravelTimeSum[start] + t.StopoverTimeSum[start]);
            date arriveTrans = t.StartDayTime + t.SaleDate_begin + date (0,0,0,t.TravelTimeSum[i] + t.StopoverTimeSum[i-1]);
            int trainNo = d - leaveStart;
            arriveTrans += date (0,trainNo - 1,0,0);
            tmp1 = queryTicket(t.Stations[i] , ed , arriveTrans);//查询中转站到ed的所有火车
        }
        if (tmp1.empty()) continue;
        if (type == 0){//time
            for (int j = 0 ; j < tmp1.size() ; ++j){
                Ticket trans = tmp1[j].first;
                Ticket end = tmp1[j].second;

                if (trans.TrainID == t.TrainID) continue;
                date leaveStart = t.StartDayTime + t.SaleDate_begin + date (0,0,0,t.TravelTimeSum[start] + t.StopoverTimeSum[start]);
                date arriveTrans = t.StartDayTime + t.SaleDate_begin + date (0,0,0,t.TravelTimeSum[i] + t.StopoverTimeSum[i-1]);
                int trainNo = d - leaveStart;
                arriveTrans += date (0,trainNo - 1,0,0);
                leaveStart += date (0,trainNo - 1,0,0);

                date Leavetrans = trans.StartDay + trans.StartDayTime + date(0, 0, 0, trans.LeaveTime);
                date arriveEnd = trans.StartDay + trans.StartDayTime + date(0,0,0, end.ArrivalTime);
                int trainNo2;
                if (arriveTrans <= Leavetrans) trainNo2 = 1;
                else {
                    trainNo2 = arriveTrans - Leavetrans;//比较tmp2和tmp3的发车时间
                    if (date(0, 0, Leavetrans.hour , Leavetrans.minute) < date (0, 0, arriveTrans.hour , arriveTrans.minute)) trainNo2++;
                }
                Leavetrans += date(0, trainNo2 - 1 , 0, 0);
                arriveEnd += date(0, trainNo2 - 1 ,0,0);
                int wait = calMinute(arriveEnd , leaveStart);
                if (wait < lowest){
                    lowest = wait;
                    int FirstTrainTime = t.TravelTimeSum[i] + t.StopoverTimeSum[i-1] - t.TravelTimeSum[start] - t.StopoverTimeSum[start];
                    lowestFirstTrainTime = FirstTrainTime;
                    ans.clear();
                    ans.push_back(make_pair(make_pair(wait , i) , make_pair(trans , end)));
                }
                else if (wait == lowest){
                    int FirstTrainTime = t.TravelTimeSum[i] + t.StopoverTimeSum[i-1] - t.TravelTimeSum[start] - t.StopoverTimeSum[start];
                    if (FirstTrainTime < lowestFirstTrainTime){
                        lowestFirstTrainTime = FirstTrainTime;
                        ans.clear();
                        ans.push_back(make_pair(make_pair(wait , i) , make_pair(trans , end)));
                    }
                }
            }
        }
        else {//cost
            for (int j = 0 ; j < tmp1.size() ; ++j){
                Ticket trans = tmp1[j].first;
                Ticket end = tmp1[j].second;
                if (trans.TrainID == t.TrainID) continue;
                int cost = abs(t.PriceSum[i] - t.PriceSum[start] + end.Price - trans.Price);
                if (cost < lowest) {
                    lowest = cost;
                    int FirstTrainTime = t.TravelTimeSum[i] + t.StopoverTimeSum[i-1] - t.TravelTimeSum[start] - t.StopoverTimeSum[start];
                    lowestFirstTrainTime = FirstTrainTime;
                    ans.clear();
                    ans.push_back(make_pair(make_pair(cost , i) , make_pair(trans , end))) ;
                }
                else if (cost == lowest){
                    int FirstTrainTime = t.TravelTimeSum[i] + t.StopoverTimeSum[i-1] - t.TravelTimeSum[start] - t.StopoverTimeSum[start];
                    if (FirstTrainTime < lowestFirstTrainTime){
                        lowestFirstTrainTime = FirstTrainTime;
                        ans.clear();
                        ans.push_back(make_pair(make_pair(cost , i) , make_pair(trans , end)));
                    }
                }
            }
        }
    }
    vector<pair<pair<int , int> , pair<Ticket , Ticket>>> ans_vec;
    if (!ans.empty()){
        ans_vec.push_back(ans[0]);
    }
    return ans_vec;
}

void ticket_System::queryTransfer(const String<40> &st , const String<40> &ed , const date &d , int type) {
    vector<Ticket> begin_station = ticket_BPT.find(st);
    vector<pair<Train , pair<Ticket , Ticket>>> ans;
    int lowest = 1e9 + 7;
    int lowestFirstTrainTime = 1e9 + 7;
    for (int i = 0 ; i < begin_station.size() ; ++i){
        Ticket ticket = begin_station[i];
        date salebegin = begin_station[i].StartDay + begin_station[i].StartDayTime + date(0,0,0,begin_station[i].LeaveTime);
        date saleend = begin_station[i].EndDay + begin_station[i].StartDayTime + date(0,0,0,begin_station[i].LeaveTime);
        if (cmp(salebegin , d) && cmp(d , saleend)){
            vector<Train> train_container = trainControl.find(ticket.TrainID);
            Train t = train_container[0];
            vector<pair<pair<int , int> , pair<Ticket , Ticket>>> way = queryTransfer(t , String<40> (st) , String<40> (ed) , d , type);
            if (!way.empty()){
                if (way[0].first.first < lowest){
                    lowest = way[0].first.first;
                    lowestFirstTrainTime = t.TravelTimeSum[way[0].first.second] + t.StopoverTimeSum[way[0].first.second - 1] - t.TravelTimeSum[ticket.StationNo] - t.StopoverTimeSum[ticket.StationNo];
                    ans.clear();
                    ans.push_back(make_pair(t , way[0].second));
                }
                else if (way[0].first.first == lowest){
                    int FirstTrainTime = t.TravelTimeSum[way[0].first.second] + t.StopoverTimeSum[way[0].first.second - 1] - t.TravelTimeSum[ticket.StationNo] - t.StopoverTimeSum[ticket.StationNo];
                    if (FirstTrainTime < lowestFirstTrainTime){
                        lowestFirstTrainTime = t.TravelTimeSum[way[0].first.second] + t.StopoverTimeSum[way[0].first.second - 1] - t.TravelTimeSum[ticket.StationNo] - t.StopoverTimeSum[ticket.StationNo];
                        ans.clear();
                        ans.push_back(make_pair(t , way[0].second));
                    }
                }
            }
        }
    }
    if (ans.empty()) {
        cout << 0 << "\n";
        return;
    }

    pair<Train , pair<Ticket , Ticket>> answer = ans[0];
    Train firstTrain = answer.first;
    vector<Train> two = trainControl.find(answer.second.first.TrainID);
    Train secondTrain = two[0];
    int start , trans;
    String<40> transfer = secondTrain.Stations[answer.second.first.StationNo];
    for (int i = 1 ; i <= firstTrain.StationNum ; ++i){
        if (firstTrain.Stations[i] == st) start = i;
        else if (firstTrain.Stations[i] == transfer) trans = i;
    }

    date tmp1 = firstTrain.StartDayTime; tmp1 += firstTrain.SaleDate_begin;
    date tmp2 = tmp1;//tmp2为到trans时间
    tmp2 += date(0 , 0 , 0 , firstTrain.TravelTimeSum[trans] + firstTrain.StopoverTimeSum[trans - 1]);
    tmp1 += date (0 , 0 , 0 , firstTrain.TravelTimeSum[start] + firstTrain.StopoverTimeSum[start]);
    int no = d - tmp1;
    tmp1 += date(0 , no - 1 , 0 , 0);
    tmp2 += date(0 , no - 1 , 0 , 0);

    cout << firstTrain.TrainID << " " << st << " ";
    tmp1.show();
    cout << " -> " << transfer << " ";
    tmp2.show();
    cout << " " << abs(firstTrain.PriceSum[trans] - firstTrain.PriceSum[start]) << " ";
    cout << trainControl.getSeatNum(firstTrain.TrainID , start , trans , no) << "\n";

    cout << secondTrain.TrainID << " " << transfer << " ";
    date tmp3 = secondTrain.StartDayTime; tmp3 += secondTrain.SaleDate_begin;
    date tmp4 = tmp3;
    tmp3 += date (0 , 0 , 0 ,answer.second.first.LeaveTime);
    tmp4 += date (0 , 0 , 0 ,answer.second.second.ArrivalTime);
    int no2;
    if (tmp2 <= tmp3) no2 = 1;
    else {
        no2 = tmp2 - tmp3;//比较tmp2和tmp3的发车时间
        if (date(0,0, tmp3.hour , tmp3.minute) < date (0,0, tmp2.hour , tmp2.minute)) no2++;
    }
    tmp3 += date (0 , no2 - 1, 0 , 0);
    tmp4 += date (0 , no2 - 1, 0 , 0);
    tmp3.show();
    cout << " -> " << ed << " ";
    tmp4.show();
    cout << " " << abs(answer.second.first.Price - answer.second.second.Price) << " ";
    cout << trainControl.getSeatNum(secondTrain.TrainID , transfer , ed , no2) << "\n";
}

vector<Ticket> ticket_System::find(const String<40> &trainID) {
    vector<Ticket> tmp = ticket_BPT.find(trainID);
    return tmp;
}

void ticket_System::restart() {
    ticket_BPT.remake("tickets_BPT.dat" , "Ticket.dat");
}
