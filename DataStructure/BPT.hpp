//
// Created by 18163 on 2021/4/23.
//

#ifndef BPT_BPT_HPP
#define BPT_BPT_HPP
#include <iostream>
#include "Vector.hpp"
#include <fstream>
#include "exception.hpp"
#include "Alloc.hpp"
using namespace std;
namespace RA {
    //[1, size+1)
    template<typename Key>
    int lower_bound(const Key *index, int size, Key _key) {
        int left = 1, right = size+1;
        while (left < right) {
            int mid = left + (right-left)/2;
            if (index[mid] < _key) left = mid+1;
            else right = mid;
        }
        return left;
    }
    template<typename Key>
    int upper_bound(const Key *index, int size, Key _key) {
        int left = 1, right = size+1;
        while (left < right) {
            int mid = left + (right-left)/2;
            if (_key < index[mid]) right = mid;
            else left = mid+1;
        }
        return left;
    }
}
namespace RA {
    template<typename Key, typename Data, int MAX = 200, int CACHESIZE = 500>
    class BPlusTree {
        typedef std::pair<Key, Data> value_type;
//        static const int MAX = 200;
        static const int MIN = MAX/2;
    private:
        struct tree_info {
            int file_end = 0;
            int head = -1;
            int root = -1;
        };
        class node {
            friend class BPlusTree;
            int Next = 0;
            /* if      Next == 0 :  the node is an internal node.
             * else if Next == -1:  the node is an leaf node, and Next point to seqlist's end.
             * else if Next  > 0 :  the node is an leaf node, and Next point to next block's address.
             */
            int Prev = -1;
            int Father = -1;
            int Address[MAX+1] {};
            Key Index[MAX+1] {};
            int Size = 0;
            /*  key值的数目   */
        public:
            node() {
                for(int & i : Address) i = -1;
            }

            node(int nxt, int prv, int father, int sz): Next(nxt), Prev(prv), Father(father), Size(sz) {
                for(int & i : Address) i = -1;
            }

            node(const node &ec) : Next(ec.Next), Prev(ec.Prev), Size(ec.Size), Father(ec.Father) {
                for (int i = 0 ; i <= Size ; ++i)
                    Address[i] = ec.Address[i];
                for (int i = 0 ; i <= Size ; ++i)
                    Index[i] = ec.Index[i];
            }

            node &operator=(const node &ec) {
                if (this == &ec) return *this;
                Father = ec.Father;
                Size = ec.Size;
                Next = ec.Next;
                Prev = ec.Prev;
                for (int i = 0 ; i <= Size ; ++i)
                    Address[i] = ec.Address[i];
                for (int i = 1 ; i <= Size ; ++i)
                    Index[i] = ec.Index[i];
                return *this;
            }

            bool operator!=(const node &ec) {
                if (this == &ec) return false;
                if (Size != ec.Size) return true;
                if (Next != ec.Next) return true;
                if (Next == 0) {
                    for (int i = 0 ; i <= Size ; ++i) {
                        if (Address[i] != ec.Address[i]) return true;
                    }
                    for (int i = 1 ; i <= Size ; ++i) {
                        if (Index[i] < ec.Index[i] || ec.Index[i] < Index[i]) return true;
                    }
                }
                else {
                    for (int i = 1 ; i <= Size ; ++i) {
                        if (Address[i] != ec.Address[i]) return true;
                    }
                    for (int i = 1 ; i <= Size ; ++i) {
                        if (Index[i] < ec.Index[i] || ec.Index[i] < Index[i]) return true;
                    }
                }
                if (Father != ec.Father) return true;
                if (Prev != ec.Prev) return true;
                return false;
            }

