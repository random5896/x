#include <bits/stdc++.h>
using namespace std;

int main()
{

    ifstream file("input2.csv");

    string line, word;
    string Outlook, Temperature, Humidity, Windy, Class;

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

        getline(str, Outlook, ',');
        getline(str, Temperature, ',');
        getline(str, Humidity, ',');
        getline(str, Windy, ',');
        getline(str, Class, ',');
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
            childName = Outlook;
            break;

        case 2:
            childName = Temperature;
            break;

        case 3:
            childName = Humidity;
            break;

        case 4:
            childName = Windy;
            break;

        default:
            childName = Class;
            break;
        }
        parent[Class]++;
        child[childName][Class]++;
    }

    double pos = parent["Play"], neg = parent["NoPlay"];
    double total = pos + neg;
   

    double parent_entropy = -((pos / total) * log10(pos / total) + (neg / total) * log10(neg / total));
    cout << "Parent Entropy: " << parent_entropy << "\n";

    double child_entropy = 0;
    for (auto p : child)
    {
        string val = p.first;
        double pR = child[val]["Play"], nR = child[val]["NoPlay"];
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