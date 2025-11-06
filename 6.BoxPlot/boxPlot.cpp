#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>

using namespace std;

// Function to read the 'Marks' column data from a CSV file
vector<double> readMarksFromCSV(const string& filePath) {
    vector<double> data;
    ifstream file(filePath);
    if (!file.is_open()) {
        throw runtime_error("Error: Could not open the file " + filePath);
    }

    string line;
    getline(file, line); // Skip header line (assumed "Marks")

    while (getline(file, line)) {
        try {
            double value = stod(line);
            data.push_back(value);
        } catch (...) {
            // Skip invalid lines
        }
    }

    file.close();
    return data;
}

// Function to calculate Q1, Q2, Q3
vector<double> calculateBoxPlotStatistics(vector<double>& sortedData) {
    int n = sortedData.size();
    double Q1, Q2, Q3;

    if (n % 2 == 0) {
        Q2 = (sortedData[n / 2 - 1] + sortedData[n / 2]) / 2.0;
        vector<double> lowerHalf(sortedData.begin(), sortedData.begin() + n / 2);
        vector<double> upperHalf(sortedData.begin() + n / 2, sortedData.end());

        Q1 = (lowerHalf.size() % 2 == 0)
            ? (lowerHalf[lowerHalf.size() / 2 - 1] + lowerHalf[lowerHalf.size() / 2]) / 2.0
            : lowerHalf[lowerHalf.size() / 2];

        Q3 = (upperHalf.size() % 2 == 0)
            ? (upperHalf[upperHalf.size() / 2 - 1] + upperHalf[upperHalf.size() / 2]) / 2.0
            : upperHalf[upperHalf.size() / 2];

    } else {
        Q2 = sortedData[n / 2];
        vector<double> lowerHalf(sortedData.begin(), sortedData.begin() + n / 2);
        vector<double> upperHalf(sortedData.begin() + n / 2 + 1, sortedData.end());

        Q1 = (lowerHalf.size() % 2 == 0)
            ? (lowerHalf[lowerHalf.size() / 2 - 1] + lowerHalf[lowerHalf.size() / 2]) / 2.0
            : lowerHalf[lowerHalf.size() / 2];

        Q3 = (upperHalf.size() % 2 == 0)
            ? (upperHalf[upperHalf.size() / 2 - 1] + upperHalf[upperHalf.size() / 2]) / 2.0
            : upperHalf[upperHalf.size() / 2];
    }

    return { Q1, Q2, Q3 };
}

int main() {
    string filePath = "input.csv";  // Fixed input file name

    try {
        // Read the data
        vector<double> data = readMarksFromCSV(filePath);

        if (data.empty()) {
            cout << "No valid numeric data found in input.csv." << endl;
            return 0;
        }

        // Sort data
        sort(data.begin(), data.end());

        // Calculate boxplot statistics
        vector<double> boxStats = calculateBoxPlotStatistics(data);
        double Q1 = boxStats[0];
        double Q2 = boxStats[1];
        double Q3 = boxStats[2];
        double IQR = Q3 - Q1;

        double lowerWhisker = Q1 - 1.5 * IQR;
        double upperWhisker = Q3 + 1.5 * IQR;

        // Find outliers
        vector<double> outliers;
        for (double val : data) {
            if (val < lowerWhisker || val > upperWhisker)
                outliers.push_back(val);
        }

        double minimum = *min_element(data.begin(), data.end());
        double maximum = *max_element(data.begin(), data.end());

        cout << fixed << setprecision(2);
        cout << "===== Boxplot Statistics =====" << endl;
        cout << "Minimum: " << minimum << endl;
        cout << "Q1: " << Q1 << endl;
        cout << "Median (Q2): " << Q2 << endl;
        cout << "Q3: " << Q3 << endl;
        cout << "IQR: " << IQR << endl;
        cout << "Lower Whisker: " << lowerWhisker << endl;
        cout << "Upper Whisker: " << upperWhisker << endl;
        cout << "Maximum: " << maximum << endl;

        if (outliers.empty()) {
            cout << "Outliers: None" << endl;
        } else {
            cout << "Outliers: ";
            for (double out : outliers) cout << out << " ";
            cout << endl;
        }

    } catch (const exception& e) {
        cerr << e.what() << endl;
    }

    return 0;
}