            ~node() = default;
        };
    private:
        tree_info treeInfo;
        /*  树节点文件的末端地址       */
        int data_end = sizeof(int);
        /*  数据记录文件的末端地址     */
        DiskMonitor<node, tree_info, CACHESIZE> bptFile;
        /*  关联树节点记录文件        */
        DiskMonitor<value_type, int, CACHESIZE> dataFile;
        /*  关联原始数据记录文件       */
    private:
        bool isEmpty() const {
            return treeInfo.root == -1;
        }
/*------search-by-B+tree-index---------------------------------------------------------*/
        int Scan_to_End(const Key &key, node &ntr) {
            int son_node = treeInfo.root;
            bptFile.read(treeInfo.root, ntr);
            while (ntr.Next == 0) {
                int i = upper_bound(ntr.Index, ntr.Size, key);
                if (ntr.Index[ntr.Size] < key) son_node = ntr.Address[ntr.Size];
                else son_node = ntr.Address[i-1];
                //第一个严格大于key的位置
                bptFile.read(son_node, ntr);
            }
            return son_node;
        }
/*------collect-all-key-element--------------------------------------------------------*/
        void Data_Find (const Key &key, vector<Data> &container) {
            node ntr;
            value_type record;
            int ntr_address;
            Scan_to_End(key, ntr);
            while (true) {
                for (int i = lower_bound(ntr.Index, ntr.Size, key) ; i <= ntr.Size ; ++i) {
                    if (key < ntr.Index[i]) break;
                    else if (ntr.Index[i] < key) continue;
                    else {
                        dataFile.read(ntr.Address[i], record);
                        container.push_back(record.second);
                    }
                }
                ntr_address = ntr.Prev;
                if (ntr_address == -1) break;
                bptFile.read(ntr_address, ntr);
                if (ntr.Index[ntr.Size] < key) break;
            }
        }
/*------insert-&&-adjust---------------------------------------------------------------*/
        void Insert(const Key &key, const Data &data) {
            value_type value(key, data);
            if (isEmpty()) {
                node nd(-1, -1, -1, 1);
                //初始根节点为叶节点
                nd.Index[1] = key;
                nd.Address[1] = data_end;
                bptFile.write(treeInfo.file_end, nd);
                treeInfo.root = treeInfo.head = treeInfo.file_end;
                treeInfo.file_end += sizeof(nd);

                dataFile.write(data_end, value);
                data_end += sizeof(value_type);
            }
            else {
                node ntr; int i;
                int ntr_address = Scan_to_End(key, ntr);
                if (!(key < ntr.Index[ntr.Size])) i = ntr.Size+1;
                else i = upper_bound(ntr.Index, ntr.Size, key);

                for (int j = ntr.Size ; j >= i ; --j) {
                    ntr.Index[j+1] = ntr.Index[j];
                    ntr.Address[j+1] = ntr.Address[j];
                }
                //在叶节点中插入
                ntr.Index[i] = key;
                ntr.Address[i] = data_end;
                ntr.Size++;

                dataFile.write(data_end, value);
                data_end += sizeof(value_type);

                bptFile.write(ntr_address, ntr);
                Insert_Adjust(ntr_address, ntr);
            }
        }

