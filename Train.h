//
// Created by lenovo on 2021/4/26.
//

#ifndef TRAINSTATION_TRAIN_H
#define TRAINSTATION_TRAIN_H
#include "String.hpp"
#include "BPT.hpp"
#include "date.hpp"
using namespace RA;

class Train;
class Train_Seat;

class Train{
private:
    String<21> TrainID;
    String<40> Stations[101]; //stationNum项
    int StationNum = 0;
    int SeatNum = 0;
    int PriceSum[101]{}; //stationNum-1项
    char Type{};
    int TravelTimeSum[101]{}; // stationNum-1项 *** 从2开始到stationnum
    int StopoverTimeSum[101]{}; // stationNum-2项 *** 从2开始到stationnum-1
    date StartDayTime{};
    date SaleDate_begin{};
    date SaleDate_end{};
    int IsRelease = 0;
    int PendingNum[101] = {0};

public:
    Train() = default;

    Train(const String<21> &trainId, String<40> *stations, int stationNum, int seatNum, const int *priceSum, char type,
          const int *travelTimeSum, const int *stopoverTimeSum, const date &startDayTime, const date &saleDateBegin,
          const date &saleDateEnd , int isRelease);

    Train &operator=(const Train &t);

    bool operator<(const Train &rhs) const;
    bool operator>(const Train &rhs) const;
    bool operator<=(const Train &rhs) const;
    bool operator>=(const Train &rhs) const;

    bool operator==(const Train &rhs) const;

    bool operator!=(const Train &rhs) const;


    friend class Train_System;
    friend class ticket_System;
};

class Train_Seat{
private:
    int seat[100][101]{};
public:
    Train_Seat() = default;
    Train_Seat &operator=(const Train_Seat &t);
    explicit Train_Seat(int seatNum);

    bool operator<(const Train_Seat &t);
    bool operator==(const Train_Seat &rhs) const;
    bool operator!=(const Train_Seat &rhs) const;

    friend class Train_System;
};

class Train_System{
private:
    BPlusTree<String<21> , Train, 100, 200> trainID_BPT;
    BPlusTree<String<21> , Train_Seat, 100, 200>  trainSeat_BPT;

public:
    Train_System(){
        trainID_BPT.initialize("train_BPT.dat" , "Train.dat");
        trainSeat_BPT.initialize("trainSeat_BPT.dat" , "trainSeat.dat");
    }

    void restart();
    vector<Train> find(const String<21> &trainID);
    vector<Train_Seat> findSeat(const String<21> &trainID);
    void addTrain(const Train &t);
    void deleteTrain(const String<21> &trainID);
    void releaseTrain(const Train &t);
    void queryTrain(const Train &t , date &d);
    int addPendingOrderNum(const Train &t , int no);
    int getPendingOrderNum(const String<21> &trainID , int no);

    void addTrainSeat(const String<21> &trainID , int num);
    int getSeatNum(const String<21> &trainID , int st , int ed , int no);
    int getSeatNum(const String<21> &trainID ,const String<40> &st , const String<40> &ed , int no);
    void modifySeat(const String<21> &trainID , int st , int ed , int no , int changeNum);
    void modifySeat(const String<21> &trainID ,const String<40> &st , const String<40> &ed , int no , int changeNum);
};


#endif //TRAINSTATION_TRAIN_H