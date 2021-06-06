//
// Created by lenovo on 2021/5/22.
//
#include "Ticket.h"

//TODO———————————————————————————————————————————class Ticket—————————————————————————————————————————————————————————//
Ticket::Ticket(const String<21> &trainID , int stationNo, const date &firstTrain_leaveDate , const date &lastTrain_leaveDate, int price) {
    TrainID = trainID;
    StationNo = stationNo;
    FirstTrain_leaveDate = firstTrain_leaveDate;
    LastTrain_leaveDate = lastTrain_leaveDate;
    Price = price;
}

Ticket &Ticket::operator=(const Ticket &t){
    if (this == &t) return *this;
    TrainID = t.TrainID;
    StationNo = t.StationNo;
    FirstTrain_leaveDate = t.FirstTrain_leaveDate;
    LastTrain_leaveDate = t.LastTrain_leaveDate;
    Price = t.Price;
    return *this;
}

Ticket::Ticket(const Ticket &t) {
    TrainID = t.TrainID;
    StationNo = t.StationNo;
    FirstTrain_leaveDate = t.FirstTrain_leaveDate;
    LastTrain_leaveDate = t.LastTrain_leaveDate;
    Price = t.Price;
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

//TODO———————————————————————————————————class Ticket_Control—————————————————————————————————————————————————————————//
void Ticket_Control::restart() {
    ticket_BPT.remake("tickets_BPT.dat" , "Ticket.dat");
}

vector<Ticket> Ticket_Control::find(const String<40> &trainID) {
    vector<Ticket> tmp = ticket_BPT.find(trainID.hash_value);
    return tmp;
}

void Ticket_Control::addTicket(const String<40> &station, const Ticket &t) {
    ticket_BPT.insert(station.hash_value , t);
}

void Ticket_Control::addTicket(const Train &t) {
    date first = t.SaleDate_begin + t.StartDayTime;
    date last = t.SaleDate_end + t.StartDayTime;
    addTicket(t.Stations[1] ,Ticket (t.TrainID , 1 , first , last , 0));
    for (int i = 2 ; i <= t.StationNum ; ++i){
        Ticket tmp (t.TrainID, i, first + date(0,0,0,t.TravelTimeSum[i] + t.StopoverTimeSum[i]),
                   last + date(0,0,0,t.TravelTimeSum[i] + t.StopoverTimeSum[i]) , t.PriceSum[i]);
        addTicket(t.Stations[i] , tmp);
    }
}

void Ticket_Control::delTicket(const String<40> &station, const Ticket &t) {
    ticket_BPT.erase(station.hash_value , t);
}

void Ticket_Control::buyTicket(const String<21> &username, const String<21> &trainID, const String<40> &st, const String<40> &ed,
                               const date &d, int num, int isQue, int OrderNo) {//num为车票数
    vector<Train> exist_train = trainSystem.findTrain(String<21>(trainID));
    if (exist_train.empty()) throw "error";
    Train t = exist_train[0];
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
        int l_ticketNum = trainSystem.getSeatNum(t.TrainID , s , e , no);
        if (l_ticketNum >= num) {
            trainSystem.modifySeat(t.TrainID , s , e , no , num);
            long long cost = (t.PriceSum[e] - t.PriceSum[s]) * num;
            cout << cost << "\n";
            Order o(OrderNo ,0 , 1 , t.TrainID , username , st , ed , leave + date(0, no - 1, 0, 0) , arrive + date(0, no - 1, 0, 0)
                    , t.PriceSum[e] - t.PriceSum[s] , no , num);
            orderSystem.addOrder(username , o);
        }
        else if (isQue == 0) {
            throw "error no seat";
        }
        else {
            int pendingNum = trainSystem.addPendingOrderNum(t.TrainID , no);
            Order o(OrderNo , pendingNum , 2 , t.TrainID , username , st , ed , leave + date(0, no - 1, 0, 0) , arrive + date(0, no - 1, 0, 0)
                    , t.PriceSum[e] - t.PriceSum[s] , no , num);
            orderSystem.addOrder(username , o);
            orderSystem.addPendingOrder(o , no , pendingNum);
            cout << "queue\n";
        }
    }
    else throw "error";
}

