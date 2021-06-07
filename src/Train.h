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

struct StationHashArray{
private:
    int StationNumber = 0;
    pair<int , int> array[101] {};

    void sort() {
        quickSort(array, 1, StationNumber);
    }
    void quickSort(pair<int , int> a[], int low, int high) {
        int mid;
        if (low >= high) return;
        mid = divide(a, low, high);
        quickSort(a, low, mid-1);
        quickSort(a, mid+1, high);
    }
    int divide(pair<int , int> a[], int low, int high) {
        pair<int , int> k = a[low];
        do {
            while (low < high && !(a[high] < k)) --high;
            if (low < high) { a[low] = a[high]; ++low; }
            while (low < high && !(k < a[low])) ++low;
            if (low < high) { a[high] = a[low]; --high; }
        } while (low != high);
        a[low] = k;
        return low;
    }

public:
    StationHashArray() = default;
    StationHashArray(pair<int , int> a[], const int &size) {
        for (int i = 1 ;  i <= size ; ++i)
            array[i] = a[i];
        StationNumber = size;
        sort();
    }
    StationHashArray &operator=(const StationHashArray &s){
        if (this == &s) return *this;
        for (int i = 1 ;  i <= s.StationNumber ; ++i)
            array[i] = s.array[i];
        StationNumber = s.StationNumber;
        return *this;
    };

    bool operator==(const StationHashArray &rhs) const {
        return StationNumber == rhs.StationNumber;
    }
    bool operator!=(const StationHashArray &rhs) const {
        return !(rhs == *this);
    }

    int operator[](const int &STATION_NAME) const{
        pair<int , int> find{};
        find.first = STATION_NAME, find.second = 0;
        int index = lower_bound(array, StationNumber, find);
        if (array[index].first != STATION_NAME) return -1;
        return array[index].second;
    };
};

class Train{
    friend class Train_Control;
    friend class Ticket_Control;
private:
    String<21> TrainID;
    String<40> Stations[101]; //stationNum项
    int StationNum = 0;
    int SeatNum = 0;
    int PriceSum[101]{}; //stationNum-1项
    char Type{};
    int TravelTimeSum[101]{}; // stationNum-1项 *** 从2开始到stationNum
    int StopoverTimeSum[101]{}; // stationNum-2项 *** 从2开始到stationNum-1
    date StartDayTime{};
    date SaleDate_begin{};
    date SaleDate_end{};
    StationHashArray StationHash;
    int IsRelease = 0;

public:
    Train() = default;

    Train(const String<21> &trainId, String<40> *stations, int stationNum, int seatNum, const int *priceSum, char type,
          const int *travelTimeSum, const int *stopoverTimeSum, const date &startDayTime, const date &saleDateBegin,
          const date &saleDateEnd , const StationHashArray &stationHash , int isRelease);

    Train &operator=(const Train &t);

    bool operator<(const Train &rhs) const;
    bool operator>(const Train &rhs) const;
    bool operator<=(const Train &rhs) const;
    bool operator>=(const Train &rhs) const;

    bool operator==(const Train &rhs) const;
    bool operator!=(const Train &rhs) const;
};

class Train_Seat{
    friend class Train_Control;
private:
    int seat[101]{};
public:
    Train_Seat() = default;
    Train_Seat &operator=(const Train_Seat &t);
    explicit Train_Seat(int seatNum);

    bool operator<(const Train_Seat &t);
    bool operator==(const Train_Seat &rhs) const;
    bool operator!=(const Train_Seat &rhs) const;
};

class Train_Control{
private:
    BPlusTree<int , Train> trainID_BPT;
    BPlusTree<pair<int , int> , Train_Seat>  trainSeat_BPT;// pair<trainID , 车次> —— seat
    BPlusTree<int , int> trainPendingNum_BPT;

public:
    Train_Control(){
        trainID_BPT.initialize("train_BPT.dat" , "Train.dat");
        trainSeat_BPT.initialize("trainSeat_BPT.dat" , "trainSeat.dat");
        trainPendingNum_BPT.initialize("trainPendingNum_BPT.dat" , "trainPendingNum.dat");
    }

    void restart();
    vector<Train> findTrain(const String<21> &trainID);
    void addTrain(const Train &t);
    void deleteTrain(const String<21> &trainID);
    void releaseTrain(const Train &t);
    void queryTrain(const Train &t , date &d);
    int addPendingOrderNum(const String<21> &trainID , int no);

    int getSeatNum(const String<21> &trainID , int st , int ed , int no);
    void modifySeat(const String<21> &trainID , int st , int ed , int no , int changeNum);
    void modifySeat(const String<21> &trainID ,const String<40> &st , const String<40> &ed , int no , int changeNum);
};
#endif //TRAINSTATION_TRAIN_H