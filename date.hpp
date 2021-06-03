//
// Created by lenovo on 2021/5/12.
//
#ifndef TRAINSTATION_DATE_HPP
#define TRAINSTATION_DATE_HPP

#include <iostream>

class date{
public:
    int Month;
    int Day;
    int Hour;
    int Minute;

    date() = default;

    date(int month , int day , int hour , int minute) : Month(month) , Day(day) , Hour(hour) , Minute(minute) {}

    date(const date &d){
        Month = d.Month;
        Day = d.Day;
        Hour = d.Hour;
        Minute = d.Minute;
    }

    void reset(int month , int day , int hour , int minute) {
        Month = month;
        Day = day;
        Hour = hour;
        Minute = minute;
    }

    date &operator=(const date &d){
        if (this == &d) return *this;
        Month = d.Month;
        Day = d.Day;
        Hour = d.Hour;
        Minute = d.Minute;
        return *this;
    }

    bool operator==(const date &rhs) const {
        return Month == rhs.Month &&
               Day == rhs.Day &&
               Hour == rhs.Hour &&
               Minute == rhs.Minute;
    }

    bool operator!=(const date &rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const date &rhs) const {
        if (Month < rhs.Month)
            return true;
        if (rhs.Month < Month)
            return false;
        if (Day < rhs.Day)
            return true;
        if (rhs.Day < Day)
            return false;
        if (Hour < rhs.Hour)
            return true;
        if (rhs.Hour < Hour)
            return false;
        return Minute < rhs.Minute;
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
        int month = Month , day = Day , hour = Hour , minute = Minute;
        minute += d.Minute;
        hour += minute / 60 + d.Hour;
        minute %= 60;
        day += hour / 24 + d.Day;
        hour %= 24;
        while (true){//处理月份
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
        month += d.Month;
        return date(month , day , hour , minute);
    }

    date &operator+=(const date &d){
        Minute += d.Minute;
        Hour += Minute / 60 + d.Hour;
        Minute %= 60;
        Day += Hour / 24 + d.Day;
        Hour %= 24;
        while (true){//处理月份
            if (Month == 2){
                if (Day <= 28) break;
                else {
                    Day -= 28;
                    Month++;
                }
            }
            else if (Month == 4 || Month == 6 || Month == 9 || Month == 11){
                if (Day <= 30) break;
                else {
                    Day -= 30;
                    Month++;
                }
            }
            else {
                if (Day <= 31) break;
                else {
                    Day -= 31;
                    Month++;
                }
            }
        }
        Month += d.Month;
        return *this;
    }

    //TODO 计算两日期之间的日期差:用于计算班数
    int operator-(const date &d) const{
        date p(Month, Day, 0, 0), q(d.Month, d.Day, 0, 0);
        int cnt = (calMinute(p, q))/1440 + 1;
        return cnt;
    }
    //TODO 计算两日期之间的minutes差
    friend int calMinute(const date &x, const date &y) {
        int x_minute = 0, y_minute = 0;
        int a[12] = {0, 31, 59, 90, 120, 151,181 , 212, 243, 273, 304, 334};//月份天数前缀和

        x_minute += 1440 * a[x.Month - 1];
        y_minute += 1440 * a[y.Month - 1];

        x_minute += 1440 * (x.Day - 1);
        y_minute += 1440 * (y.Day - 1);

        x_minute += 60 * x.Hour + x.Minute;
        y_minute += 60 * y.Hour + y.Minute;

        return x_minute - y_minute;
    }
    //TODO 用于判断日期是否在范围内
    friend bool cmp(const date &x, const date &y){
        if (x.Month == y.Month) return x.Day <= y.Day;
        return x.Month < y.Month;
    }

    void show() const{
        if (Month <= 9) std::cout << 0 << Month;
        else std::cout << Month;
        std::cout << "-";
        if (Day <= 9) std::cout << 0 << Day;
        else std::cout << Day;
        std::cout << " ";
        if (Hour <= 9) std::cout << 0 << Hour;
        else std::cout << Hour;
        std::cout << ":";
        if (Minute <= 9) std::cout << 0 << Minute;
        else std::cout << Minute;
    }
};

#endif //TRAINSTATION_DATE_HPP