void Ticket_Control::que_BuyTicket(const String<21> &username , const Order &refund_o) {// 候补队列购票
    vector<pair<int , Order>> tmp = orderSystem.findPendingOrder(make_pair(refund_o.TrainID , refund_o.StationNo));
    tmp.sort();
    for (int i = 0 ; i < tmp.size() ; ++i){
        pair<int , Order> t = tmp[i];
        Order o = t.second;
        vector<Train> con = trainSystem.findTrain(o.TrainID);
        Train train = con[0];
        int s = 0 , e = 0;
        for (int j = 1 ; j <= train.StationNum ; ++j){
            if (train.Stations[j] == o.From) s = j;
            if (train.Stations[j] == o.To) e = j;
        }
        int left = trainSystem.getSeatNum(o.TrainID , s , e , o.StationNo);
        if (o.TicketNum <= left) {
            trainSystem.modifySeat(o.TrainID , s , e , o.StationNo , o.TicketNum);
            orderSystem.delPendingOrder(o , o.StationNo , t.first);
            Order tmpo = o;
            tmpo.changeStatus(1);
            orderSystem.modifyOrder(tmpo.Username , o , tmpo);
        }
    }
}

void Ticket_Control::queryTransfer(const String<40> &st , const String<40> &ed , const date &d , int type) {
    vector<Ticket> start_station = ticket_BPT.find(st.hash_value);
    vector<Ticket> end_station = ticket_BPT.find(ed.hash_value);
    start_station.sort();
    end_station.sort();

    int lowest = 1e9 + 7;
    int lowestFirstTrainTime = 1e9 + 7;
    Train ans_FirstTrain;
    pair<int , int> firstTrainStation;
    pair<Ticket , Ticket> secondTrainTicket;
    inf_ticket tmp1 , tmp2;


    for (int i = 0 ; i < start_station.size() ; ++i){
        if (cmp(start_station[i].FirstTrain_leaveDate , d) && cmp(d , start_station[i].LastTrain_leaveDate)){
            vector<Train> train_container = trainSystem.findTrain(start_station[i].TrainID);
            Train firstTrain = train_container[0];
            int start = start_station[i].StationNo;
            for (int j = start + 1 ; j <= firstTrain.StationNum ; ++j){//枚举中转站
                date leaveStart = start_station[i].FirstTrain_leaveDate;
                date arriveTrans = firstTrain.StartDayTime + firstTrain.SaleDate_begin + date (0,0,0,firstTrain.TravelTimeSum[j] + firstTrain.StopoverTimeSum[j-1]);
                int trainNo1 = d - leaveStart;
                arriveTrans += date (0, trainNo1 - 1, 0, 0);
                leaveStart += date (0, trainNo1 - 1, 0, 0);

                vector<Ticket> trans_station;
                trans_station.clear();
                trans_station = ticket_BPT.find(firstTrain.Stations[j].hash_value);
                trans_station.sort();

                vector<pair<Ticket , Ticket>> ans;
                ans.clear();
                int p1 = 0 , p2 = 0;
                while (p1 < trans_station.size() && p2 < end_station.size()){
                    if (trans_station[p1] == end_station[p2]) {
                        if (trans_station[p1].StationNo >= end_station[p2].StationNo || trans_station[p1].TrainID == firstTrain.TrainID) {
                            p1++ ; p2++;
                            continue;
                        }
                        if (arriveTrans <= trans_station[p1].LastTrain_leaveDate){
                            ans.push_back(make_pair(trans_station[p1] , end_station[p2]));
                        }
                        p1++ ; p2++;
                    }
                    else if (trans_station[p1] < end_station[p2]) p1++;
                    else p2++;
                }

                if (type == 0){//0 for time
                    for (int k = 0 ; k < ans.size() ; ++k){
                        vector<Train> con = trainSystem.findTrain(ans[k].first.TrainID);

                        date Leavetrans = ans[k].first.FirstTrain_leaveDate;
                        date arriveEnd = con[0].SaleDate_begin + con[0].StartDayTime + date(0, 0, 0, con[0].TravelTimeSum[ans[k].second.StationNo] + con[0].StopoverTimeSum[ans[k].second.StationNo-1]);
                        int trainNo2;
                        if (arriveTrans <= Leavetrans) trainNo2 = 1;
                        else {
                            trainNo2 = arriveTrans - Leavetrans;
                            if (date(0, 0, Leavetrans.Hour , Leavetrans.Minute) < date (0, 0, arriveTrans.Hour , arriveTrans.Minute)) trainNo2++;
                        }
                        Leavetrans += date(0, trainNo2 - 1 , 0, 0);
                        arriveEnd += date(0, trainNo2 - 1 ,0,0);

                        int wait = calMinute(arriveEnd , leaveStart);
                        if (wait < lowest){
                            lowest = wait;
                            lowestFirstTrainTime = calMinute(arriveTrans , leaveStart);

                            tmp1.TrainID = firstTrain.TrainID;
                            tmp1.Value = calMinute(arriveTrans , leaveStart);
                            tmp1.Cost = firstTrain.PriceSum[j] - firstTrain.PriceSum[start];
                            tmp1.From = st;
                            tmp1.To = con[0].Stations[ans[k].first.StationNo];
                            tmp1.SeatNum = trainSystem.getSeatNum(firstTrain.TrainID , start , j , trainNo1);
                            tmp1.LeaveTime = leaveStart , tmp1.ArriveTime = arriveTrans;

                            tmp2.TrainID = con[0].TrainID;
                            tmp2.Value = calMinute(arriveEnd , Leavetrans);
                            tmp2.Cost = con[0].PriceSum[ans[k].second.StationNo] - con[0].PriceSum[ans[k].first.StationNo];
                            tmp2.From = con[0].Stations[ans[k].first.StationNo];
                            tmp2.To = ed;
                            tmp2.SeatNum = trainSystem.getSeatNum(con[0].TrainID , ans[k].first.StationNo , ans[k].second.StationNo , trainNo2);
                            tmp2.LeaveTime = Leavetrans , tmp2.ArriveTime = arriveEnd;
                        }
                        else if (wait == lowest){
                            int FirstTrainTime = calMinute(arriveTrans , leaveStart);
                            if (FirstTrainTime < lowestFirstTrainTime){
                                lowestFirstTrainTime = calMinute(arriveTrans , leaveStart);

                                tmp1.TrainID = firstTrain.TrainID;
                                tmp1.Value = calMinute(arriveTrans , leaveStart);
                                tmp1.Cost = firstTrain.PriceSum[j] - firstTrain.PriceSum[start];
                                tmp1.From = st;
                                tmp1.To = con[0].Stations[ans[k].first.StationNo];
                                tmp1.SeatNum = trainSystem.getSeatNum(firstTrain.TrainID , start , j , trainNo1);
                                tmp1.LeaveTime = leaveStart , tmp1.ArriveTime = arriveTrans;

                                tmp2.TrainID = con[0].TrainID;
                                tmp2.Value = calMinute(arriveEnd , Leavetrans);
                                tmp2.Cost = con[0].PriceSum[ans[k].second.StationNo] - con[0].PriceSum[ans[k].first.StationNo];
                                tmp2.From = con[0].Stations[ans[k].first.StationNo];
                                tmp2.To = ed;
                                tmp2.SeatNum = trainSystem.getSeatNum(con[0].TrainID , ans[k].first.StationNo , ans[k].second.StationNo , trainNo2);
                                tmp2.LeaveTime = Leavetrans , tmp2.ArriveTime = arriveEnd;
                            }
                        }
                    }
                }
                else {
                    for (int k = 0 ; k < ans.size() ; ++k){
                        int cost = abs(firstTrain.PriceSum[j] - firstTrain.PriceSum[start] + ans[k].second.Price - ans[k].first.Price);
                        if (cost < lowest) {
                            lowest = cost;
                            int FirstTrainTime = firstTrain.TravelTimeSum[j] + firstTrain.StopoverTimeSum[j-1] - firstTrain.TravelTimeSum[start] - firstTrain.StopoverTimeSum[start];
                            lowestFirstTrainTime = FirstTrainTime;
                            ans_FirstTrain = firstTrain;
                            secondTrainTicket = ans[k];
                            firstTrainStation = make_pair(start , j);
                        }
                        else if (cost == lowest){
                            int FirstTrainTime = firstTrain.TravelTimeSum[j] + firstTrain.StopoverTimeSum[j-1] - firstTrain.TravelTimeSum[start] - firstTrain.StopoverTimeSum[start];
                            if (FirstTrainTime < lowestFirstTrainTime){
                                lowestFirstTrainTime = FirstTrainTime;
                                ans_FirstTrain = firstTrain;
                                secondTrainTicket = ans[k];
                                firstTrainStation = make_pair(start , j);
                            }
                        }
                    }
                }
            }
        }
    }

    if (lowest == 1e9 + 7){
        cout << 0 << "\n";
        return;
    }

    if (type == 0){
        tmp1.show();
        tmp2.show();
    }
    else{
        date t1 = ans_FirstTrain.StartDayTime; t1 += ans_FirstTrain.SaleDate_begin;
        date t2 = t1;//tmp2为到trans时间
        t2 += date(0 , 0 , 0 , ans_FirstTrain.TravelTimeSum[firstTrainStation.second] + ans_FirstTrain.StopoverTimeSum[firstTrainStation.second - 1]);
        t1 += date (0 , 0 , 0 , ans_FirstTrain.TravelTimeSum[firstTrainStation.first] + ans_FirstTrain.StopoverTimeSum[firstTrainStation.first]);
        int no = d - t1;
        t1 += date(0 , no - 1 , 0 , 0);
        t2 += date(0 , no - 1 , 0 , 0);
        cout << ans_FirstTrain.TrainID << " " << st << " ";
        t1.show();
        cout << " -> " << ans_FirstTrain.Stations[firstTrainStation.second] << " ";
        t2.show();
        cout << " " <<ans_FirstTrain.PriceSum[firstTrainStation.second] - ans_FirstTrain.PriceSum[firstTrainStation.first] << " ";
        cout << trainSystem.getSeatNum(ans_FirstTrain.TrainID , firstTrainStation.first , firstTrainStation.second , no) << "\n";

        vector<Train> con = trainSystem.findTrain(secondTrainTicket.first.TrainID);
        cout << secondTrainTicket.first.TrainID << " " << ans_FirstTrain.Stations[firstTrainStation.second] << " ";
        date t3 = secondTrainTicket.first.FirstTrain_leaveDate;
        date t4 = con[0].SaleDate_begin + con[0].StartDayTime + date(0, 0, 0, con[0].TravelTimeSum[secondTrainTicket.second.StationNo] + con[0].StopoverTimeSum[secondTrainTicket.second.StationNo-1]);;
        int no2;
        if (t2 <= t3) no2 = 1;
        else {
            no2 = t2 - t3;//比较tmp2和tmp3的发车时间
            if (date(0, 0, t3.Hour , t3.Minute) < date (0, 0, t2.Hour , t2.Minute)) no2++;
        }
        t3 += date (0 , no2 - 1, 0 , 0);
        t4 += date (0 , no2 - 1, 0 , 0);
        t3.show();
        cout << " -> " << ed << " ";
        t4.show();
        cout << " " <<secondTrainTicket.second.Price - secondTrainTicket.first.Price << " ";
        cout << trainSystem.getSeatNum(secondTrainTicket.first.TrainID , secondTrainTicket.first.StationNo , secondTrainTicket.second.StationNo , no2) << "\n";

    }
}

