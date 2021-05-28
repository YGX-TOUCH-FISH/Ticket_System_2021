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
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
    template<typename T>
    class vector {
//private:
    private:
        T **storage;
        int form_size = 128;
        int current_size = 0;

    public:
        /**
         * TODO
         * a type for actions of the elements of a vector, and you should write
         *   a class named const_iterator with same interfaces.
         */
        /**
         * you can see RandomAccessIterator at CppReference for help.
         */
        class const_iterator;
        class iterator {
            friend vector<T>;
//	private:
        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            T **pointer = nullptr;
            vector<T> *start;
        public:
            /**
             * return a new iterator which pointer n-next elements
             * as well as operator-
             */
            iterator() = default;

            iterator(const iterator &ec){
                pointer = ec.pointer;
                start = ec.start;
            }

            iterator(T **p, vector<T> *s):pointer(p),start(s){ }

            iterator operator+(const int &n) const {
                //TODO
                iterator answer(pointer+n, start);
                return answer;
            }
            iterator operator-(const int &n) const {
                //TODO
                iterator answer(pointer-n, start);
                return answer;
            }
            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invaild_iterator.
            int operator-(const iterator &rhs) const {
                //TODO
                if (start != rhs.start)throw invalid_iterator();
                return pointer - rhs.pointer;
            }
            iterator& operator+=(const int &n) {
                //TODO
                pointer += n;
                return *this;
            }
            iterator& operator-=(const int &n) {
                //TODO
                pointer -= n;
                return *this;
            }
            /**
             * TODO iter++
             */
            iterator operator++(int) {
                iterator answer = *this;
                ++pointer;
                return answer;
            }
            /**
             * TODO ++iter
             */
            iterator& operator++() {
                ++pointer;
                return *this;
            }
            /**
             * TODO iter--
             */
            iterator operator--(int) {
                iterator answer = *this;
                --pointer;
                return answer;
            }
            /**
             * TODO --iter
             */
            iterator& operator--() {
                --pointer;
                return *this;
            }
            /**
             * TODO *it
             */
            T& operator*() const{
                return **pointer;
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory address).
             */
            bool operator==(const iterator &rhs) const {
                if (pointer == rhs.pointer && start == rhs.start)return true;
                return false;
            }
            bool operator==(const const_iterator &rhs) const {
                if (pointer == rhs.pointer && start == rhs.start)return true;
                return false;
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                if (pointer == rhs.pointer && start == rhs.start)return false;
                return true;
            }
            bool operator!=(const const_iterator &rhs) const {
                if (pointer == rhs.pointer && start == rhs.start)return false;
                return true;
            }
        };
        /**
         * TODO
         * has same function as iterator, just for a const object.
         */
        class const_iterator {
            friend vector<T>;
        public:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            T **pointer;
            const vector<T> *start;
        public:
            /**
             * return a new iterator which pointer n-next elements
             * as well as operator-
             */
            const_iterator() = default;

            const_iterator(const const_iterator &ec){
                pointer = ec.pointer;
                start = ec.start;
            }

            const_iterator(T **p, const vector<T> *s):pointer(p),start(s){ }

            const_iterator operator+(const int &n) const {
                //TODO
                const_iterator answer(pointer+n, start);
                return answer;
            }
            const_iterator operator-(const int &n) const {
                //TODO
                const_iterator answer(pointer-n, start);
                return answer;
            }
            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invaild_iterator.
            int operator-(const iterator &rhs) const {
                //TODO
                if (start != rhs.start)throw invalid_iterator();
                return pointer - rhs.pointer;
            }
            const_iterator& operator+=(const int &n) {
                //TODO
                pointer += n;
                return *this;
            }
            const_iterator& operator-=(const int &n) {
                //TODO
                pointer -= n;
                return *this;
            }
            /**
             * TODO iter++
             */
            const_iterator operator++(int) {
                const_iterator answer = *this;
                ++pointer;
                return answer;
            }
            /**
             * TODO ++iter
             */
            const_iterator& operator++() {
                ++pointer;
                return *this;
            }
            /**
             * TODO iter--
             */
            const_iterator operator--(int) {
                const_iterator answer = *this;
                --pointer;
                return answer;
            }
            /**
             * TODO --iter
             */
            const_iterator& operator--() {
                --pointer;
                return *this;
            }
            /**
             * TODO *it
             */
            const T& operator*() const{
                return **pointer;
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory address).
             */
            bool operator==(const iterator &rhs) const {
                if (pointer == rhs.pointer && start == rhs.start)return true;
                return false;
            }
            bool operator==(const const_iterator &rhs) const {
                if (pointer == rhs.pointer && start == rhs.start)return true;
                return false;
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                if (pointer == rhs.pointer && start == rhs.start)return false;
                return true;
            }
            bool operator!=(const const_iterator &rhs) const {
                if (pointer == rhs.pointer && start == rhs.start)return false;
                return true;
            }
        };
        /**
         * TODO Constructs
         * Atleast two: default constructor, copy constructor
         */
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
        /**
         * TODO Destructor
         */
        ~vector() {
            for(int i = 0 ; i < current_size; ++i)
                delete storage[i];
            delete []storage;
            storage = nullptr;
        }
        /**
         * TODO Assignment operator
         */
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
        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, cache_size)
         */
        T & at(const size_t &pos) {
            if (pos < 0 || pos >= current_size) throw index_out_of_bound();
            return storage[pos][0];
        }
        const T & at(const size_t &pos) const {
            if (pos < 0 || pos >= current_size) throw index_out_of_bound();
            return storage[pos][0];
        }
        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, cache_size)
         * !!! Pay attentions
         *   In STL this operator does not check the boundary but I want you to do.
         */
        T & operator[](const size_t &pos) {
            if (pos < 0 || pos >= current_size) throw index_out_of_bound();
            return storage[pos][0];
        }
        const T & operator[](const size_t &pos) const {
            if (pos < 0 || pos >= current_size) throw index_out_of_bound();
            return storage[pos][0];
        }
        /**
         * access the first element.
         * throw container_is_empty if cache_size == 0
         */
        const T & front() const {
            if (current_size == 0)throw container_is_empty();
            return storage[0][0];
        }
        /**
         * access the last element.
         * throw container_is_empty if cache_size == 0
         */
        const T & back() const {
            if (current_size == 0)throw container_is_empty();
            return storage[current_size-1][0];
        }
        /**
         * returns an iterator to the beginning.
         */
        iterator begin() {
            return iterator(storage, this);
        }
        const_iterator cbegin() const {
            return const_iterator(storage, this);
        }
        /**
         * returns an iterator to the end.
         */
        iterator end() {
            //std::cout << "current::" << current_size<<'\n';
            return iterator(storage+current_size, this);
        }
        const_iterator cend() const {
            return const_iterator(storage+current_size, this);
        }
        /**
         * checks whether the container is empty
         */
        bool empty() const {
            if (current_size == 0)return true;
            return false;
        }
        /**
         * returns the number of elements
         */
        size_t size() const {
            return current_size;
        }
        /**
         * clears the contents
         */
        void clear() {
            current_size = 0;
        }
        /**
         * inserts value before pos
         * returns an iterator pointing to the inserted value.
         */

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
        /**
         * inserts value at index ind.
         * after inserting, this->at(ind) == value
         * returns an iterator pointing to the inserted value.
         * throw index_out_of_bound if ind > cache_size (in this situation ind can be cache_size because after inserting the cache_size will increase 1.)
         */
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
        /**
         * removes the element at pos.
         * return an iterator pointing to the following element.
         * If the iterator pos refers the last element, the end() iterator is returned.
         */
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
        /**
         * removes the element with index ind.
         * return an iterator pointing to the following element.
         * throw index_out_of_bound if ind >= cache_size
         */
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
        /**
         * adds an element to the end.
         */
        void push_back(const T &value) {
            if (current_size == form_size) double_space();
            storage[current_size] = new T(value);
            ++current_size;
        }
        /**
         * remove the last element from the end.
         * throw container_is_empty if cache_size() == 0
         */
        void pop_back() {
            if (current_size == 0)throw container_is_empty();
            delete storage[current_size-1];
            --current_size;
        }

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
