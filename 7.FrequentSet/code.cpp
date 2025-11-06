#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <iomanip>
#include<cmath>

using namespace std;

int main(int argc, char* argv[]) {
    string input = (argc > 1) ? argv[1] : "input.csv";
    double minSupportPercent = (argc > 2) ? stod(argv[2]) : 30.0;

    vector<set<string>> transactions;
    ifstream file(input);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << input << endl;
        return 1;
    }

    string line;
    while (getline(file, line)) {
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());
        if (line.empty()) continue;

        stringstream ss(line);
        string item;
        set<string> transaction;
        while (getline(ss, item, ',')) {
            item.erase(remove_if(item.begin(), item.end(), ::isspace), item.end());
            if (!item.empty()) transaction.insert(item);
        }
        if (!transaction.empty()) transactions.push_back(transaction);
    }
    file.close();

    int totalTransactions = transactions.size();
    int minSupportCount = (int)ceil((minSupportPercent / 100.0) * totalTransactions);

    cout << "Total Transactions: " << totalTransactions << endl;
    cout << "Minimum Support: " << fixed << setprecision(1) << minSupportPercent << "% (" 
         << minSupportCount << " transactions)\n" << endl;

    // ---------- Frequent 1-itemsets ----------
    map<string, int> itemCount;
    for (auto &t : transactions)
        for (auto &it : t)
            itemCount[it]++;

    cout << "For k = 1:\n";
    cout << left << setw(20) << "Itemset" << setw(15) << "Frequency" << "Support\n";
    for (int i = 0; i < 55; i++) cout << "-";
    cout << endl;

    vector<string> L1;
    for (auto &p : itemCount) {
        double support = (p.second * 100.0) / totalTransactions;
        if (p.second >= minSupportCount) {
            cout << "[" << left << setw(17) << p.first << "]" 
                 << setw(10) << p.second 
                 << setw(10) << fixed << setprecision(0) << support << "%" << endl;
            L1.push_back(p.first);
        }
    }

    // ---------- Frequent 2-itemsets ----------
    cout << "\nFor k = 2:\n";
    cout << left << setw(30) << "Itemset" << setw(15) << "Frequency" << "Support\n";
    for (int i = 0; i < 55; i++) cout << "-";
    cout << endl;

    map<set<string>, int> pairCount;
    for (auto &t : transactions) {
        vector<string> items(t.begin(), t.end());
        for (size_t i = 0; i < items.size(); i++) {
            for (size_t j = i + 1; j < items.size(); j++) {
                set<string> pair = {items[i], items[j]};
                pairCount[pair]++;
            }
        }
    }

    for (auto &p : pairCount) {
        if (p.second >= minSupportCount) {
            double support = (p.second * 100.0) / totalTransactions;
            cout << "[";
            bool first = true;
            for (auto &it : p.first) {
                if (!first) cout << ", ";
                cout << it;
                first = false;
            }
            cout << "]" << setw(15 - (int)p.first.size()) << " "
                 << setw(10) << p.second
                 << setw(10) << fixed << setprecision(0) << support << "%" << endl;
        }
    }

    return 0;
}
