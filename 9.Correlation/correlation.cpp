#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <string>
#include <iomanip>
using namespace std;

int main(int argc, char* argv[]) {
    string input = (argc > 1) ? argv[1] : "data.csv";
    ifstream file(input);

    if (!file.is_open()) {
        cout << "Error: Cannot open file " << input << endl;
        return 1;
    }

    vector<double> studyHours, marks;
    string line;

    // Skip header
    getline(file, line);

    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string name, hourStr, markStr;

        getline(ss, name, ',');
        getline(ss, hourStr, ',');
        getline(ss, markStr, ',');

        if (!hourStr.empty() && !markStr.empty()) {
            studyHours.push_back(stod(hourStr));
            marks.push_back(stod(markStr));
        }
    }
    file.close();

    int n = min(studyHours.size(), marks.size());
    if (n == 0) {
        cout << "No valid data found.\n";
        return 0;
    }

    double meanX = 0, meanY = 0;
    for (int i = 0; i < n; i++) {
        meanX += studyHours[i];
        meanY += marks[i];
    }
    meanX /= n;
    meanY /= n;

    double numerator = 0, denomX = 0, denomY = 0;
    for (int i = 0; i < n; i++) {
        numerator += (studyHours[i] - meanX) * (marks[i] - meanY);
        denomX += pow(studyHours[i] - meanX, 2);
        denomY += pow(marks[i] - meanY, 2);
    }

    double r = numerator / sqrt(denomX * denomY);

    cout << fixed << setprecision(2);
    cout << "Mean of Study Hours = " << meanX << endl;
    cout << "Mean of Marks = " << meanY << endl;
    cout << setprecision(4);
    cout << "Pearson Correlation (r) = " << r << endl;

    if (r > 0.7)
        cout << "Strong positive correlation: More study hours → higher marks." << endl;
    else if (r > 0.3)
        cout << "Moderate positive correlation: Some relationship exists." << endl;
    else if (r > 0)
        cout << "Weak positive correlation." << endl;
    else if (r < -0.7)
        cout << "Strong negative correlation: More study hours → lower marks." << endl;
    else if (r < -0.3)
        cout << "Moderate negative correlation." << endl;
    else
        cout << "No significant correlation." << endl;

    return 0;
}
