//
// Created by 18163 on 2021/4/29.
//

#ifndef BPT_MAP_HPP
#define BPT_MAP_HPP
// only for std::less<T>
#include <functional>
#include <cstddef>
#include "exception.hpp"

namespace RA {
    template<
            class Key,
            class T,
            class Compare = std::less<Key>
    > class map {
    public:
        typedef std::pair<const Key, T> value_type;
        class avl_node {
            friend class map<Key, T, Compare>;
            value_type *data = nullptr;
            avl_node *left = nullptr;
            avl_node *right = nullptr;
            avl_node *father = nullptr;
            int height = 1;
        public:
            avl_node(): data(nullptr), left(nullptr), right(nullptr), father(nullptr), height(1) {}

            avl_node(const value_type &ec, avl_node *l, avl_node *r, avl_node* f, const int &h)
                    : left(l), right(r), father(f), height(h) {data = new value_type (ec);}

            ~avl_node() {
                if (data) delete data;
                data = nullptr;
                left = nullptr;
                right = nullptr;
                father = nullptr;
                height = 1;
            }
        };
        class const_iterator;
        class iterator {
        private:
            friend class map<Key, T, Compare>;
            map<Key, T, Compare> *tree_ptr;
            avl_node *node_ptr;
        public:
            iterator():tree_ptr(nullptr), node_ptr(nullptr) {}

            iterator(const iterator &other)
                    :tree_ptr(other.tree_ptr), node_ptr(other.node_ptr) {}

            iterator(map<Key, T, Compare> *tp, avl_node *np)
                    :tree_ptr(tp), node_ptr(np) {}

            iterator &operator=(const iterator& ec) {
                if (this == &ec) return *this;
                tree_ptr = nullptr;
                node_ptr = nullptr;
                tree_ptr = ec.tree_ptr;
                node_ptr = ec.node_ptr;
                return *this;
            }

            iterator operator++(int) {
                if (node_ptr == tree_ptr->tail) throw invalid_iterator();
                iterator ans(tree_ptr, node_ptr);
                if (node_ptr == tree_ptr->root) {
                    node_ptr = node_ptr->right;
                    if (node_ptr) {
                        while (node_ptr->left) node_ptr = node_ptr->left;
                    }
                    else node_ptr = tree_ptr->tail;
                    return ans;
                }
                else {
                    if (node_ptr == node_ptr->father->left) {
                        if (node_ptr->right) {
                            node_ptr = node_ptr->right;
                            while (node_ptr->left) node_ptr = node_ptr->left;
                            return ans;
                        }
                        else {
                            node_ptr = node_ptr->father;
                            return ans;
                        }
                    }
                    else {
                        if (node_ptr->right) {
                            node_ptr = node_ptr->right;
                            while (node_ptr->left) node_ptr = node_ptr->left;
                            return ans;
                        }
                        else {
                            while (node_ptr->father) {
                                if (node_ptr == node_ptr->father->right) {
                                    node_ptr = node_ptr->father;
                                    continue;
                                }
                                node_ptr = node_ptr->father;
                                return ans;
                            }
                            node_ptr = tree_ptr->tail;
                            return ans;
                        }
                    }
                }
            }
            iterator & operator++() {
                if (node_ptr == tree_ptr->tail) throw invalid_iterator();
                if (node_ptr == tree_ptr->root) {
                    node_ptr = node_ptr->right;
                    if (node_ptr) {
                        while (node_ptr->left) node_ptr = node_ptr->left;
                    }
                    else node_ptr = tree_ptr->tail;
                    return *this;
                }
                else {
                    if (node_ptr == node_ptr->father->left) {
                        if (node_ptr->right) {
                            node_ptr = node_ptr->right;
                            while (node_ptr->left) node_ptr = node_ptr->left;
                            return *this;
                        }
                        else {
                            node_ptr = node_ptr->father;
                            return *this;
                        }
                    }
                    else {
                        if (node_ptr->right) {
                            node_ptr = node_ptr->right;
                            while (node_ptr->left) node_ptr = node_ptr->left;
                            return *this;
                        }
                        else {
                            while (node_ptr->father) {
                                if (node_ptr == node_ptr->father->right) {
                                    node_ptr = node_ptr->father;
                                    continue;
                                }
                                node_ptr = node_ptr->father;
                                return *this;
                            }
                            node_ptr = tree_ptr->tail;
                            return *this;
                        }
                    }
                }
            }
            iterator operator--(int) {
                iterator ans(tree_ptr, node_ptr);
                if (node_ptr == tree_ptr->tail) {
                    node_ptr = tree_ptr->root;
                    if (!node_ptr) throw invalid_iterator();
                    while (node_ptr->right) node_ptr = node_ptr->right;
                    return ans;
                }
                else if (node_ptr == tree_ptr->root) {
                    node_ptr = node_ptr->left;
                    if (node_ptr) {
                        while (node_ptr->right) node_ptr = node_ptr->right;
                        return ans;
                    }
                    else throw invalid_iterator();
                }
                else {
                    if (node_ptr == node_ptr->father->left) {
                        if (node_ptr->left) {
                            node_ptr = node_ptr->left;
                            while (node_ptr->right) node_ptr = node_ptr->right;
                            return ans;
                        }
                        else {
                            while (node_ptr->father) {
                                if (node_ptr == node_ptr->father->left) {
                                    node_ptr = node_ptr->father;
                                    continue;
                                }
                                node_ptr = node_ptr->father;
                                return ans;
                            }
                            throw invalid_iterator();
                        }
                    }
                    else {
                        if (node_ptr->left) {
                            node_ptr = node_ptr->left;
                            while (node_ptr->right) node_ptr = node_ptr->right;
                            return ans;
                        }
                        else {
                            node_ptr = node_ptr->father;
                            return ans;
                        }
                    }
                }
            }
            iterator & operator--() {
                if (node_ptr == tree_ptr->tail) {
                    node_ptr = tree_ptr->root;
                    if (!node_ptr) throw invalid_iterator();
                    while (node_ptr->right) node_ptr = node_ptr->right;
                    return *this;
                }
                else if (node_ptr == tree_ptr->root) {
                    node_ptr = node_ptr->left;
                    if (node_ptr) {
                        while (node_ptr->right) node_ptr = node_ptr->right;
                        return *this;
                    }
                    throw invalid_iterator();
                }
                else {
                    if (node_ptr == node_ptr->father->left) {
                        if (node_ptr->left) {
                            node_ptr = node_ptr->left;
                            while (node_ptr->right) node_ptr = node_ptr->right;
                            return *this;
                        }
                        else {
                            while (node_ptr->father) {
                                if (node_ptr == node_ptr->father->left) {
                                    node_ptr = node_ptr->father;
                                    continue;
                                }
                                node_ptr = node_ptr->father;
                                return *this;
                            }
                            throw invalid_iterator();
                        }
                    }
                    else {
                        if (node_ptr->left) {
                            node_ptr = node_ptr->left;
                            while (node_ptr->right) node_ptr = node_ptr->right;
                            return *this;
                        }
                        else {
                            node_ptr = node_ptr->father;
                            return *this;
                        }
                    }
                }
            }
            value_type & operator*() const {
                return node_ptr->data[0];
            }
            bool operator==(const iterator &rhs) const {
                return tree_ptr == rhs.tree_ptr && node_ptr == rhs.node_ptr;
            }
            bool operator==(const const_iterator &rhs) const {
                return tree_ptr == rhs.tree_ptr && node_ptr == rhs.node_ptr;
            }
            bool operator!=(const iterator &rhs) const {
                return tree_ptr != rhs.tree_ptr || node_ptr != rhs.node_ptr;
            }
            bool operator!=(const const_iterator &rhs) const {
                return tree_ptr != rhs.tree_ptr || node_ptr != rhs.node_ptr;
            }
            value_type* operator->() const noexcept {
                if (node_ptr) return node_ptr->data;
                return nullptr;
            }
        };
        class const_iterator {
            friend class map<Key, T, Compare>;
        private:
            const map<Key, T, Compare> *tree_ptr;
            avl_node *node_ptr;
        public:
            const_iterator():tree_ptr(nullptr), node_ptr(nullptr) {}

            const_iterator(const const_iterator &other)
                    :tree_ptr(other.tree_ptr), node_ptr(other.node_ptr) {}

            const_iterator(const iterator &other)
                    :tree_ptr(other.tree_ptr), node_ptr(other.node_ptr) {}

            const_iterator(const map<Key, T, Compare> *tp, avl_node *np)
                    :tree_ptr(tp), node_ptr(np) {}

            const_iterator operator++(int) {
                if (node_ptr == tree_ptr->tail) throw invalid_iterator();
                const_iterator ans(tree_ptr, node_ptr);
                if (node_ptr == tree_ptr->root) {
                    node_ptr = node_ptr->right;
                    if (node_ptr) {
                        while (node_ptr->left) node_ptr = node_ptr->left;
                    }
                    else node_ptr = tree_ptr->tail;
                    return ans;
                }
                else {
                    if (node_ptr == node_ptr->father->left) {
                        if (node_ptr->right) {
                            node_ptr = node_ptr->right;
                            while (node_ptr->left) node_ptr = node_ptr->left;
                            return ans;
                        }
                        else {
                            node_ptr = node_ptr->father;
                            return ans;
                        }
                    }
                    else {
                        if (node_ptr->right) {
                            node_ptr = node_ptr->right;
                            while (node_ptr->left) node_ptr = node_ptr->left;
                            return ans;
                        }
                        else {
                            while (node_ptr->father) {
                                if (node_ptr == node_ptr->father->right) {
                                    node_ptr = node_ptr->father;
                                    continue;
                                }
                                node_ptr = node_ptr->father;
                                return ans;
                            }
                            node_ptr = tree_ptr->tail;
                            return ans;
                        }
                    }
                }
            }
            const_iterator & operator++() {
                if (node_ptr == tree_ptr->tail) throw invalid_iterator();
                if (node_ptr == tree_ptr->root) {
                    node_ptr = node_ptr->right;
                    if (node_ptr) {
                        while (node_ptr->left) node_ptr = node_ptr->left;
                    }
                    else node_ptr = tree_ptr->tail;
                    return *this;
                }
                else {
                    if (node_ptr == node_ptr->father->left) {
                        if (node_ptr->right) {
                            node_ptr = node_ptr->right;
                            while (node_ptr->left) node_ptr = node_ptr->left;
                            return *this;
                        }
                        else {
                            node_ptr = node_ptr->father;
                            return *this;
                        }
                    }
                    else {
                        if (node_ptr->right) {
                            node_ptr = node_ptr->right;
                            while (node_ptr->left) node_ptr = node_ptr->left;
                            return *this;
                        }
                        else {
                            while (node_ptr->father) {
                                if (node_ptr == node_ptr->father->right) {
                                    node_ptr = node_ptr->father;
                                    continue;
                                }
                                node_ptr = node_ptr->father;
                                return *this;
                            }
                            node_ptr = tree_ptr->tail;
                            return *this;
                        }
                    }
                }
            }
            const_iterator operator--(int) {
                const_iterator ans(tree_ptr, node_ptr);
                if (node_ptr == tree_ptr->tail) {
                    node_ptr = tree_ptr->root;
                    if (!node_ptr) throw invalid_iterator();
                    while (node_ptr->right) node_ptr = node_ptr->right;
                    return ans;
                }
                else if (node_ptr == tree_ptr->root) {
                    node_ptr = node_ptr->left;
                    if (node_ptr) {
                        while (node_ptr->right) node_ptr = node_ptr->right;
                        return ans;
                    }
                    else throw invalid_iterator();
                }
                else {
                    if (node_ptr == node_ptr->father->left) {
                        if (node_ptr->left) {
                            node_ptr = node_ptr->left;
                            while (node_ptr->right) node_ptr = node_ptr->right;
                            return ans;
                        }
                        else {
                            while (node_ptr->father) {
                                if (node_ptr == node_ptr->father->left) {
                                    node_ptr = node_ptr->father;
                                    continue;
                                }
                                node_ptr = node_ptr->father;
                                return ans;
                            }
                            throw invalid_iterator();
                        }
                    }
                    else {
                        if (node_ptr->left) {
                            node_ptr = node_ptr->left;
                            while (node_ptr->right) node_ptr = node_ptr->right;
                            return ans;
                        }
                        else {
                            node_ptr = node_ptr->father;
                            return ans;
                        }
                    }
                }
            }
            const_iterator & operator--() {
                if (node_ptr == tree_ptr->tail) {
                    node_ptr = tree_ptr->root;
                    if (!node_ptr) throw invalid_iterator();
                    while (node_ptr->right) node_ptr = node_ptr->right;
                    return *this;
                }
                else if (node_ptr == tree_ptr->root) {
                    node_ptr = node_ptr->left;
                    if (node_ptr) {
                        while (node_ptr->right) node_ptr = node_ptr->right;
                        return *this;
                    }
                    throw invalid_iterator();
                }
                else {
                    if (node_ptr == node_ptr->father->left) {
                        if (node_ptr->left) {
                            node_ptr = node_ptr->left;
                            while (node_ptr->right) node_ptr = node_ptr->right;
                            return *this;
                        }
                        else {
                            while (node_ptr->father) {
                                if (node_ptr == node_ptr->father->left) {
                                    node_ptr = node_ptr->father;
                                    continue;
                                }
                                node_ptr = node_ptr->father;
                                return *this;
                            }
                            throw invalid_iterator();
                        }
                    }
                    else {
                        if (node_ptr->left) {
                            node_ptr = node_ptr->left;
                            while (node_ptr->right) node_ptr = node_ptr->right;
                            return *this;
                        }
                        else {
                            node_ptr = node_ptr->father;
                            return *this;
                        }
                    }
                }
            }

            const value_type & operator*() const {
                return node_ptr->data[0];
            }
            bool operator==(const iterator &rhs) const {
                return tree_ptr == rhs.tree_ptr && node_ptr == rhs.node_ptr;
            }
            bool operator==(const const_iterator &rhs) const {
                return tree_ptr == rhs.tree_ptr && node_ptr == rhs.node_ptr;
            }
            bool operator!=(const iterator &rhs) const {
                return tree_ptr != rhs.tree_ptr || node_ptr != rhs.node_ptr;
            }
            bool operator!=(const const_iterator &rhs) const {
                return tree_ptr != rhs.tree_ptr || node_ptr != rhs.node_ptr;
            }
            const value_type * operator->() const {
                if (node_ptr) return node_ptr->data;
                return nullptr;
            }
        };
    private:
        avl_node *root;
        avl_node *tail;
        Compare cmp;
        size_t num = 0;
        int height(avl_node *ptr) const {
            if (!ptr) return 0;
            else return ptr->height;
        }
        void clean(avl_node *&ptr) {
            if (!ptr) return;
            clean(ptr->left);
            clean(ptr->right);
            delete ptr;
            ptr = nullptr;
        }

        void LL_spin(avl_node *&ptr) {
            avl_node *tmp = ptr->left;

            ptr->left = tmp->right;
            if (tmp->right)tmp->right->father = ptr;

            tmp->right = ptr;
            tmp->father = ptr->father;
            ptr->father = tmp;

            ptr->height = std::max(height(ptr->left), height(ptr->right))+1;
            tmp->height = std::max(height(tmp->left), height(tmp->right))+1;
            ptr = tmp;
        }
        void RR_spin(avl_node *&ptr) {
            avl_node *tmp = ptr->right;

            ptr->right = tmp->left;
            if (tmp->left)tmp->left->father = ptr;

            tmp->left = ptr;
            tmp->father = ptr->father;
            ptr->father = tmp;

            ptr->height = std::max(height(ptr->left), height(ptr->right))+1;
            tmp->height = std::max(height(tmp->left), height(tmp->right))+1;
            ptr = tmp;
        }
        void LR_spin(avl_node *&ptr) {
            RR_spin(ptr->left);
            LL_spin(ptr);
        }
        void RL_spin(avl_node *&ptr) {
            LL_spin(ptr->right);
            RR_spin(ptr);
        }

        void push(const value_type &ec, avl_node *&ptr) {
            if (!root) root = new avl_node(ec, nullptr, nullptr, nullptr, 1);
            else if (cmp(ec.first, ptr->data->first)) {
                if (!ptr->left) {
                    ptr->left = new avl_node(ec, nullptr, nullptr, ptr, 1);
                    ptr->height = std::max(height(ptr->left), height(ptr->right))+1;
                }
                else push(ec, ptr->left);

                if (height(ptr->left) - height(ptr->right) == 2) {
                    if (cmp(ec.first, ptr->left->data->first)) LL_spin(ptr);
                    else if (cmp(ptr->left->data->first, ec.first)) LR_spin(ptr);
                }
            }
            else if (cmp(ptr->data->first, ec.first)) {
                if (!ptr->right) {
                    ptr->right = new avl_node(ec,nullptr, nullptr, ptr, 1);
                    ptr->height = std::max(height(ptr->left), height(ptr->right))+1;
                }
                else push(ec, ptr->right);

                if (height(ptr->right) - height(ptr->left) == 2) {
                    if (cmp(ptr->right->data->first, ec.first)) RR_spin(ptr);
                    else if (cmp(ec.first, ptr->right->data->first)) RL_spin(ptr);
                }
            }
            ptr->height = std::max(height(ptr->left), height(ptr->right))+1;
        }
        bool remove(const Key &key, avl_node *&ptr) {
            if (!ptr) return true;
            if ( !cmp(key, ptr->data->first) && !cmp(ptr->data->first, key) ) {
                if (!ptr->left && !ptr->right) {
                    delete ptr;
                    ptr = nullptr;
                    return false;
                }
                else if (!ptr->left) {
                    avl_node *tmp = ptr;
                    avl_node *dad = ptr->father;
                    ptr = ptr->right;
                    ptr->father = dad;
                    delete tmp; tmp = nullptr;
                    return false;
                }
                else if (!ptr->right) {
                    avl_node *tmp = ptr;
                    avl_node *dad = ptr->father;
                    ptr = ptr->left;
                    ptr->father = dad;
                    delete tmp; tmp = nullptr;
                    return false;
                }
                else {
                    avl_node *tmp = ptr->left;
                    while (tmp->right) tmp = tmp->right;

                    if (tmp != ptr->left) {
                        avl_node *ptr_f = ptr->father;
                        avl_node *ptr_l = ptr->left;
                        avl_node *ptr_r = ptr->right;
                        int ptr_h = ptr->height;
                        value_type ptr_val = ptr->data[0];

                        avl_node *tmp_f = tmp->father;
                        avl_node *tmp_l = tmp->left;
                        int tmp_h = tmp->height;
                        value_type tmp_val = tmp->data[0];

                        ptr->father = tmp_f;
                        tmp_f->right = ptr;
                        ptr->left = tmp_l;
                        if (tmp_l) tmp_l->father = ptr;
                        ptr->right = nullptr;
                        ptr->height = tmp_h;
//                    delete ptr->data; ptr->data = new value_type (tmp_val);

                        tmp->father = ptr_f;
                        tmp->left = ptr_l;
                        ptr_l->father = tmp;
                        tmp->right = ptr_r;
                        ptr_r->father = tmp;
                        tmp->height = ptr_h;
//                    delete tmp->data; tmp->data = new value_type (ptr_val);

                        ptr = tmp;

                        if (remove(key, ptr->left)) return true;
                        else return balance(ptr, 0);
                    }
                    else {
                        avl_node *ptr_f = ptr->father;
                        avl_node *ptr_r = ptr->right;
                        int ptr_h = ptr->height;
                        value_type ptr_val = ptr->data[0];

                        avl_node *tmp_l = tmp->left;
                        int tmp_h = tmp->height;
                        value_type tmp_val = tmp->data[0];

                        ptr->father = tmp;
                        ptr->left = tmp_l;
                        if (tmp_l) tmp_l->father = ptr;
                        ptr->right = nullptr;
                        ptr->height = tmp_h;
//                    delete ptr->data; ptr->data = new value_type (tmp_val);

                        tmp->father = ptr_f;
                        tmp->left = ptr;
                        tmp->right = ptr_r;
                        ptr_r->father = tmp;
                        tmp->height = ptr_h;
//                    delete tmp->data; tmp->data = new value_type (ptr_val);

                        ptr = tmp;
                        if (remove(key, ptr->left)) return true;
                        else return balance(ptr, 0);
                    }
                }
            }
            else if (cmp(key, ptr->data->first)) {
                if (remove(key, ptr->left)) return true;
                else return balance(ptr, 0);
            }
            else {
                if (remove(key, ptr->right)) return true;
                else return balance(ptr, 1);
            }
        }

        bool balance(avl_node *&ptr, const int &type) {
            if (type == 0) {
                if (height(ptr->left) == height(ptr->right)) {
                    --ptr->height;
                    return false;//balance: +1 ->0
                }
                else if (height(ptr->left) == height(ptr->right)-1) return true;//balance: 0 -> -1
                else {
                    if (height(ptr->right->left) > height(ptr->right->right)) {
                        RL_spin(ptr);
                        return false;
                    }
                    RR_spin(ptr);
                    if (height(ptr->right) == height(ptr->left)) return false;//h+1 -> h
                    return true;
                }
            }
            else {
                if (height(ptr->left) == height(ptr->right)) {
                    --ptr->height;
                    return false;
                }
                else if (height(ptr->right) == height(ptr->left)-1) return true;
                else if (height(ptr->right) == height(ptr->left)-2) {
                    if (height(ptr->left->left) < height(ptr->left->right)) {
                        LR_spin(ptr);
                        return false;
                    }
                    LL_spin(ptr);
                    if (height(ptr->right) == height(ptr->left)) return false;
                    return true;
                }
            }
            return false;
        }
        avl_node *search(const Key &key, avl_node *ptr) const {
            if (!ptr) return ptr;
            else {
                if (cmp(key, ptr->data->first)) return search(key, ptr->left);
                else if (cmp(ptr->data->first, key)) return search(key, ptr->right);
                else return ptr;
            }
        }
    public:
        map():root(nullptr) {
            tail = new avl_node;
        }
        map(const map &other) {
            tail = new avl_node;
            root = nullptr;
            for(const_iterator it = other.cbegin() ; it != other.cend() ; ++it)
                push(it.node_ptr->data[0], root);
            num = other.num;
        }
        map & operator=(const map &other) {
            if (this == &other) return *this;
            clean(root);
            for(const_iterator it = other.cbegin() ; it != other.cend() ; ++it)
                push(it.node_ptr->data[0], root);
            num = other.num;
            return *this;
        }
        ~map() {
            clear();
            delete tail;
        }
        T & at(const Key &key) {
            avl_node *ptr = search(key, root);
            if (!ptr) throw index_out_of_bound();
            else return ptr->data->second;
        }
        const T & at(const Key &key) const {
            avl_node *ptr = search(key, root);
            if (!ptr) throw index_out_of_bound();
            else return ptr->data->second;
        }
        T & operator[](const Key &key) {
            avl_node *ptr = search(key, root);
            if (!ptr) {
                value_type val(key, T());
                push(val, root);
                ++num;
                ptr = search(key, root);
                return ptr->data->second;
            }
            else return ptr->data->second;
        }
        const T & operator[](const Key &key) const {
            avl_node *ptr = search(key, root);
            if (!ptr) throw index_out_of_bound();
            else return ptr->data->second;
        }
        iterator begin() {
//        if (!root) throw invalid_iterator();
            avl_node *ptr = root;
            if (!ptr) return iterator(this, tail);
            while (ptr->left) ptr = ptr->left;
            return iterator(this, ptr);
        }
        const_iterator cbegin() const {
//        if (!root) throw invalid_iterator();
            avl_node *ptr = root;
            if (!ptr) return const_iterator(this, tail);
            while (ptr->left) ptr = ptr->left;
            return const_iterator(this, ptr);
        }
        iterator end() {
            return iterator(this, tail);
        }
        const_iterator cend() const {
            return const_iterator(this, tail);
        }
        bool empty() const {
            return root == nullptr;
        }
        size_t size() const {
            return num;
        }
        void clear() {
            clean(root);
            num = 0;
        }
        std::pair<iterator, bool> insert(const value_type &value) {
            avl_node *tp = search(value.first, root);
            if (tp) {
                iterator ans(this, tp);
                return std::pair<iterator, bool> (ans, false);
            }
            else {
                push(value, root);
                ++num;
                tp = search(value.first, root);
                iterator ans(this, tp);
                return std::pair<iterator, bool> (ans, true);
            }
        }
        void erase(iterator pos) {
            if (pos.tree_ptr != this) throw invalid_iterator();
            if (pos.node_ptr == tail) throw invalid_iterator();
            avl_node *ptr = search(pos.node_ptr->data->first, root);
            if (!ptr) throw invalid_iterator();
            remove(pos.node_ptr->data->first, root);
            --num;
        }

        size_t count(const Key &key) const {
            if (search(key, root)) return 1;
            else return 0;
        }

        iterator find(const Key &key) {
            avl_node *ptr = search(key, root);
            if (ptr) return iterator(this, ptr);
            else return iterator(this, tail);
        }
        const_iterator find(const Key &key) const {
            avl_node *ptr = search(key, root);
            if (ptr) return const_iterator(this, ptr);
            else return const_iterator(this, tail);
        }
    };

}

#endif //BPT_MAP_HPP
