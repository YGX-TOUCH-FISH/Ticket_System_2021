//
// Created by lenovo on 2021/4/26.
//

#ifndef TRAINSTATION_USERS_H
#define TRAINSTATION_USERS_H
#include "String.hpp"
#include "Map.hpp"
#include "BPT.hpp"
using namespace RA;

class User;

class User{
private:
    String<21> username;
    String<31> password;
    String<20> name;
    String<31> mailAddress;
    int privilege{};
    int TotalOrder = 0;

public:
    User();
    User(const String<21> &u , const String<31> &p , const String<20> &n, const String<31> &m , int pri , int totalOrder);

    int getPrivilege() const;
    bool operator==(const User &rhs) const;
    bool operator!=(const User &rhs) const;

    friend class user_System;
};


class user_System{
private:
    BPlusTree<String<21> , User, 400, 20000> username_BPT;

public:
    user_System(){
        username_BPT.initialize("users_BPT.dat" , "users.dat");
    }
    void restart();
    vector<User> find(const String<21> &username);
    bool empty();
    void add_user(User &u);
    int user_addOrder(const String<21> &username);
    void login(const String<21> &u , const String<31> &p);
    void logout(const String<21>& u);
    void query_user(const User &u);
    void modify_user(const User &m_user ,  const String<31>& p , const String<20>& n, const String<31>& m , int pri);
};

#endif //TRAINSTATION_USERS_H
