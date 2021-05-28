//
// Created by lenovo on 2021/4/26.
//
#include "Users.h"
using namespace RA;

map<String<21> , int> user_Online;
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

vector<User> user_System::find(const String<21> &username) {
    vector<User> tmp = username_BPT.find(username);
    return tmp;
}

void user_System::add_user(User &u) {
    vector<User> user_exist;
    user_exist = username_BPT.find(u.username);
    if (!user_exist.empty()) {
        throw "register fail";
    }
    username_BPT.insert(u.username , u);
}

void user_System::login(const String<21> &u , const String<31> &p){
    auto it = user_Online.find(u);
    if (it != user_Online.end()) throw "have login";
    vector<User> user_exist;
    user_exist = username_BPT.find(u);
    if (user_exist.empty()) {
        throw "no finding";
    }
    User U = user_exist[0];
    if (U.password != p) throw "wrong password";
    user_Online.insert(make_pair(u , 1));
}

void user_System::logout(const String<21> &u) {
    auto it = user_Online.find(u);
    if (it == user_Online.end()) throw "no finding";
    user_Online.erase(it);
}

void user_System::query_user(const User &u) {
    cout << u.username << " " << u.name << " " << u.mailAddress << " " << u.privilege << "\n";
}

void user_System::modify_user(const User &m_user , const String<31> &p, const String<20> &n, const String<31> &m, int pri) {
    User tmp(m_user.username , p , n , m , pri);
    if (p == "") tmp.password = m_user.password;
    if (n == "") tmp.name = m_user.name;
    if (m == "") tmp.mailAddress = m_user.mailAddress;
    if (pri == -1) tmp.privilege = m_user.privilege;
    username_BPT.modify(m_user.username , m_user , tmp);
    cout << tmp.username << " " << tmp.name << " " << tmp.mailAddress << " " << tmp.privilege << "\n";
}