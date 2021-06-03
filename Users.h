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
    friend class User_Control;
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

    bool operator==(const User &rhs) const;
    bool operator!=(const User &rhs) const;

    int getPrivilege() const;
};


class User_Control{
private:
    BPlusTree<String<21> , User, 200, 1000, 10000> username_BPT;//username——User BPT:储存用户信息

public:
    User_Control(){
        username_BPT.initialize("users_BPT.dat" , "users.dat");
    }
    void restart();
    bool empty();
    vector<User> find(const String<21> &username);

    void add_user(User &u);

    void login(const String<21> &u , const String<31> &p);
    void logout(const String<21>& u);
    void show_inf_user(const User &u);
    void modify_user(const User &m_user ,  const String<31>& p , const String<20>& n, const String<31>& m , int pri);
    int user_addOrderNum(const String<21> &username);
};

#endif //TRAINSTATION_USERS_H
