//min_max and z_score normalization
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cmath>

using namespace std;

// Split a CSV line by comma
vector<string> split_line(const string &line, char delimiter = ',') {
    vector<string> tokens;
    string token;
    istringstream token_stream(line);
    while (getline(token_stream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Find minimum value
double find_min(const vector<double> &data) {
    double min_val = data[0];
    for (double val : data)
        if (val < min_val) min_val = val;
    return min_val;
}

// Find maximum value
double find_max(const vector<double> &data) {
    double max_val = data[0];
    for (double val : data)
        if (val > max_val) max_val = val;
    return max_val;
}

// Calculate mean
double calculate_mean(const vector<double> &data) {
    double sum = 0.0;
    for (double val : data) sum += val;
    return sum / data.size();
}

// Calculate standard deviation
double calculate_std_dev(const vector<double> &data, double mean) {
    double variance = 0.0;
    for (double val : data)
        variance += (val - mean) * (val - mean);
    variance /= data.size();
    return sqrt(variance);
}

// Main normalization function
void normalize_column(const string &input_csv, const string &output_csv, const string &column_name) {
    ifstream infile(input_csv);
    if (!infile.is_open()) {
        cout << "Error: Cannot open input file.\n";
        return;
    }

    ofstream outfile(output_csv);
    if (!outfile.is_open()) {
        cout << "Error: Cannot open output file.\n";
        return;
    }

    string line;
    vector<vector<string>> table;
    vector<double> values;
    size_t col_index = -1;
    bool header = true;

    while (getline(infile, line)) {
        vector<string> row = split_line(line);
        if (header) {
            // Find target column
            for (size_t i = 0; i < row.size(); ++i) {
                if (row[i] == column_name) {
                    col_index = i;
                    break;
                }
            }
            if (col_index == -1) {
                cout << "Error: Column not found.\n";
                return;
            }

            // Add new headers
            row.push_back(column_name + "_MinMax");
            row.push_back(column_name + "_ZScore");
            header = false;
        } else {
            values.push_back(stod(row[col_index]));
        }
        table.push_back(row);
    }

    double min_val = find_min(values);
    double max_val = find_max(values);
    double mean = calculate_mean(values);
    double std_dev = calculate_std_dev(values, mean);

    cout << fixed << setprecision(4);
    cout << "\n" << setw(10) << "Value"
         << setw(15) << "Min-Max"
         << setw(15) << "Z-Score" << "\n";
    cout << string(45, '-') << "\n";

    for (size_t i = 0; i < table.size(); ++i) {
        if (i == 0) {
            // Write header row
            for (auto &cell : table[i]) outfile << cell << ",";
            outfile << "\n";
        } else {
            double val = values[i - 1];
            double minmax = (val - min_val) / (max_val - min_val);
            double zscore = (val - mean) / std_dev;

            // Write to CSV
            for (auto &cell : table[i]) outfile << cell << ",";
            outfile << minmax << "," << zscore << "\n";

            // Print to console
            cout << setw(10) << val
                 << setw(15) << minmax
                 << setw(15) << zscore << "\n";
        }
    }

    infile.close();
    outfile.close();
    cout << "\nNormalized data saved to " << output_csv << endl;
}

// --------------------- MAIN ---------------------
int main() {
    string input_file = "input.csv";
    string output_file = "normalized_output.csv";
    string column_name;

    cout << "Enter column name to normalize: ";
    getline(cin, column_name);

    normalize_column(input_file, output_file, column_name);
    return 0;
}
