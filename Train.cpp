//
// Created by lenovo on 2021/4/26.
//

#include "Train.h"

//TODO——————————————————————————————————————————Train类函数————————————————————————————————————————————————————————————//
Train::Train(const String<21> &trainId, String<40> *stations, int stationNum, int seatNum, const int *priceSum, char type,
             const int *travelTimeSum, const int *stopoverTimeSum, const date &startDayTime, const date &saleDateBegin,
             const date &saleDateEnd , int isRelease = 0){
    TrainID = trainId;
    StationNum = stationNum;
    SeatNum = seatNum;
    StartDayTime = startDayTime;
    SaleDate_begin = saleDateBegin;
    SaleDate_end = saleDateEnd;
    Type = type;
    for (int i = 1 ; i <= stationNum ; ++i) Stations[i] = stations[i];
    for (int i = 2 ; i <= stationNum ; ++i) PriceSum[i] = priceSum[i];
    for (int i = 2 ; i <= stationNum ; ++i) TravelTimeSum[i] = travelTimeSum[i];
    for (int i = 2 ; i < stationNum ; ++i) StopoverTimeSum[i] = stopoverTimeSum[i];
    PendingNum = 0;
    IsRelease = isRelease;
}

Train &Train::operator=(const Train &t) {
    if (this == &t) return *this;
    TrainID = t.TrainID;
    StationNum = t.StationNum;
    SeatNum = t.SeatNum;
    StartDayTime = t.StartDayTime;
    SaleDate_begin = t.SaleDate_begin;
    SaleDate_end = t.SaleDate_end;
    Type = t.Type;
    for (int i = 1 ; i <= StationNum ; ++i) Stations[i] = t.Stations[i];
    for (int i = 2 ; i <= StationNum ; ++i) PriceSum[i] = t.PriceSum[i];
    for (int i = 2 ; i <= StationNum ; ++i) TravelTimeSum[i] = t.TravelTimeSum[i];
    for (int i = 2 ; i < StationNum ; ++i) StopoverTimeSum[i] = t.StopoverTimeSum[i];
    PendingNum = t.PendingNum;
    IsRelease = t.IsRelease;
    return *this;
}

bool Train::operator<(const Train &rhs) const {
    return TrainID < rhs.TrainID;
}

bool Train::operator>(const Train &rhs) const {
    return rhs < *this;
}

bool Train::operator<=(const Train &rhs) const {
    return !(rhs < *this);
}

bool Train::operator>=(const Train &rhs) const {
    return !(*this < rhs);
}

bool Train::operator==(const Train &rhs) const {
    return TrainID == rhs.TrainID &&
           IsRelease == rhs.IsRelease &&
           PendingNum == rhs.PendingNum;
}

bool Train::operator!=(const Train &rhs) const {
    return !(rhs == *this);
}

//TODO——————————————————————————————————————Train_Seat类函数———————————————————————————————————————————————————————————//
Train_Seat::Train_Seat(int seatNum){
    for (int i = 0 ; i <= 100 ; ++i) seat[i] = seatNum;
}

bool Train_Seat::operator<(const Train_Seat &t){
    return seat[1] < t.seat[1];
}

Train_Seat &Train_Seat::operator=(const Train_Seat &t) {
    if (this == &t) return *this;
    for (int i = 0 ; i <= 100 ; ++i) seat[i] = t.seat[i];
    return *this;
}

bool Train_Seat::operator==(const Train_Seat &rhs) const {
    for (int i = 0 ; i <= 100 ; ++i){
        if (seat[i] != rhs.seat[i]) return false;
    }
    return true;
}

bool Train_Seat::operator!=(const Train_Seat &rhs) const {
    return !(rhs == *this);
}

//TODO————————————————————————————————————Train_System类接口———————————————————————————————————————————————————————————//
void Train_Control::restart() {
    trainID_BPT.remake("train_BPT.dat" , "Train.dat");
    trainSeat_BPT.remake("trainSeat_BPT.dat" , "trainSeat.dat");
}

//TODO—————————————————about Train———————————————————————//
vector<Train> Train_Control::findTrain(const String<21> &trainID) {
    vector<Train> tmp = trainID_BPT.find(trainID);
    return tmp;
}

void Train_Control::addTrain(const Train &t) {
    vector<Train> exist_train;
    exist_train = trainID_BPT.find(t.TrainID);
    if (!exist_train.empty()) throw "have existed";
    trainID_BPT.insert(t.TrainID , t);
}

void Train_Control::deleteTrain(const String<21> &trainID) {
    vector<Train> exist_train;
    exist_train = trainID_BPT.find(trainID);
    if (exist_train.empty()) throw "no findTrain";
    Train t = exist_train[0];
    if (t.IsRelease == 1) throw "cannot delete";
    trainID_BPT.erase(trainID , t);
    int dayCount = t.SaleDate_end - t.SaleDate_begin;
    for (int i = 1 ; i <= dayCount ; ++i){
        trainSeat_BPT.erase(make_pair(trainID , i) , Train_Seat (t.SeatNum));
    }
    //to do
}

