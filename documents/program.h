//
// Created by lenovo on 2021/4/26.
//

#ifndef TRAINSTATION_PROGRAM_H
#define TRAINSTATION_PROGRAM_H

#include "String.hpp"
using namespace RA;

void Init();

void Run(std::string &command);

void add_user(std::string &cmd);

void login(std::string &cmd);

void logout(std::string &cmd);

void query_profile(std::string &cmd);

void modify_profile(std::string &cmd);

void add_train(std::string &cmd);

void release_train(std::string &cmd);

void query_train(std::string &cmd);

void delete_train(std::string &cmd);

void query_ticket(const std::string &cmd);

void query_transfer(const std::string &cmd);

void buy_ticket(const std::string &cmd);

void query_order(const std::string &cmd);

void refund_ticket(const std::string &cmd);

void Clean();

void Exit();

#endif //TRAINSTATION_PROGRAM_H
