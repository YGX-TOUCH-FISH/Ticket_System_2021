//
// Created by lenovo on 2021/5/12.
//

#ifndef TRAINSTATION_DATE_HPP
#define TRAINSTATION_DATE_HPP
#include <iostream>
class date{
public:
    int month;
    int day;
    int hour;
    int minute;

    date() = default;

    date(int M , int d , int h , int m) : month(M) , day(d) , hour(h) , minute(m) {}

    date(const date &d){
        month = d.month;
        day = d.day;
        hour = d.hour;
        minute = d.minute;
    }

    void reset(int m , int d , int h , int min) {
        month = m;
        day = d;
        hour = h;
        minute = min;
    }


    date &operator=(const date &d){
        if (this == &d) return *this;
        month = d.month;
        day = d.day;
        hour = d.hour;
        minute = d.minute;
        return *this;
    }

    bool operator==(const date &rhs) const {
        return month == rhs.month &&
               day == rhs.day &&
               hour == rhs.hour &&
               minute == rhs.minute;
    }

    bool operator!=(const date &rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const date &rhs) const {
        if (month < rhs.month)
            return true;
        if (rhs.month < month)
            return false;
        if (day < rhs.day)
            return true;
        if (rhs.day < day)
            return false;
        if (hour < rhs.hour)
            return true;
        if (rhs.hour < hour)
            return false;
        return minute < rhs.minute;
    }

    bool operator>(const date &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const date &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const date &rhs) const {
        return !(*this < rhs);
    }

    date operator+(const date &d) const{
        int M = month , D = day , h = hour , min = minute;
        min += d.minute;
        while (min >= 60){
            h++;
            min -= 60;
        }
        h += d.hour;
        while (h >= 24){
            D++;
            h -= 24;
        }
        D += d.day;
        while (true){
            if (M == 2){
                if (D <= 28) break;
                else {
                    D -= 28;
                    M++;
                }
            }
            else if (M == 4 || M == 6 || M == 9 || M == 11){
                if (D <= 30) break;
                else {
                    D -= 30;
                    M++;
                }
            }
            else {
                if (D <= 31) break;
                else {
                    D -= 31;
                    M++;
                }
            }
        }
        M += d.month;
        return date(M , D , h , min);
    }

    date &operator+=(const date &d){
        minute += d.minute;
        while (minute >= 60){
            hour++;
            minute -= 60;
        }
        hour += d.hour;
        while (hour >= 24){
            day++;
            hour -= 24;
        }
        day += d.day;
        while (true){
            if (month == 2){
                if (day <= 28) break;
                else {
                    day -= 28;
                    month++;
                }
            }
            else if (month == 4 || month == 6 || month == 9 || month == 11){
                if (day <= 30) break;
                else {
                    day -= 30;
                    month++;
                }
            }
            else {
                if (day <= 31) break;
                else {
                    day -= 31;
                    month++;
                }
            }
        }
        month += d.month;
        return *this;
    }



    friend int calMinute(const date &x, const date &y) {
        int x_minute = 0, y_minute = 0;
        int a[12] = {0, 31, 59, 90, 120, 151,181 , 212, 243, 273, 304, 334};
        x_minute += 1440*a[x.month-1];
        y_minute += 1440*a[y.month-1];

        x_minute += 1440*(x.day-1);
        y_minute += 1440*(y.day-1);

        x_minute += 60*x.hour + x.minute;
        y_minute += 60*y.hour + y.minute;

        return x_minute-y_minute;
    }


    int operator-(const date &d) const{ // 只有6 7 8 (9)月
//        if (month == d.month) return day - d.day + 1;
//        int cnt = 0;
//        if (d.month == 4 || d.month == 6 || d.month == 9 || d.month == 11) cnt += 30 - d.day + 1;
//        else if (d.month == 2) cnt += 28 - d.day + 1;
//        else cnt += 31 - d.day + 1;
//        cnt += day;
//        int m = d.month + 1;
//        while (m != month){
//            if (m == 4 || m == 6 || m == 9 || m == 11) cnt += 30;
//            else if (m == 2) cnt += 28;
//            else cnt += 31;
//            m++;
//        }
        date p(month, day, 0, 0), q(d.month, d.day, 0, 0);
        int cnt = calMinute(p, q)/1440 + 1;
        return cnt;
    }

    friend bool cmp(const date &x, const date &y){//用于判断日期是否在范围内
        if (x.month == y.month) return x.day <= y.day;
        return x.month < y.month;
    }

    void show() const{
        if (month <= 9) std::cout << 0 << month;
        else std::cout << month;
        std::cout << "-";
        if (day <= 9) std::cout << 0 << day;
        else std::cout << day;
        std::cout << " ";
        if (hour <= 9) std::cout << 0 << hour;
        else std::cout << hour;
        std::cout << ":";
        if (minute <= 9) std::cout << 0 << minute;
        else std::cout << minute;
    }
};

#endif //TRAINSTATION_DATE_HPP
