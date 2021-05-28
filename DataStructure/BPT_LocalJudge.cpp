#include <iostream>
#include <map>
#include <algorithm>
#include <ctime>
#include "BPT.hpp"
#define TESTSIZE 1000000
using namespace RA;
void init() {
    ofstream o("test_tree");
    o.close();
    o.open("test_data");
    o.close();
}
void testIntAll() {
    std::cout << "[TESTSIZE]" << TESTSIZE << '\n';
    std::map<int, vector<int>> answer;
    vector<int> store;
    init();
    srand(22333);
    int _key;
    BPlusTree<int, int, 500, 20000> bpt;
    bpt.initialize("test_tree", "test_data");
    for (int i = 0; i < TESTSIZE; i++) {
        _key = rand();
//        cout << "[i]" << i << " || [key]" << _key << '\n';
        store.push_back(_key);
        bpt.insert(_key, i);
        if (i % (TESTSIZE / 100) == 0)cout << "[count down] Inserting...\t" << i * 100 / TESTSIZE + 1 << "%" << endl;
    }
    for (int i = 0; i < TESTSIZE; i++) {
        if (i % 2 == 0) {
            answer[store[i]].push_back(i);
        }
    }
    cout << endl;
    int i;
    for (i = 0; i < TESTSIZE; i++) {
        if (i % 2 == 1) {
            if (!bpt.erase(store[i], i)) {
                cerr << "[error]erase failed when erasing\nkey: " << store[i] << "\tdata: " << i << endl;
                break;
            }
        }
        if (i % (TESTSIZE / 100) == 0)cout << "[count down] Erasing...\t" << i * 100 / TESTSIZE + 1 << "%" << endl;
    }
    cout << endl;


    int cnt = 0;
    size_t size = answer.size();
    for (const auto &i:answer) {
        vector<int> result;
        result = bpt.find(i.first);
        result.sort();
        if (i.second.size() != result.size()) {
            cerr << "[error]wrong size." << endl;
            cerr << "key: " << i.first << endl;
            cerr << "answer size: " << i.second.size() << endl;
            cerr << "answer:" << endl;
//            for (auto j : i.second)cerr << j << " ";
            cerr << endl;
            cerr << "result size: " << result.size() << endl;
            cerr << "result:" << endl;
            for (auto j : result)cerr << j << " ";
            cerr << endl;
        }
        else {
            for (int j = 0; j < i.second.size(); j++) {
                if (i.second[j] != result[j]) {
                    cerr << "[error]wrong answer." << endl;
                    cerr << "data: " << i.second[j] << endl;
                    cerr << "your data: " << result[j] << endl;
                }
            }
        }
        if (size/100 != 0 && cnt % (size / 100) == 0)cout << "[count down] Checking...\t" << cnt * 100 / size + 1 << "%" << endl;
        cnt++;
    }
    cout << "over\n";
    cout << endl;
}
void testRepeatedInsert() {
    init();
    int a = 100;
    BPlusTree<int, int>bpt;
    bpt.initialize("test_tree", "test_data");
    for (int i = 0 ; i < TESTSIZE ; ++i) {
        bpt.insert(a, i);
    }
    vector<int> tt;
    tt = bpt.find(a);
    if (tt.size() != TESTSIZE) {
        cout << "[tt.size]" << tt.size() << '\n';
        cout << "[answer]" << TESTSIZE << '\n';
        cerr << "[error] size wrong!\n";
    }
    else {
        tt.sort();
        int cnt = 0;
        for (auto i: tt) {
            if (i != cnt)cerr << "[error] element wrong!" << endl;
            cnt++;
        }
    }
    cout << "over\n";
}
int main() {
    clock_t startTime, endTime;
    startTime = clock();
    testIntAll();
    endTime = clock();
    cout << "The run time is: " << (double)(endTime-startTime)/CLOCKS_PER_SEC << "s\n";
    return 0;
}