        void Insert_Adjust(int pos, node nd) {
            int ntr_address = pos, bro_address;
            node ntr(nd), rr;
            while (ntr.Size == MAX) {
                Key k = ntr.Index[MAX/2+1];
                node bro(ntr.Next, ntr_address, ntr.Father, 0);
                if (ntr.Next != 0) {
                    int i = MAX/2;
                    bro.Size = ntr.Size-i;
                    ntr.Size = i;
                    for (int j = 1 ; j <= bro.Size ; ++j) {
                        bro.Index[j] = ntr.Index[j+i];
                        bro.Address[j] = ntr.Address[j+i];
                    }
                    bro_address = treeInfo.file_end;
                    if (ntr.Next != -1) {
                        bptFile.read(bro.Next, rr);
                        rr.Prev = bro_address;
                        bptFile.write(ntr.Next, rr);
                    }
                    ntr.Next = bro_address;
                    treeInfo.file_end += sizeof(bro);
                }
                else {
                    node son;
                    int i = MAX/2 + 1;
                    bro.Size = ntr.Size-i;
                    ntr.Size = i-1;
                    for (int j = 0 ; j <= bro.Size ; ++j) {
                        bro.Index[j] = ntr.Index[j+i];
                        bro.Address[j] = ntr.Address[j+i];
                    }
                    bro_address = treeInfo.file_end;
                    treeInfo.file_end += sizeof(bro);
                    for (int j = 0 ; j <= bro.Size ; ++j) {
                        bptFile.read(ntr.Address[j+i], son);
                        son.Father = bro_address;
                        bptFile.write(ntr.Address[j+i], son);
                    }
                }
                if (ntr.Father == -1) {
                    node rt(0, -1, -1, 1);
                    //新节点必为内部节点
                    rt.Index[1] = k;
                    rt.Address[0] = ntr_address;
                    rt.Address[1] = bro_address;
                    ntr.Father = bro.Father = treeInfo.file_end;
                    bptFile.write(ntr_address, ntr);
                    bptFile.write(bro_address, bro);
                    bptFile.write(treeInfo.file_end, rt);
                    treeInfo.root = treeInfo.file_end;
                    treeInfo.file_end += sizeof(rt);
                    break;
                }
                else {
                    bptFile.write(ntr_address, ntr);
                    bptFile.write(bro_address, bro);
                    int father_address = ntr.Father;
                    bptFile.read(father_address, ntr);
                    int j;
                    for (j = 0 ; j <= ntr.Size ; ++j)
                        if (ntr.Address[j] == ntr_address) break;
                    if (j > ntr.Size) {
                        cerr << "INSERT ADJUST ERROR\n";
                        throw bad_tree_struct();
                    }
                    for (int p = ntr.Size ; p >= j+1 ; --p) {
                        ntr.Index[p+1] = ntr.Index[p];
                        ntr.Address[p+1] = ntr.Address[p];
                    }
                    ntr.Index[j+1] = k;
                    ntr.Address[j+1] = bro_address;
                    ntr.Size++;
                    ntr_address = father_address;
                    bptFile.write(ntr_address, ntr);
                }
            }
        }
/*------erase-&&-adjust----------------------------------------------------------------*/
        bool Erase(const Key &key, const Data &data) {
            node ntr;
            value_type record;
            int ntr_address = Scan_to_End(key, ntr);
            while (true) {
                for (int i = lower_bound(ntr.Index, ntr.Size, key) ; i <= ntr.Size ; ++i) {
                    if (key < ntr.Index[i]) break;
                    if (ntr.Index[i] < key) continue;
                    dataFile.read(ntr.Address[i], record);
                    if (record.second == data) {
                        for (int j = i ; j < ntr.Size ; ++j) {
                            ntr.Index[j] = ntr.Index[j+1];
                            ntr.Address[j] = ntr.Address[j+1];
                        }
                        --ntr.Size;
                        bptFile.write(ntr_address, ntr);

                        if (i == 1) { Erase_Check(ntr_address, ntr, ntr.Index[1]); }

                        if (ntr_address != treeInfo.root) Erase_Adjust(ntr_address, ntr);
                        else { if (ntr.Size == 0) treeInfo.root = -1; }
                        return true;
                    }
                }
                ntr_address = ntr.Prev;
                if (ntr_address == -1) break;
                bptFile.read(ntr_address, ntr);
                if (ntr.Index[ntr.Size] < key) break;
            }
            return false;
        }

        void Erase_Check(int pos, node nd, const Key &k) {
            node ntr(nd), father;
            int ntr_address(pos), father_address(ntr.Father), i;

            while (father_address != -1) {
                bptFile.read(father_address, father);
                for (i = 0 ; i <= father.Size ; ++i)
                    if (father.Address[i] == ntr_address) break;

                if (i == 0) {
                    ntr_address = father_address;
                    father_address = father.Father;
                }
                else {
                    father.Index[i] = k;
                    bptFile.write(father_address, father);
                    break;
                }
            }
        }

