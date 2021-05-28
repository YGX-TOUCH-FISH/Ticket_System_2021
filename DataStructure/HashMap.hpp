//
// Created by 18163 on 2021/5/9.
//

#ifndef BPT_HASHMAP_HPP
#define BPT_HASHMAP_HPP
/*
 * All explicit and partial specializations of hash provided by the standard library are :
 *
 * DefaultConstructible,
 * CopyAssignable,
 * Swappable :: Move_Assignable, Move_Constructible.
 * Destructible.
 *
 * User-provided specializations of hash also must meet those requirements.
 */
#include <iostream>
#include "exception.hpp"
using namespace std;
namespace RA {
    //预估效果： Key：Key, Data：BinaryNode *
    template<
            typename Key,
            typename Data,
            typename Hash = std::hash<Key>
            >
    class HashMap {
        static const int CAPACITY = 101;
    private:
        class Node {
            //防构造函数缺省
        public:
            Key key;
            Data data;
            Node *next;
        public:
            Node():key(Key()), data(Data()), next(nullptr) {}

            Node(Key _key, Data _data): key(_key), data(_data), next(nullptr) {}

            ~Node() = default;
        };
        class LinkList {
        public:
            Node *head = nullptr;
        public:
            LinkList() = default;

            ~LinkList() {
                Node *ptr = head;
                while (ptr) {
                    Node *tmp = ptr;
                    ptr = ptr->next;
                    delete tmp;
                }
                head = nullptr;
            }

            Node *find(const Key &_key) {
                Node *ptr = head;
                while (ptr) {
                    if (ptr->key == _key) return ptr;
                    else ptr = ptr->next;
                }
                return ptr;
                //找不到：返回nullptr
            }

            void insert (const Key &_key, const Data &_data) {
                Node *ptr = new Node;
                ptr->key = _key;
                ptr->data = _data;
                ptr->next = head;
                head = ptr;
            }

            bool erase (const Key &_key) {
                if (!head) return false;
                Node *ptr = head;
                if (head->key == _key) {
                    head = head->next;
                    delete ptr; ptr = nullptr;
                    return true;
                }
                else {
                    while (ptr->next) {
                        if (ptr->next->key == _key) {
                            Node *exr = ptr->next;
                            ptr->next = ptr->next->next;
                            delete exr; exr = nullptr;
                            return true;
                        }
                        else ptr = ptr->next;
                    }
                    return false;
                }
            }
        };

        LinkList array[CAPACITY+1];
        //相同哈希值的构建成一链表
        int size = 0;
        Hash hash;
        inline int hash_key(Key _key) {
            int rt = hash(_key) % CAPACITY;
            if (rt < 0) rt += CAPACITY;
            return rt;
        }
    public:
        HashMap() {
            for (auto & i : array) i.head = nullptr;
        }

        Data &operator[](Key _key) {
            //找不到就插入, data为默认构造
            int i = hash_key(_key);
            Node *rt = array[i].find(_key);
            //在同key值链表中找到该key值
            if (!rt) array[i].insert(_key, Data()), ++size;

            rt = array[i].find(_key);
            return rt->data;
        }

        bool find(Key _key) {
            int i = hash_key(_key);
            if (array[i].find(_key)) return true;
            else return false;
        }

        void erase(Key _key) {
            int i = hash_key(_key);
            if (array[i].erase(_key)) --size;
            else return;
        }
    };
}
#endif //BPT_HASHMAP_HPP
