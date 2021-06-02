//
// Created by 18163 on 2021/5/17.
//


#ifndef TICKET_SYSTEM_2021_MASTER_ALLOC_HPP
#define TICKET_SYSTEM_2021_MASTER_ALLOC_HPP
#include <iostream>
#include <fstream>
#include "exception.hpp"
#include "Map.hpp"
using namespace std;
namespace RA {
    template<typename InternInfo, typename HeadInfo, int CACHESIZE = 500>
    class DiskMonitor{
    private:
        class Binary_Node {
        public:
            Binary_Node *prev = nullptr;
            Binary_Node *next = nullptr;
            int key = -1;
            InternInfo *data = nullptr;
        public:
            bool isModified = false;
            Binary_Node() = delete;
            explicit Binary_Node(const InternInfo &info, Binary_Node *pre = nullptr, Binary_Node *nxt = nullptr) {
                data = new InternInfo(info);
                prev = pre;
                next = nxt;
            }
            ~Binary_Node() {
                delete data;
            }
        };
    private:
        fstream file;
        Binary_Node *head = nullptr;
        Binary_Node *tail = nullptr;
        map<int, Binary_Node*> cache_map;
        int cache_size = 0;
//        static const int cache_CAPACITY = 500;

        bool cache_find(int infoAddress) {
            if (cache_map.find(infoAddress) != cache_map.end()) {
                Binary_Node *ptr = cache_map[infoAddress];
                if (ptr->prev) {
                    Binary_Node *pre = ptr->prev;
                    Binary_Node *nxt = ptr->next;
                    pre->next = nxt;
                    if (nxt) nxt->prev = pre;
                    else tail = pre;//尾部节点被移到头部

                    head->prev = ptr;
                    ptr->next = head;
                    ptr->prev = nullptr;
                    head = ptr;
                }
                return true;
            }
            else return false;
        }
        InternInfo &cache_info(int infoAddress) {
            return cache_map[infoAddress]->data[0];
        }
        std::pair<int, InternInfo> cache_pop() {
            if (!tail) throw container_is_empty();
            std::pair<int ,InternInfo> out_msg;
            out_msg.first = tail->key;
            out_msg.second = tail->data[0];
            if (!tail->isModified) out_msg.first = -1;//未被修改过，返回-1
            //准备返回值
            auto it = cache_map.find(tail->key);
            cache_map.erase(it);
            //map中删除
            Binary_Node *ptr = tail;
            tail = tail->prev;
            if (tail) tail->next = nullptr;
            else head = nullptr;
            delete ptr;
            ptr = nullptr;
            --cache_size;
            //链表中删除
            return out_msg;
        }
        void cache_push(int infoAddress, const InternInfo &internInfo) {
            if (cache_size == 0) {
                head = new Binary_Node(internInfo);
                tail = head;
                head->key = infoAddress;
                ++cache_size;
            }
            else {
                Binary_Node *ptr;
                ptr = new Binary_Node(internInfo, nullptr, head);
                ptr->key = infoAddress;
                head->prev = ptr;
                head = ptr;
                ++cache_size;
            }
            cache_map[infoAddress] = head;
        }
    public:
        DiskMonitor() = default;
        ~DiskMonitor() {
            Binary_Node *ptr = head;
            while (ptr) {
                if (ptr->isModified) {
                    file.seekp(ptr->key);
                    file.write(reinterpret_cast<const char *>(ptr->data), sizeof(InternInfo));
                }
                Binary_Node *tmp = ptr;
                ptr = ptr->next;
                delete tmp;
            }
            head = tail = nullptr;
            file.close();
        }

        void clear() {
            cache_map.clear();
            cache_size = 0;
            Binary_Node *ptr = head;
            while (ptr) {
                //清空缓存，不需要读进去
                Binary_Node *tmp = ptr;
                ptr = ptr->next;
                delete tmp;
            }
            head = tail = nullptr;
            file.close();
        }

        void initialize(const string &filename) {
            fstream test;
            test.open(filename);
            if (test.fail()) {
                ofstream openfile(filename);
                openfile.close();
            }
            test.close();
            file.open(filename);
        }

        bool empty() {
            file.seekg(0, ios::end);
            return (file.tellg() == 0);
        }

        void read_HeadInfo(HeadInfo &headInfo) {
            file.seekg(0);
            file.read(reinterpret_cast<char *>(&headInfo), sizeof(HeadInfo));
        }

        void write_HeadInfo(const HeadInfo &headInfo) {
            file.seekp(0);
            file.write(reinterpret_cast<const char *>(&headInfo), sizeof(HeadInfo));
        }

        void read(int info_address, InternInfo &internInfo) {
            if (info_address < 0) throw bad_read_flow();

//            if (cache_find(info_address)) internInfo = cache_info(info_address);
//            else {
                file.seekg(info_address);
                file.read(reinterpret_cast<char *>(&internInfo), sizeof(internInfo));
//                if (cache_size == CACHESIZE) {
//                    std::pair<int, InternInfo> out = cache_pop();
//                    if (out.first != -1) {
//                        file.seekp(out.first);
//                        file.write(reinterpret_cast<const char *>(&out.second), sizeof(InternInfo));
//                    }
//                }
//                cache_push(info_address, internInfo);
//            }
        }

        void write(int info_address, const InternInfo &internInfo) {
            if (info_address < 0) throw bad_write_flow();
//            if (cache_map.find(info_address) != cache_map.end()) {
//                if (cache_info(info_address) != internInfo) {
//                    cache_info(info_address) = internInfo;
//                    cache_map[info_address]->isModified = true;
//                }
////            }
//            else {
            file.seekp(info_address);
            file.write(reinterpret_cast<const char *>(&internInfo), sizeof(internInfo));
//            }
        }
    };
}

#endif //TICKET_SYSTEM_2021_MASTER_ALLOC_HPP