        void Erase_Adjust(int pos, node nd) {
            node ntr(nd), rightBrother, leftBrother, rr, ll, father;
            int ntr_address = pos, father_address;
            while (true) {
                if (ntr.Father == -1) break;
                if (ntr.Next != 0) {
                    if (ntr.Size >= MIN) break;
                    if (ntr.Next != -1) {
                        bptFile.read(ntr.Next, rightBrother);
                        if (ntr.Size + rightBrother.Size >= 2*MIN && ntr.Father == rightBrother.Father) {
                            Borrow_from_Right_Leaf(ntr, rightBrother);
                            bptFile.write(ntr_address, ntr);
                            bptFile.write(ntr.Next, rightBrother);
                            bptFile.read(ntr.Father, father);
                            int i;
                            for (i = 0 ;  i <= father.Size ; ++i)
                                if (father.Address[i] == ntr_address) break;
                            father.Index[i+1] = rightBrother.Index[1];
                            bptFile.write(ntr.Father, father);
                            break;
                        }
                    }
                    if (ntr.Prev != -1) {
                        bptFile.read(ntr.Prev, leftBrother);
                        if (ntr.Size + leftBrother.Size >= 2*MIN && ntr.Father == leftBrother.Father) {
                            Borrow_from_Left_Leaf(ntr, leftBrother);
                            bptFile.write(ntr_address, ntr);
                            bptFile.write(ntr.Prev, leftBrother);
                            bptFile.read(ntr.Father, father);
                            int i;
                            for (i = 0 ; i <= father.Size ; ++i)
                                if (father.Address[i] == ntr_address) break;
                            father.Index[i] = ntr.Index[1];
                            bptFile.write(ntr.Father, father);
                            break;
                        }
                    }
                    //拒绝具有不同父节点的叶节点之间的借子
                    /*------------------merge-bro------------------------------------------*/

                    if (ntr.Next != -1 && ntr.Father == rightBrother.Father) {
                        //合并节点
                        for (int j = 1 ; j <= rightBrother.Size ; ++j) {
                            ntr.Index[ntr.Size+j] = rightBrother.Index[j];
                            ntr.Address[ntr.Size+j] = rightBrother.Address[j];
                        }
                        ntr.Size += rightBrother.Size;
                        ntr.Next = rightBrother.Next;
                        if (rightBrother.Next != -1) {
                            bptFile.read(rightBrother.Next, rr);
                            rr.Prev = ntr_address;
                            bptFile.write(rightBrother.Next, rr);
                        }
                        bptFile.write(ntr_address, ntr);
                        //合并节点

                        //更新父节点
                        father_address = ntr.Father;
                        bptFile.read(father_address, ntr);
                        //把父亲点读入ntr
                        int i;
                        for (i = 0 ; i <= ntr.Size ; ++i)
                            if (ntr.Address[i] == ntr_address) break;
                        if (i >= ntr.Size) {
                            cerr << "ERASE ADJUST: MERGE RIGHT: LEAF BAD FATHER\n";
                            throw bad_tree_struct();
                        }
                        for (int j = i+1 ; j < ntr.Size ; ++j) {
                            ntr.Address[j] = ntr.Address[j+1];
                            ntr.Index[j] = ntr.Index[j+1];
                        }
                        ntr.Size--;
                        if (ntr.Size == 0) {
                            if (treeInfo.root != father_address) {
                                cerr << "ERASE ADJUST: MERGE RIGHT: EMPTY LEAF NODE\n";
                                throw bad_tree_struct();
                            }
                            //根节点唯二的两个子节点合并，根节点消失，无需更新父节点，终止。
                            treeInfo.root = ntr_address;
                            bptFile.read(ntr_address, ntr);
                            ntr.Father = -1;
                            bptFile.write(ntr_address, ntr);
                            break;
                        }
                        ntr_address = father_address;
                        bptFile.write(ntr_address, ntr);
                        //更新父节点
                        continue;
                    }
                    if (ntr.Prev != -1 && ntr.Father == leftBrother.Father) {
                        //合并节点
                        for (int i = ntr.Size ; i >= 1 ; --i) {
                            ntr.Index[leftBrother.Size+i] = ntr.Index[i];
                            ntr.Address[leftBrother.Size+i] = ntr.Address[i];
                        }
                        for (int i = 1 ; i <= leftBrother.Size ; ++i) {
                            ntr.Index[i] = leftBrother.Index[i];
                            ntr.Address[i] = leftBrother.Address[i];
                        }
                        ntr.Size += leftBrother.Size;
                        ntr.Prev = leftBrother.Prev;
                        if (leftBrother.Prev != -1) {
                            bptFile.read(leftBrother.Prev, ll);
                            ll.Next = ntr_address;
                            bptFile.write(leftBrother.Prev, ll);
                        }
                        else treeInfo.head = ntr_address;
                        bptFile.write(ntr_address, ntr);
                        //合并节点

                        //更新父节点
                        father_address = ntr.Father;
                        bptFile.read(father_address, ntr);
                        int i;
                        for (i = 0 ; i <= ntr.Size ; ++i)
                            if (ntr.Address[i] == ntr_address) break;
                        if (i > ntr.Size || i == 0) {
                            cerr << "ERASE ADJUST: MERGE LEFT: LEAF BAD FATHER\n";
                            throw bad_tree_struct();
                        }
                        ntr.Index[i] = ntr.Index[i-1];
                        //DEBUG:merge过后新的叶节点的key值改变
                        for (int j = i-1 ; j < ntr.Size ; ++j) {
                            ntr.Index[j] = ntr.Index[j+1];
                            ntr.Address[j] = ntr.Address[j+1];
                        }
                        --ntr.Size;
                        if (ntr.Size == 0) {
                            if (treeInfo.root != father_address) {
                                cerr << "ERASE ADJUST: MERGE LEFT: EMPTY LEAF NODE\n";
                                throw bad_tree_struct();
                            }
                            //根节点唯二的两个子节点合并，根节点消失，无需更新父节点，终止。
                            treeInfo.root = ntr_address;
                            bptFile.read(ntr_address, ntr);
                            ntr.Father = -1;
                            bptFile.write(ntr_address, ntr);
                            break;
                        }
                        ntr_address = father_address;
                        bptFile.write(ntr_address, ntr);
                        continue;
                    }
                    //拒绝具有不同父节点的叶节点之间的合并
                    throw unknown_field();
                }
                else {
                    if (ntr.Size+1 >= MIN) break;
                    int right_address = -1, left_address = -1;
                    father_address = ntr.Father;
                    bptFile.read(father_address, father);
                    int i;
                    for (i = 0 ; i <= father.Size ; ++i)
                        if (father.Address[i] == ntr_address) break;
                    if (i > father.Size) {
                        cerr << "ERASE ADJUST: INTERN BAD FATHER\n";
                        throw bad_tree_struct();
                    }
/*------------------borrow-from-bro--------------------------------------------*/
                    if (i < father.Size) {
                        right_address = father.Address[i+1];
                        bptFile.read(right_address, rightBrother);
                        if (ntr.Size + rightBrother.Size >= 2*MIN-2) {
                            //借右兄弟的内部节点，只借一个
                            node son;
                            bptFile.read(rightBrother.Address[0], son);
                            son.Father = ntr_address;
                            bptFile.write(rightBrother.Address[0], son);

                            Borrow_from_Right_Intern(ntr, rightBrother, father, i+1);

                            bptFile.write(ntr_address, ntr);
                            bptFile.write(right_address, rightBrother);
                            bptFile.write(father_address, father);
                            break;
                        }
                    }
                    if (i > 0) {
                        left_address = father.Address[i-1];
                        bptFile.read(left_address, leftBrother);
                        if (ntr.Size + leftBrother.Size >= 2*MIN-2) {
                            //借左兄弟的内部节点，只借一个
                            node son;
                            bptFile.read(leftBrother.Address[leftBrother.Size], son);
                            son.Father = ntr_address;
                            bptFile.write(leftBrother.Address[leftBrother.Size], son);

                            Borrow_from_Left_Intern(ntr, leftBrother, father, i);

                            bptFile.write(ntr_address, ntr);
                            bptFile.write(left_address, leftBrother);
                            bptFile.write(father_address, father);
                            break;
                        }
                    }
                    //必定是同父节点之间的借子
/*------------------merge-bro--------------------------------------------------*/
                    if (i < father.Size) {
                        ntr.Size++;
                        node son;
                        for (int j = 0 ; j <= rightBrother.Size ; ++j) {
                            ntr.Index[ntr.Size+j] = rightBrother.Index[j];
                            ntr.Address[ntr.Size+j] = rightBrother.Address[j];
                            bptFile.read(rightBrother.Address[j], son);
                            son.Father = ntr_address;
                            bptFile.write(rightBrother.Address[j], son);
                        }
                        ntr.Index[ntr.Size] = father.Index[i+1];
                        ntr.Size += rightBrother.Size;
                        bptFile.write(ntr_address, ntr);
                        ntr = father;
                        for (int j = i+1 ; j < ntr.Size ; ++j) {
                            ntr.Index[j] = ntr.Index[j+1];
                            ntr.Address[j] = ntr.Address[j+1];
                        }
                        ntr.Size--;
                        if (ntr.Size == 0) {
                            if (treeInfo.root != father_address) {
                                cerr << "ERASE ADJUST: MERGE RIGHT: EMPTY INTERN NODE\n";
                                throw bad_tree_struct();
                            }
                            //根节点唯二的两个子节点合并，根节点消失，无需更新父节点，终止。
                            treeInfo.root = ntr_address;
                            bptFile.read(ntr_address, ntr);
                            ntr.Father = -1;
                            bptFile.write(ntr_address, ntr);
                            break;
                        }
                        ntr_address = father_address;
                        bptFile.write(ntr_address, ntr);
                        continue;
                    }
                    if (i > 0) {
                        node son;
                        ntr.Index[0] = father.Index[i];
                        for(int j = ntr.Size ; j >= 0 ; --j) {
                            ntr.Index[j+leftBrother.Size+1] = ntr.Index[j];
                            ntr.Address[j+leftBrother.Size+1] = ntr.Address[j];
                        }
                        for(int j = 0 ; j <= leftBrother.Size ; ++j) {
                            ntr.Index[j] = leftBrother.Index[j];
                            ntr.Address[j] = leftBrother.Address[j];
                            bptFile.read(leftBrother.Address[j], son);
                            son.Father = ntr_address;
                            bptFile.write(leftBrother.Address[j], son);
                        }
                        ntr.Size += leftBrother.Size+1;
                        bptFile.write(ntr_address, ntr);
                        father.Index[i] = father.Index[i-1];
                        //DEBUG:类似叶节点
                        ntr = father;
                        for (int j = i-1 ; j < ntr.Size ; ++j) {
                            ntr.Index[j] = ntr.Index[j+1];
                            ntr.Address[j] = ntr.Address[j+1];
                        }
                        ntr.Size--;
                        if (ntr.Size == 0) {
                            if (treeInfo.root != father_address) {
                                cerr << "ERASE ADJUST: MERGE LEFT: EMPTY INTERN NODE\n";
                                throw bad_tree_struct();
                            }
                            //根节点唯二的两个子节点合并，根节点消失，无需更新父节点，终止。
                            treeInfo.root = ntr_address;
                            bptFile.read(ntr_address, ntr);
                            ntr.Father = -1;
                            bptFile.write(ntr_address, ntr);
                            break;
                        }
                        ntr_address = father_address;
                        bptFile.write(ntr_address, ntr);
                        continue;
                    }
                    //必定是同父节点之间的合并
                    throw unknown_field();
                }
            }
        }

