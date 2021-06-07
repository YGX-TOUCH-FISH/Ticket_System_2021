//
// Created by lenovo on 2021/4/26.
//
#include "Users.h"
using namespace RA;

map<int , int> user_Online;

//TODO———————————————————————————————————————————class  user——————————————————————————————————————————————————————————//
User::User() = default;

User::User(const String<21> &u, const String<31> &p, const String<20> &n, const String<31> &m, int pri) {
    username = u;
    password = p;
    name = n;
    mailAddress = m;
    privilege = pri;
}

bool User::operator==(const User &rhs) const {
    return username == rhs.username &&
           password == rhs.password &&
           name == rhs.name &&
           mailAddress == rhs.mailAddress &&
           privilege == rhs.privilege;
}

bool User::operator!=(const User &rhs) const {
    return !(rhs == *this);
}

int User::getPrivilege() const {
    return privilege;
}

//TODO————————————————————————————————————————class  User_Control—————————————————————————————————————————————————————//
//TODO clean时调用restart函数可以清空BPT与缓存区并重新初始化
void User_Control::restart() {
    username_BPT.remake("users_BPT.dat" , "users.dat");
    userOrderNum.remake("userOrderNum_BPT.dat" , "userOrderNum.dat");
}

bool User_Control::empty() {
    return username_BPT.isEmpty();
}

vector<User> User_Control::find(const String<21> &username) {
    vector<User> tmp = username_BPT.find(username.hash_value);
    return tmp;
}

void User_Control::add_user(User &u) {
    vector<User> user_exist;
    user_exist = username_BPT.find(u.username.hash_value);
    if (!user_exist.empty()) {
        throw "error (existed)";
    }
    username_BPT.insert(u.username.hash_value , u);
    userOrderNum.insert(u.username.hash_value , 0);
}

void User_Control::login(const String<21> &u , const String<31> &p){
    auto it = user_Online.find(u.hash_value);
    if (it != user_Online.end()) throw "error (have login)";
    vector<User> user_exist;
    user_exist = username_BPT.find(u.hash_value);
    if (user_exist.empty()) {
        throw "no finding";
    }
    User U = user_exist[0];
    if (U.password != p) throw "wrong password";
    user_Online.insert(make_pair(u.hash_value , U.privilege));
}

void User_Control::logout(const String<21> &u) {
    auto it = user_Online.find(u.hash_value);
    if (it == user_Online.end()) throw "error (no login)";
    user_Online.erase(it);
}

void User_Control::show_inf_user(const User &u) {
    cout << u.username << " " << u.name << " " << u.mailAddress << " " << u.privilege << "\n";
}

void User_Control::modify_user(const User &m_user , const String<31> &p, const String<20> &n, const String<31> &m, int pri) {
    User tmp(m_user.username , p , n , m , pri);
    if (p == "") tmp.password = m_user.password;
    if (n == "") tmp.name = m_user.name;
    if (m == "") tmp.mailAddress = m_user.mailAddress;
    if (pri == -1) tmp.privilege = m_user.privilege;
    else {
        if (user_Online.find(m_user.username.hash_value) != user_Online.end()){
            user_Online[m_user.username.hash_value] = pri;
        }
    }
    username_BPT.modify(m_user.username.hash_value , m_user , tmp);
    cout << tmp.username << " " << tmp.name << " " << tmp.mailAddress << " " << tmp.privilege << "\n";
}

//TODO 用户购买车票后调用addOrderNum函数,其记录的订单数加一
int User_Control::user_addOrderNum(const String<21> &username) {
    vector<int> con = userOrderNum.find(username.hash_value);
    int Num = con[0];
    Num++;
    userOrderNum.modify(username.hash_value , con[0] , Num);
    return Num;
}