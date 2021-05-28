//
// Created by lenovo on 2021/5/12.
//

#ifndef TRAINSTATION_DATE_HPP
#define TRAINSTATION_DATE_HPP
#include <iostream>
class date{
private:
    int month;
    int day;
    int hour;
    int minute;

public:
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

    bool operator<(const date &d) const{
        if (month != d.month) return month < d.month;
        else if (day != d.day) return day < d.day;
        else if (hour != d.hour) return hour < d.hour;
        else return minute < d.month;
    }

    date &operator+(const date &d){
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

    int operator-(const date &d) const{ // 只有6 7 8 (9)月
        if (month == d.month) return day - d.day + 1;
        int cnt = 0;
        if (d.month == 4 || d.month == 6 || d.month == 9 || d.month == 11) cnt += 30 - d.day + 1;
        else if (d.month == 2) cnt += 28 - d.day + 1;
        else cnt += 31 - d.day + 1;
        cnt += day;
        int m = d.month + 1;
        while (m != month){
            if (m == 4 || m == 6 || m == 9 || m == 11) cnt += 30;
            else if (m == 2) cnt += 28;
            else cnt += 31;
            m++;
        }
        return cnt;
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