        bool Modify(const Key &key, const Data &data, const Data &update) {
            node ntr;
            value_type record;
            int ntr_address = Scan_to_End(key, ntr);
            while (true) {
                for (int i = lower_bound(ntr.Index, ntr.Size, key) ; i <= ntr.Size ; ++i) {
                    if (key < ntr.Index[i]) break;
                    if (ntr.Index[i] < key) continue;

                    dataFile.read(ntr.Address[i], record);
                    if (record.second == data) {
                        record.second = update;
                        dataFile.write(ntr.Address[i], record);
                        return true;
                    }
                }
                ntr_address = ntr.Prev;
                if (ntr_address == -1) break;
                bptFile.read(ntr_address, ntr);
                if (ntr.Index[ntr.Size] < key) break;
            }
            return false;
        }
/*------borrow-function----------------------------------------------------------------*/
        void Borrow_from_Right_Leaf(node &ntr, node &rightBrother) {
            int n = MIN - ntr.Size;
            for (int i = 1 ; i <= n ; ++i) {
                ntr.Index[ntr.Size+i] = rightBrother.Index[i];
                ntr.Address[ntr.Size+i] = rightBrother.Address[i];
            }
            for (int i = 1 ; i <= rightBrother.Size-n ; ++i) {
                rightBrother.Index[i] = rightBrother.Index[i+n];
                rightBrother.Address[i] = rightBrother.Address[i+n];
            }
            ntr.Size += n;
            rightBrother.Size -= n;
        }
        void Borrow_from_Left_Leaf(node &ntr, node &leftBrother) {
            int n = MIN - ntr.Size;
            for (int i = ntr.Size ; i >= 1 ; --i) {
                ntr.Index[i+n] = ntr.Index[i];
                ntr.Address[i+n] = ntr.Address[i];
            }
            for (int i = 1 ; i <= n ; ++i) {
                ntr.Index[i] = leftBrother.Index[leftBrother.Size-n+i];
                ntr.Address[i] = leftBrother.Address[leftBrother.Size-n+i];
            }
            ntr.Size += n;
            leftBrother.Size -= n;
        }
        void Borrow_from_Right_Intern(node &ntr, node &rightBrother, node &Father, int p) {
            //只借一个
            ntr.Size++;
            ntr.Index[ntr.Size] = Father.Index[p];
            ntr.Address[ntr.Size] = rightBrother.Address[0];
            Father.Index[p] = rightBrother.Index[1];
            for (int i = 0 ; i < rightBrother.Size ; ++i) {
                rightBrother.Index[i] = rightBrother.Index[i+1];
                rightBrother.Address[i] = rightBrother.Address[i+1];
            }
            rightBrother.Size--;
        }
        void Borrow_from_Left_Intern(node &ntr, node &leftBrother, node &Father, int p) {
            //只借一个
            for (int i = ntr.Size ; i >= 0 ; --i) {
                ntr.Index[i+1] = ntr.Index[i];
                ntr.Address[i+1] = ntr.Address[i];
            }
            ntr.Index[1] = Father.Index[p];
            Father.Index[p] = leftBrother.Index[leftBrother.Size];
            ntr.Address[0] = leftBrother.Address[leftBrother.Size];
            leftBrother.Size--;
            ntr.Size++;
        }

