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
    friend class Train_Control;
    friend class Ticket_Control;
private:
    struct data {
        int StationName;
        int Offset;
        bool operator <(const data &ec) const {
            return StationName < ec.StationName;
        }
    };
    struct StationArray{
    private:
        void sort() {
            quickSort(array, 1, StationNumber);
        }
        void quickSort(data a[], int low, int high) {
            int mid;
            if (low >= high) return;
            mid = divide(a, low, high);
            quickSort(a, low, mid-1);
            quickSort(a, mid+1, high);
        }
        int divide(data a[], int low, int high) {
            data k = a[low];
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
        int StationNumber = 0;
        data array[101] {};

        int operator[](const int &STATION_NAME) const{
            data find;
            find.StationName = STATION_NAME, find.Offset = 0;
            int index = lower_bound(array, StationNumber, find);
            return array[index].Offset;
        };

        StationArray(data a[], const int &size) {
            for (int i = 1 ;  i <= size ; ++i)
                array[i] = a[i];
            StationNumber = size;
            sort();
        }
    };

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
    int IsRelease = 0;

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
    BPlusTree<int , Train, 200, 10000, 0> trainID_BPT;
    BPlusTree<pair<String<21> , int> , Train_Seat, 200, 0, 0>  trainSeat_BPT;// pair<trainID , 车次> —— seat
    BPlusTree<int , int, 200, 10000, 0> trainPendingNum_BPT;

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

    //TODO delete
    vector<Train_Seat> findSeat(const String<21> &trainID , int no);
    int getSeatNum(const String<21> &trainID ,const String<40> &st , const String<40> &ed , int no);
};
#endif //TRAINSTATION_TRAIN_H