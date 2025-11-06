
#include <bits/stdc++.h>
using namespace std;

int main()
{

    ifstream file("input.csv");

    string line, word;
    string Day, Level, Routine, PlayGame, Value;

    map<string, int> parent;
    map<string, map<string, int>> child;

    if (!file.is_open())
    {
        perror("Error in opening input file : ");
        return -1;
    }

    int i = 0;
    string childName;
    while (getline(file, line))
    {
        stringstream str(line);

        getline(str, Day, ',');
        getline(str, Level, ',');
        getline(str, Routine, ',');
        getline(str, PlayGame, ',');
        getline(str, Value, ',');

        int choice;

        if (i == 0)
        {
            i++;
            cout << "Enter Child Column Number : ";
            cin >> choice;
            continue;
        }

        switch (choice)
        {
        case 1:
            childName = Day;
            break;

        case 2:
            childName = Level;
            break;

        case 3:
            childName = Routine;
            break;

        case 4:
            childName = Value;
            break;

        default:
            childName = Routine;
            break;
        }

        parent[PlayGame]++;
        child[childName][PlayGame]++;
    }

    double pos = parent["Yes"], neg = parent["No"];
    double total = pos + neg;

    double parent_entropy = -((pos / total) * log2(pos / total) + (neg / total) * log2(neg / total));
    cout << "Parent Entropy: " << parent_entropy << "\n";

    double child_entropy = 0;
    for (auto p : child)
    {
        string val = p.first;
        double pR = child[val]["Yes"], nR = child[val]["No"];
        double tR = pR + nR;
        double pRv = 0;
        double nRv = 0;
        if (pR)
            pRv = (pR / tR) * log2(pR / tR);
        if (nR)
            nRv = (nR / tR) * log2(nR / tR);
        child_entropy += -((pR + nR) / total) * (pRv + nRv);
    }

    cout << "Child Entropy * Their proportion : " << child_entropy << "\n";

    cout << "Info gain : " << parent_entropy - child_entropy << "\n";

    return 0;
}