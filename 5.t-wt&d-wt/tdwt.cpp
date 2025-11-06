#include <bits/stdc++.h>
#include <iomanip>
using namespace std;

int main() {
    string filename = "input.csv"; // Input CSV file
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << " Error: Could not open " << filename << endl;
        return 1;
    }

    string line, classType, playType, countStr;
    int count;
    bool headerSkipped = false;

    // Maps for storing counts
    map<string, map<string, int>> dataMap;
    map<string, int> classSum;
    map<string, int> typeSum;

    // Read file
    while (getline(file, line)) {
        if (!headerSkipped) { headerSkipped = true; continue; } // skip header
        if (line.empty()) continue;

        stringstream ss(line);
        getline(ss, classType, ',');
        getline(ss, playType, ',');
        getline(ss, countStr, ',');
        count = stoi(countStr);

        dataMap[classType][playType] = count;
        classSum[classType] += count;
        typeSum[playType] += count;
    }
    file.close();

    // Grand total
    int grandTotal = 0;
    for (auto &p : typeSum) grandTotal += p.second;

    // Display table
    cout << fixed << setprecision(2);
    cout << "\n=============================================================\n";
    cout << "                CROSS TABULATION WITH WEIGHTS                \n";
    cout << "=============================================================\n\n";

    cout << left << setw(18) << "Class";
    for (auto &t : typeSum) {
        cout << setw(15) << (t.first + " (Cnt)")
             << setw(15) << "T-Weight%"
             << setw(15) << "D-Weight%";
    }
    cout << setw(15) << "Total"
         << setw(15) << "T-Weight%"
         << setw(15) << "D-Weight%" << endl;

    cout << string(18 + typeSum.size() * 36 + 36, '-') << endl;

    // Rows
    for (auto &c : classSum) {
        string cls = c.first;
        cout << left << setw(18) << cls;
        for (auto &t : typeSum) {
            int val = dataMap[cls][t.first];
            double tWeight = (double)val / classSum[cls] * 100;
            double dWeight = (double)val / typeSum[t.first] * 100;
            cout << setw(12) << val
                 << setw(12) << tWeight
                 << setw(12) << dWeight;
        }
        cout << setw(12) << classSum[cls]
             << setw(12) << "100.00"
             << setw(12) << ((double)classSum[cls] / grandTotal * 100) << endl;
    }

    cout << string(18 + typeSum.size() * 36 + 36, '-') << endl;

    // Total Row
    cout << left << setw(18) << "Total";
    for (auto &t : typeSum) {
        cout << setw(12) << t.second
             << setw(12) << ((double)t.second / grandTotal * 100)
             << setw(12) << "100.00";
    }
    cout << setw(12) << grandTotal
         << setw(12) << "100.00"
         << setw(12) << "100.00" << endl;

    cout << "\n=============================================================\n";
    cout << "✅ Computation Completed Successfully\n";
    cout << "=============================================================\n";

    return 0;
}
