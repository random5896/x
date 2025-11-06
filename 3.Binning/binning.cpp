#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <bits/stdc++.h>
using namespace std;

vector<int> read_csv(const string &file_name)
{
    vector<int> data;
    ifstream file(file_name);
    string line;

    if (file.is_open())
    {
        getline(file, line); // skip header (e.g., "mark")
        while (getline(file, line))
        {
            if (!line.empty())
                data.push_back(stoi(line));
        }
        file.close();
    }
    else
    {
        cerr << "Error: Could not open the file." << endl;
    }

    return data;
}

vector<int> binning_by_median(vector<int> data, int bin_size)
{
    if (bin_size == 0)
        return data;
    if (bin_size > data.size())
        bin_size = data.size();

    sort(data.begin(), data.end());
    vector<int> binned_data;

    for (size_t i = 0; i < data.size(); i += bin_size)
    {
        vector<int> bin_data(data.begin() + i, data.begin() + min(i + bin_size, data.size()));
        int median = bin_data[bin_data.size() / 2];
        binned_data.insert(binned_data.end(), bin_data.size(), median);
    }

    return binned_data;
}

vector<double> binning_by_mean(vector<int> data, int bin_size)
{
    if (bin_size == 0)
        return vector<double>(data.begin(), data.end());
    if (bin_size > data.size())
        bin_size = data.size();

    sort(data.begin(), data.end());
    vector<double> binned_data;

    for (size_t i = 0; i < data.size(); i += bin_size)
    {
        vector<int> bin_data(data.begin() + i, data.begin() + min(i + bin_size, data.size()));
        double mean = accumulate(bin_data.begin(), bin_data.end(), 0.0) / bin_data.size();
        binned_data.insert(binned_data.end(), bin_data.size(), round(mean * 100) / 100);
    }

    return binned_data;
}

vector<int> binning_by_boundaries(vector<int> data, int bin_size)
{
    if (bin_size == 0)
        return data;
    if (bin_size > data.size())
        bin_size = data.size();

    sort(data.begin(), data.end());
    vector<int> binned_data;

    for (size_t i = 0; i < data.size(); i += bin_size)
    {
        vector<int> bin_data(data.begin() + i, data.begin() + min(i + bin_size, data.size()));
        int min_val = bin_data.front();
        int max_val = bin_data.back();
        for (const int &value : bin_data)
        {
            if (abs(value - min_val) <= abs(value - max_val))
                binned_data.push_back(min_val);
            else
                binned_data.push_back(max_val);
        }
    }

    return binned_data;
}

int main()
{
    string file_name = "input.csv";
    int bin_size;

    cout << "Enter the number of bins: ";
    cin >> bin_size;

    vector<int> data = read_csv(file_name);

    if (data.empty())
    {
        cerr << "Error: No data found or file could not be opened." << endl;
        return 1;
    }

    vector<int> median_binned = binning_by_median(data, bin_size);
    vector<double> mean_binned = binning_by_mean(data, bin_size);
    vector<int> boundaries_binned = binning_by_boundaries(data, bin_size);

    cout << "\n=============================================\n";
    cout << "           BINNING RESULTS (TERMINAL OUTPUT)\n";
    cout << "=============================================\n";
    cout << left << setw(12) << "Original"
         << setw(15) << "Median Bin"
         << setw(15) << "Mean Bin"
         << "Boundary Bin\n";
    cout << string(60, '-') << endl;

    for (size_t i = 0; i < data.size(); ++i)
    {
        cout << left << setw(12) << data[i]
             << setw(15) << median_binned[i]
             << setw(15) << mean_binned[i]
             << boundaries_binned[i] << endl;
    }

    cout << "=============================================\n";
    cout << "✅ Process Completed Successfully!\n";
    cout << "=============================================\n";

    return 0;
}
