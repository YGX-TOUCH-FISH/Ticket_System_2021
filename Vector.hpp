//
// Created by 18163 on 2021/4/26.
//

#ifndef BPT_VECTOR_HPP
#define BPT_VECTOR_HPP

#include "exception.hpp"
#include <climits>
#include <cstddef>
#include <iostream>
#include <cstring>

namespace RA {
    template<typename T>
    class vector {
    private:
        T **storage;
        int form_size = 128;
        int current_size = 0;

    public:
        class const_iterator;
        class iterator {
            friend vector<T>;
        private:
            T **pointer = nullptr;
            vector<T> *start;
        public:
            iterator() = default;

            iterator(const iterator &ec){
                pointer = ec.pointer;
                start = ec.start;
            }

            iterator(T **p, vector<T> *s):pointer(p),start(s){ }

            iterator operator+(const int &n) const {
                iterator answer(pointer+n, start);
                return answer;
            }

            iterator operator-(const int &n) const {
                iterator answer(pointer-n, start);
                return answer;
            }

            int operator-(const iterator &rhs) const {// return the distance between two iterators
                if (start != rhs.start)throw invalid_iterator();
                return pointer - rhs.pointer;
            }

            iterator& operator+=(const int &n) {
                pointer += n;
                return *this;
            }

            iterator& operator-=(const int &n) {
                pointer -= n;
                return *this;
            }

            iterator operator++(int) {
                iterator answer = *this;
                ++pointer;
                return answer;
            }

            iterator& operator++() {
                ++pointer;
                return *this;
            }

            iterator operator--(int) {
                iterator answer = *this;
                --pointer;
                return answer;
            }

            iterator& operator--() {
                --pointer;
                return *this;
            }

            T& operator*() const{
                return **pointer;
            }

            bool operator==(const iterator &rhs) const {
                if (pointer == rhs.pointer && start == rhs.start)return true;
                return false;
            }
            bool operator==(const const_iterator &rhs) const {
                if (pointer == rhs.pointer && start == rhs.start)return true;
                return false;
            }
            bool operator!=(const iterator &rhs) const {
                if (pointer == rhs.pointer && start == rhs.start)return false;
                return true;
            }
            bool operator!=(const const_iterator &rhs) const {
                if (pointer == rhs.pointer && start == rhs.start)return false;
                return true;
            }
        };

        class const_iterator {
            friend vector<T>;
        public:
            T **pointer;
            const vector<T> *start;
        public:
            const_iterator() = default;

            const_iterator(const const_iterator &ec){
                pointer = ec.pointer;
                start = ec.start;
            }

            const_iterator(T **p, const vector<T> *s):pointer(p),start(s){ }

            const_iterator operator+(const int &n) const {
                const_iterator answer(pointer+n, start);
                return answer;
            }

            const_iterator operator-(const int &n) const {
                const_iterator answer(pointer-n, start);
                return answer;
            }

            int operator-(const iterator &rhs) const {
                if (start != rhs.start)throw invalid_iterator();
                return pointer - rhs.pointer;
            }
            const_iterator& operator+=(const int &n) {
                pointer += n;
                return *this;
            }
            const_iterator& operator-=(const int &n) {
                pointer -= n;
                return *this;
            }

            const_iterator operator++(int) {
                const_iterator answer = *this;
                ++pointer;
                return answer;
            }

            const_iterator& operator++() {
                ++pointer;
                return *this;
            }

            const_iterator operator--(int) {
                const_iterator answer = *this;
                --pointer;
                return answer;
            }

            const_iterator& operator--() {
                --pointer;
                return *this;
            }

            const T& operator*() const{
                return **pointer;
            }

            bool operator==(const iterator &rhs) const {
                if (pointer == rhs.pointer && start == rhs.start)return true;
                return false;
            }
            bool operator==(const const_iterator &rhs) const {
                if (pointer == rhs.pointer && start == rhs.start)return true;
                return false;
            }
            bool operator!=(const iterator &rhs) const {
                if (pointer == rhs.pointer && start == rhs.start)return false;
                return true;
            }
            bool operator!=(const const_iterator &rhs) const {
                if (pointer == rhs.pointer && start == rhs.start)return false;
                return true;
            }
        };

        vector() {
            storage = new T* [form_size];
        }

        vector(const vector &other) {
            form_size = other.form_size;
            current_size = other.current_size;
            storage = new T*[other.form_size];
            for(int i = 0 ; i < current_size ; ++i)
                storage[i] = new T (other.storage[i][0]);
        }

        ~vector() {
            for(int i = 0 ; i < current_size; ++i)
                delete storage[i];
            delete []storage;
            storage = nullptr;
        }

        vector &operator=(const vector &other) {
            if (this == &other)return *this;
            for(int i = 0 ; i < current_size ; ++i)
                delete storage[i];
            delete []storage;
            storage = new T*[other.form_size];
            form_size = other.form_size;
            current_size = other.current_size;
            for(int i = 0 ; i < current_size ; ++i)
                storage[i] = new T(other.storage[i][0]);
            return *this;
        }

