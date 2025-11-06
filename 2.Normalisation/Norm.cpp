#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;

int main() {
    string filename = "input.csv";  // Input CSV file
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return 1;
    }

    vector<double> marks;
    string line, header;

    // Read header
    if (getline(file, header)) {
        cout << "Header: " << header << endl;
    }

    // Read data (mark values)
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        if (getline(ss, cell, ',')) {
            try {
                double value = stod(cell);
                marks.push_back(value);
            } catch (...) {
                continue;
            }
        }
    }

    file.close();

    if (marks.empty()) {
        cerr << "No numeric data found in file!" << endl;
        return 1;
    }

    double min_val = *min_element(marks.begin(), marks.end());
    double max_val = *max_element(marks.begin(), marks.end());
    double sum = 0.0;

    for (double m : marks) sum += m;
    double mean = sum / marks.size();

    double sd = 0.0;
    for (double m : marks) sd += pow(m - mean, 2);
    sd = sqrt(sd / marks.size());

    cout << fixed << setprecision(2);
    cout << "\nMin Value: " << min_val;
    cout << "\nMax Value: " << max_val;
    cout << "\nMean: " << mean;
    cout << "\nStandard Deviation: " << sd << endl;

    cout << "\n--------------------------------------------------" << endl;
    cout << "Mark\tMin-Max Norm\tZ-Score Norm" << endl;
    cout << "--------------------------------------------------" << endl;

    for (double m : marks) {
        double minmax = (max_val == min_val) ? 0.0 : (m - min_val) / (max_val - min_val);
        double zscore = (sd == 0) ? 0.0 : (m - mean) / sd;
        cout << m << "\t" << minmax << "\t\t" << zscore << endl;
    }

    return 0;
}