    public:
/*------constructor-&&-destructor------------------------------------------------------*/
        BPlusTree() = default;

        void initialize(const string &file1, const string &file2) {
            bptFile.initialize(file1);
            dataFile.initialize(file2);
            if (bptFile.empty()) {
                treeInfo.file_end = sizeof(treeInfo);
                bptFile.write_HeadInfo(treeInfo);
            }
            else bptFile.read_HeadInfo(treeInfo);

            if (dataFile.empty()) {
                data_end = sizeof(data_end);
                dataFile.write_HeadInfo(data_end);
            }
            else dataFile.read_HeadInfo(data_end);
        }

        ~BPlusTree() {
            bptFile.write_HeadInfo(treeInfo);
            dataFile.write_HeadInfo(data_end);
        }

/*------external-interface-------------------------------------------------------------*/
        vector<Data> find(const Key &key) {
            vector<Data> rt;
            Data_Find(key, rt);
            return rt;
        }

        bool erase(const Key &key, const Data &data) { return Erase(key, data); }

        bool erase(const value_type &value) { return Erase(value.first, value.second); }

        void insert(const Key &key, const Data &data) { Insert(key, data); }

        void insert(const value_type &value) { Insert(value.first, value.second); }

        bool modify(const Key &key, const Data &data, const Data &update) { return Modify(key, data, update); }

