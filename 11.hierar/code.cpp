#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <limits>

using namespace std;

double singleLinkage(const vector<double>& c1, const vector<double>& c2);
double completeLinkage(const vector<double>& c1, const vector<double>& c2);
double averageLinkage(const vector<double>& c1, const vector<double>& c2);

int main() {
    string file = "data.csv";
    string outputFile = "output.csv";
    vector<double> marks;
    int choice;

    cout << "Choose Linkage Method:" << endl;
    cout << "1. Single Linkage" << endl;
    cout << "2. Complete Linkage" << endl;
    cout << "3. Average Linkage" << endl;
    cin >> choice;

    ifstream infile(file);
    if (!infile.is_open()) {
        cerr << "Unable to open file" << endl;
        return 1;
    }

    ofstream outfile(outputFile);
    if (!outfile.is_open()) {
        cerr << "Unable to open output file" << endl;
        return 1;
    }

    string line;
    getline(infile, line); // To skip the header if there is one
    while (getline(infile, line)) {
        string::size_type sz;
        marks.push_back(stod(line, &sz));
    }
    infile.close();

    vector<vector<double>> clusters;
    for (double v : marks) {
        clusters.push_back({v});
    }

    // Write header to output CSV file
    outfile << "Step,Clusters,Merged Cluster\n";
    int step = 1;

    // Agglomerative Hierarchical Clustering process
    while (clusters.size() > 1) {
        double minDist = numeric_limits<double>::max();
        int idx1 = -1, idx2 = -1;

        for (size_t i = 0; i < clusters.size(); ++i) {
            for (size_t j = i + 1; j < clusters.size(); ++j) {
                double dist = 0;

                switch (choice) {
                    case 1:
                        dist = singleLinkage(clusters[i], clusters[j]);
                        break;
                    case 2:
                        dist = completeLinkage(clusters[i], clusters[j]);
                        break;
                    case 3:
                        dist = averageLinkage(clusters[i], clusters[j]);
                        break;
                    default:
                        cout << "Invalid choice. Exiting." << endl;
                        return 1;
                }

                if (dist < minDist) {
                    minDist = dist;
                    idx1 = i;
                    idx2 = j;
                }
            }
        }

        // Merging clusters
        clusters[idx1].insert(clusters[idx1].end(), clusters[idx2].begin(), clusters[idx2].end());
        clusters.erase(clusters.begin() + idx2);

        // Output the current clusters and the merged cluster to the console and CSV file
        cout << "Step " << step << ": Merging clusters with minimum distance: " << minDist << endl;
        outfile << step << ",";

        // Write all clusters to CSV
        for (const auto& cluster : clusters) {
            outfile << "{";
            for (double v : cluster) {
                outfile << v << " ";
            }
            outfile << "} ";
        }

        outfile << ",Merged {";
        for (double v : clusters[idx1]) {
            outfile << v << " ";
        }
        outfile << "}\n";

        step++;
    }

    // Final Cluster Output
    cout << "Final Cluster: " << endl;
    outfile << "Final,";
    outfile << "{";
    for (double v : clusters[0]) {
        cout << v << " ";
        outfile << v << " ";
    }
    cout << endl;
    outfile << "}\n";

    outfile.close();
    cout << "Results saved to " << outputFile << endl;

    return 0;
}

double singleLinkage(const vector<double>& c1, const vector<double>& c2) {
    double minDist = numeric_limits<double>::max();
    for (double v1 : c1) {
        for (double v2 : c2) {
            double dist = fabs(v1 - v2);
            if (dist < minDist) {
                minDist = dist;
            }
        }
    }
    return minDist;
}

double completeLinkage(const vector<double>& c1, const vector<double>& c2) {
    double maxDist = numeric_limits<double>::min();
    for (double v1 : c1) {
        for (double v2 : c2) {
            double dist = fabs(v1 - v2);
            if (dist > maxDist) {
                maxDist = dist;
            }
        }
    }
    return maxDist;
}

double averageLinkage(const vector<double>& c1, const vector<double>& c2) {
    double totalDist = 0.0;
    int count = 0;
    for (double v1 : c1) {
        for (double v2 : c2) {
            totalDist += fabs(v1 - v2);
            count++;
        }
    }
    return totalDist / count;
}