        void double_space(){
            form_size = current_size*2;
            T **new_storage = new T*[form_size];
            for(int i = 0 ; i <current_size ; ++i){
                new_storage[i] = new T(storage[i][0]);
                delete storage[i];
            }
            delete []storage;
            storage = new_storage;
        }

//———————————————————————————————————————————————*****————————————————————————————————————————————————————————————————//
        T & at(const size_t &pos) {
            if (pos < 0 || pos >= current_size) throw index_out_of_bound();
            return storage[pos][0];
        }
        const T & at(const size_t &pos) const {
            if (pos < 0 || pos >= current_size) throw index_out_of_bound();
            return storage[pos][0];
        }

        T & operator[](const size_t &pos) {
            if (pos < 0 || pos >= current_size) throw index_out_of_bound();
            return storage[pos][0];
        }
        const T & operator[](const size_t &pos) const {
            if (pos < 0 || pos >= current_size) throw index_out_of_bound();
            return storage[pos][0];
        }

        const T & front() const {
            if (current_size == 0)throw container_is_empty();
            return storage[0][0];
        }

        const T & back() const {
            if (current_size == 0)throw container_is_empty();
            return storage[current_size-1][0];
        }

        iterator begin() {
            return iterator(storage, this);
        }
        const_iterator cbegin() const {
            return const_iterator(storage, this);
        }
        iterator end() {
            //std::cout << "current::" << current_size<<'\n';
            return iterator(storage+current_size, this);
        }
        const_iterator cend() const {
            return const_iterator(storage+current_size, this);
        }

        bool empty() const {
            if (current_size == 0)return true;
            return false;
        }

        size_t size() const {
            return current_size;
        }

        void clear() {
            current_size = 0;
        }

        iterator insert(iterator pos, const T &value) {
            if (pos.start != this) throw runtime_error();
            if (pos.pointer - storage > current_size || pos.pointer - storage < 0) throw runtime_error();
            if (current_size == form_size) double_space();
            if (current_size == 0){
                push_back(value);
                return iterator(storage, this);
            }
            int count;
            for( count = 0 ; count < current_size ; ++count)
                if (storage + count == pos.pointer) break;

            storage[current_size] = new T(storage[current_size-1][0]);
            for ( int i = current_size-1 ; i >= count ; --i)
                storage[i+1][0] = storage[i][0];
            storage[count][0] = value;
            ++current_size;
            return iterator(pos.pointer, this);
        }

        iterator insert(const size_t &ind, const T &value) {
            if (ind >= current_size)throw index_out_of_bound();
            if (current_size == form_size) double_space();
            if (current_size == 0){
                push_back(value);
                return iterator(storage, this);
            }
            storage[current_size] = new T(storage[current_size-1][0]);
            for(int i = current_size-1 ; i >= ind ; --i)
                storage[i+1][0] = storage[i][0];
            storage[ind][0] = value;
            ++current_size;
            iterator answer(storage+ind, this);
            return answer;
        }

        iterator erase(iterator pos) {
            if (pos.start != this) throw runtime_error();
            if (pos.pointer - storage < 0 || pos.pointer - storage >= current_size) throw runtime_error();
            int count = 0;
            while (storage + count != pos.pointer)++count;
            delete storage[count];
            for(int i = count ; i < current_size-1 ;++i)
                storage[i] = storage[i+1];
            storage[current_size-1] = nullptr;
            --current_size;
            return iterator(storage+count, this);
        }

        iterator erase(const size_t &ind) {
            if (ind >= current_size)throw index_out_of_bound();
            delete storage[ind];
            storage[ind] = nullptr;
            for(int i = ind ; i < current_size-1 ; ++i)
                storage[i] = storage[i+1];
            storage[current_size-1] = nullptr;
            --current_size;
            return iterator(storage+ind, this);
        }

        void push_back(const T &value) {
            if (current_size == form_size) double_space();
            storage[current_size] = new T(value);
            ++current_size;
        }

        void pop_back() {
            if (current_size == 0)throw container_is_empty();
            delete storage[current_size-1];
            --current_size;
        }

        //TODO 内置sort
        void sort() {
            quick_sort(0, current_size-1);
        }

        void quick_sort(int l, int r) {
            int mid;
            if (l >= r) return;
            mid = divide(l, r);
            quick_sort(l, mid-1);
            quick_sort(mid+1, r);
        }
        int divide(int l, int r) {
            T k = storage[l][0];
            do {
                while (l < r && storage[r][0] >= k) --r;
                if (l < r) storage[l][0] = storage[r][0], ++l;
                while (l < r && storage[l][0] <= k) ++l;
                if (l < r) storage[r][0] = storage[l][0], --r;
            } while (l != r);
            storage[l][0] = k;
            return l;
        }
    };


}

#endif //BPT_VECTOR_HPP
