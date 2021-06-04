//
// Created by lenovo on 2021/5/22.
//
#include "Ticket.h"

//TODO———————————————————————————————————————————class Ticket—————————————————————————————————————————————————————————//
Ticket::Ticket(const String<21> &trainID , int stationNo, const date &startDayTime , const date &startDay, const date &endDay , int arrivalTime , int leaveTime , int price) {
    TrainID = trainID;
    StationNo = stationNo;
    StartDayTime = startDayTime;
    SaleDate_begin = startDay;
    SaleDate_end = endDay;
    Price = price;
    ArrivalTime = arrivalTime;
    LeaveTime = leaveTime;
}

Ticket &Ticket::operator=(const Ticket &t){
    if (this == &t) return *this;
    TrainID = t.TrainID;
    StationNo = t.StationNo;
    StartDayTime = t.StartDayTime;
    SaleDate_begin = t.SaleDate_begin;
    SaleDate_end = t.SaleDate_end;
    Price = t.Price;
    ArrivalTime = t.ArrivalTime;
    LeaveTime = t.LeaveTime;
    return *this;
}

Ticket::Ticket(const Ticket &t) {
    TrainID = t.TrainID;
    StationNo = t.StationNo;
    StartDayTime = t.StartDayTime;
    SaleDate_begin = t.SaleDate_begin;
    SaleDate_end = t.SaleDate_end;
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
    addTicket(t.Stations[1] ,Ticket (t.TrainID , 1 , t.StartDayTime ,  t.SaleDate_begin , t.SaleDate_end , 0 , 0 , 0));
    for (int i = 2 ; i <= t.StationNum ; ++i){
        Ticket tmp(t.TrainID , i ,  t.StartDayTime , t.SaleDate_begin , t.SaleDate_end , t.TravelTimeSum[i] + t.StopoverTimeSum[i - 1]
                , t.TravelTimeSum[i] + t.StopoverTimeSum[i] , t.PriceSum[i]);
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
            int pendingNum = trainSystem.addPendingOrderNum(t , no);
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
        int left = trainSystem.getSeatNum(o.TrainID , o.From , o.To , o.StationNo);
        if (o.TicketNum <= left) {
            trainSystem.modifySeat(o.TrainID , o.From , o.To , o.StationNo , o.TicketNum);
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

    for (int i = 0 ; i < start_station.size() ; ++i){
        date salebegin = start_station[i].SaleDate_begin + start_station[i].StartDayTime + date(0, 0, 0, start_station[i].LeaveTime);
        date saleend = start_station[i].SaleDate_end + start_station[i].StartDayTime + date(0, 0, 0, start_station[i].LeaveTime);
        if (cmp(salebegin , d) && cmp(d , saleend)){
            vector<Train> train_container = trainSystem.findTrain(start_station[i].TrainID);
            Train firstTrain = train_container[0];
            int start = start_station[i].StationNo;
            for (int j = start + 1 ; j <= firstTrain.StationNum ; ++j){
                date leaveStart = firstTrain.StartDayTime + firstTrain.SaleDate_begin + date (0,0,0,firstTrain.TravelTimeSum[start] + firstTrain.StopoverTimeSum[start]);
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
                        date saleend = trans_station[p1].SaleDate_end + trans_station[p1].StartDayTime + date(0, 0, 0, trans_station[p1].LeaveTime);
                        if (arriveTrans <= saleend){
                            ans.push_back(make_pair(trans_station[p1] , end_station[p2]));
                        }
                        p1++ ; p2++;
                    }
                    else if (trans_station[p1] < end_station[p2]) p1++;
                    else p2++;
                }

                if (type == 0){//0 for time
                    for (int k = 0 ; k < ans.size() ; ++k){
                        date Leavetrans = ans[k].first.SaleDate_begin + ans[k].first.StartDayTime + date(0, 0, 0, ans[k].first.LeaveTime);
                        date arriveEnd = ans[k].first.SaleDate_begin + ans[k].first.StartDayTime + date(0, 0, 0, ans[k].second.ArrivalTime);
                        int trainNo2;
                        if (arriveTrans <= Leavetrans) trainNo2 = 1;
                        else {
                            trainNo2 = arriveTrans - Leavetrans;//比较tmp2和tmp3的发车时间
                            if (date(0, 0, Leavetrans.Hour , Leavetrans.Minute) < date (0, 0, arriveTrans.Hour , arriveTrans.Minute)) trainNo2++;
                        }
                        Leavetrans += date(0, trainNo2 - 1 , 0, 0);
                        arriveEnd += date(0, trainNo2 - 1 ,0,0);

                        int wait = calMinute(arriveEnd , leaveStart);
                        if (wait < lowest){
                            lowest = wait;
                            lowestFirstTrainTime = calMinute(arriveTrans , leaveStart);
                            ans_FirstTrain = firstTrain;
                            secondTrainTicket = ans[k];
                            firstTrainStation = make_pair(start , j);
                        }
                        else if (wait == lowest){
                            int FirstTrainTime = calMinute(arriveTrans , leaveStart);
                            if (FirstTrainTime < lowestFirstTrainTime){
                                lowestFirstTrainTime = calMinute(arriveTrans , leaveStart);
                                ans_FirstTrain = firstTrain;
                                secondTrainTicket = ans[k];
                                firstTrainStation = make_pair(start , j);
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

    date tmp1 = ans_FirstTrain.StartDayTime; tmp1 += ans_FirstTrain.SaleDate_begin;
    date tmp2 = tmp1;//tmp2为到trans时间
    tmp2 += date(0 , 0 , 0 , ans_FirstTrain.TravelTimeSum[firstTrainStation.second] + ans_FirstTrain.StopoverTimeSum[firstTrainStation.second - 1]);
    tmp1 += date (0 , 0 , 0 , ans_FirstTrain.TravelTimeSum[firstTrainStation.first] + ans_FirstTrain.StopoverTimeSum[firstTrainStation.first]);
    int no = d - tmp1;
    tmp1 += date(0 , no - 1 , 0 , 0);
    tmp2 += date(0 , no - 1 , 0 , 0);

    cout << ans_FirstTrain.TrainID << " " << st << " ";
    tmp1.show();
    cout << " -> " << ans_FirstTrain.Stations[firstTrainStation.second] << " ";
    tmp2.show();
    cout << " " <<ans_FirstTrain.PriceSum[firstTrainStation.second] - ans_FirstTrain.PriceSum[firstTrainStation.first] << " ";
    cout << trainSystem.getSeatNum(ans_FirstTrain.TrainID , firstTrainStation.first , firstTrainStation.second , no) << "\n";

    cout << secondTrainTicket.first.TrainID << " " << ans_FirstTrain.Stations[firstTrainStation.second] << " ";
    date tmp3 = secondTrainTicket.first.StartDayTime; tmp3 += secondTrainTicket.first.SaleDate_begin;
    date tmp4 = tmp3;
    tmp3 += date (0 , 0 , 0 ,secondTrainTicket.first.LeaveTime);
    tmp4 += date (0 , 0 , 0 ,secondTrainTicket.second.ArrivalTime);
    int no2;
    if (tmp2 <= tmp3) no2 = 1;
    else {
        no2 = tmp2 - tmp3;//比较tmp2和tmp3的发车时间
        if (date(0, 0, tmp3.Hour , tmp3.Minute) < date (0, 0, tmp2.Hour , tmp2.Minute)) no2++;
    }
    tmp3 += date (0 , no2 - 1, 0 , 0);
    tmp4 += date (0 , no2 - 1, 0 , 0);
    tmp3.show();
    cout << " -> " << ed << " ";
    tmp4.show();
    cout << " " <<secondTrainTicket.second.Price - secondTrainTicket.first.Price << " ";
    cout << trainSystem.getSeatNum(secondTrainTicket.first.TrainID , secondTrainTicket.first.StationNo , secondTrainTicket.second.StationNo , no2) << "\n";

}