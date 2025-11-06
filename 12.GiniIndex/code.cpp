#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <cmath>

using namespace std;

// Function to read dataset from CSV file
vector<vector<string>> readCSV(const string& filePath) {
    vector<vector<string>> dataset;
    ifstream file(filePath);
    string line;
    while (getline(file, line)) {
        vector<string> row;
        stringstream ss(line);
        string value;
        while (getline(ss, value, ',')) {
            row.push_back(value);
        }
        dataset.push_back(row);
    }
    return dataset;
}

// Function to calculate entropy of a target column
double calculateEntropy(const vector<vector<string>>& dataset, int columnIndex) {
    unordered_map<string, int> frequencyMap;
    int totalRows = dataset.size() - 1;  // excluding the header

    // Calculate frequency of each class in the target column
    for (int i = 1; i < dataset.size(); i++) {
        frequencyMap[dataset[i][columnIndex]]++;
    }

    // Calculate entropy
    double entropy = 0.0;
    for (const auto& pair : frequencyMap) {
        double probability = static_cast<double>(pair.second) / totalRows;
        entropy -= probability * log2(probability);
    }
    return entropy;
}

// Function to calculate information gain for a given attribute
double calculateInformationGain(const vector<vector<string>>& dataset, int attributeIndex, double totalEntropy) {
    unordered_map<string, int> subsetFrequencyMap;
    unordered_map<string, unordered_map<string, int>> subsetClassMap;
    int totalRows = dataset.size() - 1;

    // Calculate frequencies and class counts for each attribute value
    for (int i = 1; i < dataset.size(); i++) {
        string attributeValue = dataset[i][attributeIndex];
        string classLabel = dataset[i].back(); // target class

        subsetFrequencyMap[attributeValue]++;
        subsetClassMap[attributeValue][classLabel]++;
    }

    // Calculate the entropy for each subset and sum them up
    double weightedEntropy = 0.0;
    for (const auto& pair : subsetFrequencyMap) {
        double subsetEntropy = 0.0;
        double subsetSize = pair.second;

        for (const auto& classPair : subsetClassMap[pair.first]) {
            double probability = static_cast<double>(classPair.second) / subsetSize;
            subsetEntropy -= probability * log2(probability);
        }
        weightedEntropy += (subsetSize / totalRows) * subsetEntropy;
    }

    return totalEntropy - weightedEntropy;
}

// Function to calculate the Gini index for a given attribute
double calculateGiniIndex(const vector<vector<string>>& dataset, int attributeIndex) {
    unordered_map<string, int> subsetFrequencyMap;
    unordered_map<string, unordered_map<string, int>> subsetClassMap;
    int totalRows = dataset.size() - 1;

    // Calculate frequencies and class counts for each attribute value
    for (int i = 1; i < dataset.size(); i++) {
        string attributeValue = dataset[i][attributeIndex];
        string classLabel = dataset[i].back(); // target class

        subsetFrequencyMap[attributeValue]++;
        subsetClassMap[attributeValue][classLabel]++;
    }

    // Calculate the Gini index for each subset and sum them up
    double weightedGini = 0.0;
    for (const auto& pair : subsetFrequencyMap) {
        double subsetGini = 1.0;
        double subsetSize = pair.second;

        for (const auto& classPair : subsetClassMap[pair.first]) {
            double probability = static_cast<double>(classPair.second) / subsetSize;
            subsetGini -= probability * probability;
        }
        weightedGini += (subsetSize / totalRows) * subsetGini;
    }

    return weightedGini;
}

int main() {
    string filePath = "data.csv";
    string outputFilePath = "output.csv";
    
    vector<vector<string>> dataset = readCSV(filePath);

    if (dataset.empty()) {
        cout << "Error reading the CSV file." << endl;
        return 1;
    }

    // Open output CSV file
    ofstream outputFile(outputFilePath);
    if (!outputFile.is_open()) {
        cout << "Error opening the output file." << endl;
        return 1;
    }

    // Write headers to the output CSV file
    outputFile << "Attribute,Information Gain,Gini Index\n";

    // Calculate the entropy of the target attribute (last column)
    double totalEntropy = calculateEntropy(dataset, dataset[0].size() - 1);
    cout << "Total Entropy: " << totalEntropy << endl;

    // Calculate information gain and Gini index for each attribute and write to CSV
    for (int i = 0; i < dataset[0].size() - 1; i++) {
        double infoGain = calculateInformationGain(dataset, i, totalEntropy);
        double giniIndex = calculateGiniIndex(dataset, i);
        cout << "Attribute: " << dataset[0][i] << endl;
        cout << "  Information Gain: " << infoGain << endl;
        cout << "  Gini Index: " << giniIndex << endl;

        // Write the results to the CSV file
        outputFile << dataset[0][i] << "," << infoGain << "," << giniIndex << "\n";
    }

    outputFile.close();
    cout << "Results saved to " << outputFilePath << endl;

    return 0;
}