void Ticket_Control::queryTicket(const String<40> &st , const String<40> &ed , const date &d , int type) {
    vector<Ticket> begin_station = ticket_BPT.find(st.hash_value);
    vector<Ticket> end_station = ticket_BPT.find(ed.hash_value);
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
            if (cmp(begin_station[p1].FirstTrain_leaveDate , d) && cmp(d , begin_station[p1].LastTrain_leaveDate)){
                begin_ans.push_back(begin_station[p1]);
                end_ans.push_back(end_station[p2]);
            }
            p1++ ; p2++;
        }
        else if (begin_station[p1] < end_station[p2]) p1++;
        else p2++;
    }

    vector<inf_ticket> ans_container;
    if (type == 0){//time
        for (int i = 0 ; i < begin_ans.size() ; ++i){
            vector<Train> con = trainSystem.findTrain(begin_ans[i].TrainID);
            Train train = con[0];
            int no = d - begin_ans[i].FirstTrain_leaveDate;
            int start = begin_ans[i].StationNo , end = end_ans[i].StationNo;
            int time = train.TravelTimeSum[end] + train.StopoverTimeSum[end-1] - train.TravelTimeSum[start] - train.StopoverTimeSum[start];
            int cost = train.PriceSum[end] - train.PriceSum[start];
            date leaveTime = begin_ans[i].FirstTrain_leaveDate + date(0,no-1,0,0);
            date arriveTime = leaveTime + date(0,0,0,time);
            inf_ticket tmp(time, cost, train.TrainID , st , ed , trainSystem.getSeatNum(train.TrainID , start , end , no), leaveTime , arriveTime);
            ans_container.push_back(tmp);
        }
        ans_container.sort();
        cout << ans_container.size() << "\n";
        for (int i = 0 ; i < ans_container.size() ; ++i){
            ans_container[i].show();
        }
    }
    else {//cost
        for (int i = 0 ; i < begin_ans.size() ; ++i){
            vector<Train> con = trainSystem.findTrain(begin_ans[i].TrainID);
            Train train = con[0];
            int no = d - begin_ans[i].FirstTrain_leaveDate;
            int start = begin_ans[i].StationNo , end = end_ans[i].StationNo;
            int time = train.TravelTimeSum[end] + train.StopoverTimeSum[end-1] - train.TravelTimeSum[start] - train.StopoverTimeSum[start];
            int cost = train.PriceSum[end] - train.PriceSum[start];
            date leaveTime = begin_ans[i].FirstTrain_leaveDate + date(0,no-1,0,0);
            date arriveTime = leaveTime + date(0,0,0,time);
            inf_ticket tmp(cost, cost , train.TrainID , st , ed , trainSystem.getSeatNum(train.TrainID , start , end , no), leaveTime , arriveTime);
            ans_container.push_back(tmp);
        }
        ans_container.sort();
        cout << ans_container.size() << "\n";
        for (int i = 0 ; i < ans_container.size() ; ++i){
            ans_container[i].show();
        }
    }
}


inf_ticket::inf_ticket(int value, int cost, const String<21> &trainId, const String<40> &from, const String<40> &to,
                       int seatNum , const date &leaveTime, const date &arriveTime) :
Value(value), Cost(cost), TrainID(trainId), From(from), SeatNum(seatNum) , To(to), LeaveTime(leaveTime), ArriveTime(arriveTime) {}

void inf_ticket::show() {
    cout << TrainID << " " << From << " ";
    LeaveTime.show();
    cout << " -> " << To << " ";
    ArriveTime.show();
    cout << " " << Cost;
    cout << " " << SeatNum << "\n";
}

bool inf_ticket::operator<(const inf_ticket &rhs) const {
    if (Value < rhs.Value)
        return true;
    if (rhs.Value < Value)
        return false;
    return TrainID < rhs.TrainID;
}

bool inf_ticket::operator>(const inf_ticket &rhs) const {
    return rhs < *this;
}

bool inf_ticket::operator<=(const inf_ticket &rhs) const {
    return !(rhs < *this);
}

bool inf_ticket::operator>=(const inf_ticket &rhs) const {
    return !(*this < rhs);
}