void Train_Control::releaseTrain(const Train &t) {
    if (t.IsRelease == 1) throw "have released";
    Train re_t = t;
    re_t.IsRelease = 1;
    trainID_BPT.modify(t.TrainID , t , re_t);
}

void Train_Control::queryTrain(const Train &t, date &d) {
    if (cmp(t.SaleDate_begin , d) && cmp(d , t.SaleDate_end)){
        int No = d - t.SaleDate_begin;

        vector<Train_Seat> tmp = trainSeat_BPT.find(make_pair(t.TrainID , No));
        Train_Seat t_seat = tmp[0];
        std::cout << t.TrainID << " " << t.Type << "\n";
        std::cout << t.Stations[1] << " " << "xx-xx xx:xx" << " -> ";
        (d += t.StartDayTime).show();
        std::cout << " " << 0 << " " << t_seat.seat[1] << "\n"; //始发站
        for (int i = 2 ; i < t.StationNum ; ++i){
            std::cout << t.Stations[i] << " ";
            (d + date(0 , 0 , 0 ,t.TravelTimeSum[i] + t.StopoverTimeSum[i - 1])).show();
            std::cout << " -> ";
            (d + date(0 , 0 , 0 , t.TravelTimeSum[i] + t.StopoverTimeSum[i])).show();
            std::cout << " " << t.PriceSum[i] << " " << t_seat.seat[i] << "\n";
        }
        std::cout << t.Stations[t.StationNum] << " ";
        (d + date(0 , 0 , 0 , t.TravelTimeSum[t.StationNum] + t.StopoverTimeSum[t.StationNum - 1])).show();
        std::cout << " -> " << "xx-xx xx:xx " << t.PriceSum[t.StationNum] << " " << "x" << "\n";
    }
    else throw "error";
}

int Train_Control::addPendingOrderNum(const Train &t , int no) {
    Train tmp = t;
    tmp.PendingNum++;
    trainID_BPT.modify(t.TrainID , t , tmp);
    return tmp.PendingNum;
}

//TODO—————————————————about TrainSeat———————————————————//
vector<Train_Seat> Train_Control::findSeat(const String<21> &trainID , int no) {
    vector<Train_Seat> tmp = trainSeat_BPT.find(make_pair(trainID , no));
    return tmp;
}

void Train_Control::addTrainSeat(const String<21> &trainID , int num , int dayCount) {
    Train_Seat a(num);
    for (int i = 1 ; i <= dayCount ; ++i) trainSeat_BPT.insert(make_pair(trainID , i) , a);
}

int Train_Control::getSeatNum(const String<21> &trainID , int st , int ed , int no) {
    vector<Train_Seat> tmp = trainSeat_BPT.find(make_pair(trainID , no));
    Train_Seat t_train = tmp[0];
    int ans = 1e9 + 7;
    for (int i = st ; i < ed ; ++i){
        ans = min(t_train.seat[i] , ans);
    }
    return ans;
}

int Train_Control::getSeatNum(const String<21> &trainID, const String<40> &st, const String<40> &ed, int no) {
    vector<Train> tmp = trainID_BPT.find(trainID);
    Train t = tmp[0];
    int s = 0 , e = 0;
    for (int i = 1 ; i <= t.StationNum ; ++i){
        if (t.Stations[i] == st) s = i;
        else if (t.Stations[i] == ed) e = i;
    }
    vector<Train_Seat> tmp2 = trainSeat_BPT.find(make_pair(trainID , no));
    Train_Seat t_train = tmp2[0];
    int ans = 1e9 + 7;
    for (int i = s ; i < e ; ++i){
        ans = min(t_train.seat[i] , ans);
    }
    return ans;
}

void Train_Control::modifySeat(const String<21> &trainID, int st, int ed, int no, int changeNum) {
    vector<Train_Seat> tmp = trainSeat_BPT.find(make_pair(trainID , no));
    Train_Seat t_train = tmp[0];
    for (int i = st ; i < ed ; ++i){
        t_train.seat[i] -= changeNum;
    }
    trainSeat_BPT.modify(make_pair(trainID , no) , tmp[0] , t_train);
}

void Train_Control::modifySeat(const String<21> &trainID , const String<40> &st , const String<40> &ed , int no , int changeNum) {
    vector<Train> tmp = trainID_BPT.find(trainID);
    Train t = tmp[0];
    int s = 0 , e = 0;
    for (int i = 1 ; i <= t.StationNum ; ++i){
        if (t.Stations[i] == st) s = i;
        else if (t.Stations[i] == ed) e = i;
    }
    vector<Train_Seat> tmp2 = trainSeat_BPT.find(make_pair(trainID , no));
    Train_Seat t_train = tmp2[0];
    for (int i = s ; i < e ; ++i){
        t_train.seat[i] -= changeNum;
    }
    trainSeat_BPT.modify(make_pair(trainID , no) , tmp2[0] , t_train);
}
//———————————————————————————————————————————————*****————————————————————————————————————————————————————————————————//