        bool modify(const value_type &value, const Data &update) { return Modify(value.first, value.second, update); }

        //--------------------------
        void seqlist_show() {
            node ntr;
            int ntr_address = treeInfo.head;
            while (ntr_address != -1) {
                bptFile.read(ntr_address, ntr);
                leaf_node_show(ntr);
                ntr_address = ntr.Next;
            }
        }

        void leaf_node_show(node ntr) {
            value_type record;
            cout << "[leaf node]\n";
            cout << "[father]" << ntr.Father << '\n';
            cout << "[size]" << ntr.Size << '\n';
            for (int i = 1 ; i <= ntr.Size ; ++i) {
                cout << "[node record]" << ntr.Index[i] << ": ";
                dataFile.read(ntr.Address[i], record);
                cout << record.first << ", " << record.second << ";\n";
            }
            cout << "[prev]" << ntr.Prev << '\n';
            cout << "[next]" << ntr.Next << '\n';
            cout << "---------------------------\n";
        }

        void tree_show() {
            int ntr_address = treeInfo.root;
            node ntr;
            bptFile.read(ntr_address, ntr);
            if (ntr.Next != 0) {
                cout << "[intern node]: NULL";
                return;
            }
            intern_node_show(ntr, ntr.Index[1]);
        }

        void intern_node_show(node ntr, Key key) {
            if (ntr.Next != 0) return;

            if (ntr.Father == -1) cout << "[root node]\n";
            else cout << "[intern node]\n[father first key] " << key << '\n';
            cout << "[father]" << ntr.Father << '\n';
            cout << "[size]" << ntr.Size << '\n';
            cout << "[index] ";
            for(int i = 1 ; i <= ntr.Size ; ++i) {
                cout << ntr.Index[i] << "; ";
            }
            cout << '\n';
            cout << "[address] ";
            for (int i = 0 ; i <= ntr.Size ; ++i) {
                cout << ntr.Address[i] << "; ";
            }
            cout << "\n---------------------------\n";
            for (int i = 0 ; i <= ntr.Size ; ++i) {
                node son;
                bptFile.read(ntr.Address[i], son);
                intern_node_show(son, ntr.Index[1]);
            }

        }
    };
}
#endif //BPT_BPT_HPP