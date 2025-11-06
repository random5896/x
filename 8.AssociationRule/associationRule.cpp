#include <bits/stdc++.h>
using namespace std;

int main() {
    string input = "input.csv";
    ifstream file(input);
    if (!file.is_open()) {
        cout << "Error: Could not open file.\n";
        return 0;
    }

    vector<set<string>> transactions;
    string line;

    // Read transactions
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string item;
        set<string> trans;
        while (getline(ss, item, ',')) {
            item.erase(remove_if(item.begin(), item.end(), ::isspace), item.end());
            if (!item.empty()) trans.insert(item);
        }
        if (!trans.empty()) transactions.push_back(trans);
    }
    file.close();

    int N = transactions.size();
    double minSupportPercent, minConfidencePercent;

    cout << "Enter minimum support percentage (%): ";
    cin >> minSupportPercent;
    cout << "Enter minimum confidence percentage (%): ";
    cin >> minConfidencePercent;

    map<set<string>, int> support;

    // Count 1-itemsets and 2-itemsets
    for (auto &t : transactions) {
        for (auto &a : t)
            support[{a}]++;

        vector<string> v(t.begin(), t.end());
        for (size_t i = 0; i < v.size(); i++) {
            for (size_t j = i + 1; j < v.size(); j++) {
                set<string> pair = {v[i], v[j]};
                support[pair]++;
            }
        }
    }

    int minSupportCount = ceil((minSupportPercent / 100.0) * N);

    cout << "\n=============================================\n";
    cout << "     Association Rule Mining Results\n";
    cout << "=============================================\n";
    cout << "Total Transactions: " << N << endl;
    cout << "Min Support Count: " << minSupportCount 
         << " (" << minSupportPercent << "%)" << endl;
    cout << "Min Confidence: " << minConfidencePercent << "%\n";

    cout << "\nFrequent 1-itemsets:\n";
    cout << "Itemset\t\tCount\tSupport\n";
    for (auto &s : support) {
        if (s.first.size() == 1 && s.second >= minSupportCount) {
            double sup = (double)s.second / N * 100;
            for (auto &x : s.first)
                cout << "[" << x << "]\t\t" << s.second << "\t" << fixed << setprecision(1) << sup << "%\n";
        }
    }

    cout << "\nFrequent 2-itemsets:\n";
    cout << "Itemset\t\t\tCount\tSupport\n";
    for (auto &s : support) {
        if (s.first.size() == 2 && s.second >= minSupportCount) {
            double sup = (double)s.second / N * 100;
            cout << "[";
            int c = 0;
            for (auto &x : s.first) {
                cout << x;
                if (++c < 2) cout << ", ";
            }
            cout << "]\t\t" << s.second << "\t" << fixed << setprecision(1) << sup << "%\n";
        }
    }

    cout << "\n--- Association Rules ---\n";
    for (auto &s : support) {
        if (s.first.size() == 2 && s.second >= minSupportCount) {
            vector<string> items(s.first.begin(), s.first.end());
            string A = items[0], B = items[1];
            int supAB = s.second;
            int supA = support[{A}];
            int supB = support[{B}];

            double confAtoB = (double)supAB / supA * 100;
            double confBtoA = (double)supAB / supB * 100;
            double supPercent = (double)supAB / N * 100;

            if (confAtoB >= minConfidencePercent)
                cout << "[" << A << "] => [" << B << "]  (Support: " << fixed << setprecision(1)
                     << supPercent << "%, Confidence: " << confAtoB << "%)\n";

            if (confBtoA >= minConfidencePercent)
                cout << "[" << B << "] => [" << A << "]  (Support: " << fixed << setprecision(1)
                     << supPercent << "%, Confidence: " << confBtoA << "%)\n";
        }
    }

    cout << "\n✅ Association Rule Generation Completed Successfully!\n";
    return 0;
}
