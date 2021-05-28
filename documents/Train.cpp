//
// Created by lenovo on 2021/4/26.
//

#include "Train.h"

//——————————————————————————————————————————————Train类函数————————————————————————————————————————————————————————————//
Train::Train(const String<21> &trainId, String<40> *stations, int stationNum, int seatNum, const int *priceSum, char type,
             const int *travelTimeSum, const int *stopoverTimeSum, const date &startDayTime, const date &saleDateBegin,
             const date &saleDateEnd , int isRelease = 0){
    TrainID = trainId;
    StationNum = stationNum;
    SeatNum = seatNum;
    StartDayTime = startDayTime;
    SaleDate_begin = saleDateBegin;
    SaleDate_end = saleDateEnd;
    for (int i = 1 ; i <= stationNum ; ++i) Stations[i] = stations[i];
    for (int i = 2 ; i <= stationNum ; ++i) PriceSum[i] = priceSum[i];
    for (int i = 2 ; i <= stationNum ; ++i) TravelTimeSum[i] = travelTimeSum[i];
    for (int i = 2 ; i < stationNum ; ++i) StopoverTimeSum[i] = stopoverTimeSum[i];
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
    for (int i = 1 ; i <= StationNum ; ++i) Stations[i] = t.Stations[i];
    for (int i = 2 ; i <= StationNum ; ++i) PriceSum[i] = t.PriceSum[i];
    for (int i = 2 ; i <= StationNum ; ++i) TravelTimeSum[i] = t.TravelTimeSum[i];
    for (int i = 2 ; i < StationNum ; ++i) StopoverTimeSum[i] = t.StopoverTimeSum[i];
    IsRelease = t.IsRelease;
    return *this;
}


bool Train::operator==(const Train &rhs) const {
    return TrainID == rhs.TrainID;
}

