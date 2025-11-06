#include <bits/stdc++.h>
using namespace std;

struct Data {
    string fruit;
    string seeds;
    string weight;
    string ripe;
    string type;
};

vector<Data> parseCSV(const string &path) {
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Error: cannot open " << path << "\n";
        return {};
    }
    string line, token;
    vector<Data> rows;
    getline(file, line); // header
    while (getline(file, line)) {
        stringstream ss(line);
        Data d;
        getline(ss, token, ','); // No (skip)
        getline(ss, d.fruit, ',');
        getline(ss, d.seeds, ',');
        getline(ss, d.weight, ',');
        getline(ss, d.ripe, ',');
        getline(ss, d.type, ',');
        // Trim whitespace (if any)
        auto trim = [](string &s) {
            while (!s.empty() && isspace((unsigned char)s.back())) s.pop_back();
            while (!s.empty() && isspace((unsigned char)s.front())) s.erase(s.begin());
        };
        trim(d.fruit); trim(d.seeds); trim(d.weight); trim(d.ripe); trim(d.type);
        if (!d.type.empty()) rows.push_back(d);
    }
    return rows;
}

int main() {
    string fileName = "input.csv";
    vector<Data> data = parseCSV(fileName);
    if (data.empty()) {
        cout << "No data loaded. Ensure data.csv exists and has rows.\n";
        return 1;
    }

    // Gather class counts and feature-value sets
    map<string, int> classCount;
    map<string, map<string, int>> count_fruit, count_seeds, count_weight, count_ripe;
    set<string> setFruit, setSeeds, setWeight, setRipe;

    for (auto &r : data) {
        classCount[r.type]++;
        count_fruit[r.type][r.fruit]++;
        count_seeds[r.type][r.seeds]++;
        count_weight[r.type][r.weight]++;
        count_ripe[r.type][r.ripe]++;

        setFruit.insert(r.fruit);
        setSeeds.insert(r.seeds);
        setWeight.insert(r.weight);
        setRipe.insert(r.ripe);
    }

    int N = data.size();
    cout << fixed << setprecision(6);
    cout << "\n=== Dataset summary ===\n";
    cout << "Total records: " << N << "\n";
    for (auto &p : classCount) cout << "Class " << p.first << " count = " << p.second << "\n";

    // Read evidence from user
    string eFruit, eSeeds, eWeight, eRipe;
    cout << "\nEnter Fruit  : "; cin >> eFruit;
    cout << "Enter Seeds  : "; cin >> eSeeds;
    cout << "Enter Weight : "; cin >> eWeight;
    cout << "Enter Ripe   : "; cin >> eRipe;

    // Laplace smoothing parameter
    double alpha = 1.0;

    // For each class compute:
    // prior, conditional probs with Laplace smoothing, likelihood (prior * product cond), then normalized posterior
    struct ClassStats {
        string cls;
        double prior;
        double p_fruit;
        double p_seeds;
        double p_weight;
        double p_ripe;
        double joint;   // prior * product(cond)
        double posterior; // normalized
    };
    vector<ClassStats> stats;

    // list classes (ensure stable order)
    vector<string> classes;
    for (auto &p : classCount) classes.push_back(p.first);

    for (auto &cls : classes) {
        int C = classCount[cls];
        double prior = (double)C / N;

        // number of possible values for each feature
        int kFruit = (int)setFruit.size();
        int kSeeds = (int)setSeeds.size();
        int kWeight = (int)setWeight.size();
        int kRipe = (int)setRipe.size();

        double pf = (count_fruit[cls][eFruit] + alpha) / (C + alpha * kFruit);
        double ps = (count_seeds[cls][eSeeds] + alpha) / (C + alpha * kSeeds);
        double pw = (count_weight[cls][eWeight] + alpha) / (C + alpha * kWeight);
        double pr = (count_ripe[cls][eRipe] + alpha) / (C + alpha * kRipe);

        double joint = prior * pf * ps * pw * pr;

        stats.push_back({cls, prior, pf, ps, pw, pr, joint, 0.0});
    }

    // Normalize to get posterior probabilities
    double sumJoint = 0.0;
    for (auto &s : stats) sumJoint += s.joint;
    if (sumJoint == 0.0) {
        cout << "\nWarning: joint probabilities are all zero (check data/values). Exiting.\n";
        return 1;
    }
    for (auto &s : stats) s.posterior = s.joint / sumJoint;

    // Print neat table of intermediate probabilities
    cout << "\n\n=== Conditional probabilities (Laplace smoothing, alpha = " << alpha << ") ===\n\n";
    cout << left << setw(8) << "Class"
         << setw(12) << "Prior"
         << setw(16) << ("P(Fruit=" + eFruit + "|C)")
         << setw(18) << ("P(Seeds=" + eSeeds + "|C)")
         << setw(16) << ("P(Weight=" + eWeight + "|C)")
         << setw(14) << ("P(Ripe=" + eRipe + "|C)")
         << setw(14) << "Joint\n";
    cout << string(90, '-') << "\n";

    for (auto &s : stats) {
        cout << left << setw(8) << s.cls
             << setw(12) << s.prior
             << setw(16) << s.p_fruit
             << setw(18) << s.p_seeds
             << setw(16) << s.p_weight
             << setw(14) << s.p_ripe
             << setw(14) << s.joint << "\n";
    }

    // Print posteriors and final prediction
    cout << "\n\n=== Posterior probabilities ===\n";
    for (auto &s : stats) {
        cout << "P(" << s.cls << " | evidence) = " << s.posterior << "\n";
    }

    // predicted class
    auto best = max_element(stats.begin(), stats.end(), [](const ClassStats &a, const ClassStats &b){
        return a.posterior < b.posterior;
    });

    cout << "\n---------------------------------\n";
    cout << "Predicted class: " << best->cls << "  (Confidence: " << best->posterior * 100 << "%)\n";
    cout << "---------------------------------\n\n";

    return 0;
}