bool Train::operator!=(const Train &rhs) const {
    return !(rhs == *this);
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

//———————————————————————————————————————————————*****————————————————————————————————————————————————————————————————//

//——————————————————————————————————————————Train_Seat类函数———————————————————————————————————————————————————————————//
Train_Seat::Train_Seat(int seatNum){
    for (int i = 0 ; i < 100 ; ++i){
        for (int j = 0 ; j <= 100 ; ++j) seat[i][j] = seatNum;
    }
}

bool Train_Seat::operator<(const Train_Seat &t){
    return seat[1][1] < t.seat[1][1];
}



Train_Seat &Train_Seat::operator=(const Train_Seat &t) {
    if (this == &t) return *this;
    for (int i = 0 ; i < 100 ; ++i){
        for (int j = 0 ; j <= 100 ; ++j) seat[i][j] = t.seat[i][j];
    }
    return *this;
}

bool Train_Seat::operator==(const Train_Seat &rhs) const {
    return seat == rhs.seat;
}

bool Train_Seat::operator!=(const Train_Seat &rhs) const {
    return !(rhs == *this);
}
//———————————————————————————————————————————————*****————————————————————————————————————————————————————————————————//

//————————————————————————————————————————Train_System类接口———————————————————————————————————————————————————————————//
void Train_System::addTrain(const Train &t) {
    vector<Train> exist_train;
    exist_train = trainID_BPT.find(t.TrainID);
    if (!exist_train.empty()) throw "have existed";
    trainID_BPT.insert(t.TrainID , t);
}

void Train_System::deleteTrain(const String<21> &trainID) {
    vector<Train> exist_train;
    exist_train = trainID_BPT.find(trainID);
    if (exist_train.empty()) throw "no find";
    Train t = exist_train[0];
    if (t.IsRelease == 1) throw "cannot delete";
    trainID_BPT.erase(trainID , t);
    //to do
}

void Train_System::releaseTrain(const Train &t) {
    if (t.IsRelease == 1) throw "have released";
    Train re_t = t;
    re_t.IsRelease = 1;
    trainID_BPT.modify(t.TrainID , t , re_t);
    std::cout << 0 << "\n";
}

void Train_System::queryTrain(const Train &t, date &d) {
    vector<Train_Seat> tmp = trainSeat_BPT.find(t.TrainID);
    Train_Seat t_seat = tmp[0];

    int No = d - t.SaleDate_begin;
    std::cout << t.TrainID << " " << t.Type << "\n";
    std::cout << t.Stations[1] << " " << "xx-xx xx:xx" << " -> ";
    (d + date(0 , 0 , t.TravelTimeSum[1] , 0)).show();
    std::cout << " " << 0 << " " << t_seat.seat[No][1] << "\n"; //始发站
    for (int i = 2 ; i < t.StationNum ; ++i){
        std::cout << t.Stations[i] << " ";
        (d + date(0 , 0 , t.TravelTimeSum[i] + t.StopoverTimeSum[i - 1], 0)).show();
        std::cout << " -> ";
        (d + date(0 , 0 , t.TravelTimeSum[i] + t.StopoverTimeSum[i] , 0)).show();
        std::cout << t.PriceSum[i] << " " << t_seat.seat[No][i] << "\n";
    }
    std::cout << t.Stations[t.StationNum] << " ";
    (d + date(0 , 0 , t.TravelTimeSum[t.StationNum] + t.StopoverTimeSum[t.StationNum - 1], 0)).show();
    std::cout << " -> " << "xx-xx xx:xx" << t.PriceSum[t.StationNum] << " " << "x" << "\n";
}


void Train_System::addTrainSeat(const String<21> &trainID , int num) {
    vector<Train_Seat> exist_train;
    exist_train = trainSeat_BPT.find(trainID);
    if (!exist_train.empty()) throw "have existed";
    Train_Seat a(num);
    trainSeat_BPT.insert(trainID , a);
}

int Train_System::getSeatNum(const String<21> &trainID , int st , int ed , int no) {
    vector<Train_Seat> tmp = trainSeat_BPT.find(trainID);
    Train_Seat t_train = tmp[0];
    int ans = 1e9 + 7;
    for (int i = st ; i < ed ; ++i){
        ans = min(t_train.seat[no][i] , ans);
    }
    return ans;
}

int Train_System::getSeatNum(const String<21> &trainID, const String<40> &st, const String<40> &ed, int no) {
    vector<Train> tmp = trainID_BPT.find(trainID);
    Train t = tmp[0];
    int s = 0 , e = 0;
    for (int i = 1 ; i <= t.StationNum ; ++i){
        if (t.Stations[i] == st) s = i;
        else if (t.Stations[i] == ed) e = i;
    }
    vector<Train_Seat> tmp2 = trainSeat_BPT.find(trainID);
    Train_Seat t_train = tmp2[0];
    int ans = 1e9 + 7;
    for (int i = s ; i < e ; ++i){
        ans = min(t_train.seat[no][i] , ans);
    }
    return ans;
}


void Train_System::modifySeat(const String<21> &trainID, int st, int ed, int no, int changeNum) {
    vector<Train_Seat> tmp = trainSeat_BPT.find(trainID);
    Train_Seat t_train = tmp[0];
    for (int i = st ; i < ed ; ++i){
        t_train.seat[no][i] -= changeNum;
    }
}

void Train_System::modifySeat(const String<21> &trainID ,const String<40> &st , const String<40> &ed , int no , int changeNum) {
    vector<Train> tmp = trainID_BPT.find(trainID);
    Train t = tmp[0];
    int s = 0 , e = 0;
    for (int i = 1 ; i <= t.StationNum ; ++i){
        if (t.Stations[i] == st) s = i;
        else if (t.Stations[i] == ed) e = i;
    }
    vector<Train_Seat> tmp2 = trainSeat_BPT.find(trainID);
    Train_Seat t_train = tmp2[0];
    for (int i = s ; i < e ; ++i){
        t_train.seat[no][i] -= changeNum;
    }
}

vector<Train> Train_System::find(const String<21> &trainID) {
    vector<Train> tmp = trainID_BPT.find(trainID);
    return tmp;
}

vector<Train_Seat> Train_System::findSeat(const String<21> &trainID) {
    vector<Train_Seat> tmp = trainSeat_BPT.find(trainID);
    return tmp;
}

//———————————————————————————————————————————————*****————————————————————————————————————————————————————————————————//