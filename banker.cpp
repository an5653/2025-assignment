#include <iostream>
#include <string>
#include <fstream>
#include <vector>


using namespace std;

void printWork(const vector<vector<int>>& work) {
    for (size_t i = 0; i < work.size(); ++i) {
        cout << "Work[" << i << "]: ";
        for (size_t j = 0; j < work[i].size(); ++j) {
            cout << work[i][j] << " ";
        }
        cout << endl;
    }
    cout << " ---------------- " << endl;
}

void printWork(const vector<int>& work) {
    cout << "Index: ";
    for (size_t i = 0; i < work.size(); ++i) {
        cout << work[i] << " ";
    }
    cout << endl;
    cout << " -----------------" << endl;
}

bool isSafety(vector<vector<int>> Allocation,
    vector<int> Available,
    const vector<vector<int>>& Max) {

    int numberOfCustomers = Allocation.size();
    int numberOfResources = Allocation[0].size();

    vector<bool> safe(numberOfCustomers, false);
    int safeCount = 0;

    bool progress = true;

    while (safeCount < numberOfCustomers && progress) {
        progress = false;
        for (int i = 0; i < numberOfCustomers; ++i) {
            if (!safe[i]) {
                bool canProceed = true;
                for (int j = 0; j < numberOfResources; ++j) {
                    if (Max[i][j] - Allocation[i][j] > Available[j]) {
                        canProceed = false;
                        break;
                    }
                }
                if (canProceed) {
                    for (int j = 0; j < numberOfResources; ++j) {
                        Available[j] += Allocation[i][j];
                    }
                    safe[i] = true;
                    safeCount++;
                    progress = true;
                }
            }
        }
    }

    return safeCount == numberOfCustomers;
}




int main() {
    int n, m;
    ifstream fin("banker.inp");
    ofstream fout("banker.out");

    fin >> n;
    fin >> m;

    vector<int> r_type(m);
    for (int i = 0; i < m; i++)
    {
        fin >> r_type[i];
    }

    vector<vector<int>> max(n, vector<int>(m));
    vector<vector<int>> allocation(n, vector<int>(m));


    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            fin >> max[i][j];
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            fin >> allocation[i][j];
        }
    }
    vector<vector<int>> need(n, vector<int>(m));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
    vector<int> use(m);
    for (int i = 0; i < m; i++)
    {
        use[i] = 0;
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            use[j] += allocation[i][j];
        }
    }

    vector<int> avail(m);
    for (int i = 0; i < m; i++)
    {
        avail[i] = r_type[i] - use[i];
    }

    vector<int> tmp_avail(m);
    string command;
    int index;
    int count = 1;
    vector<vector<int>> work;
    vector<int> arrindex;
    while (true) {
        fin >> command;
        if (command == "quit") break;

        fin >> index;
        arrindex.push_back(index);
        vector<int> w(m + 1);
        w[0] = (command == "request") ? 0 : 1;

        for (int i = 1; i <= m; i++) {
            fin >> w[i];
        }

        work.push_back(w);
    }
    int finish = 0;
    int fcount = 0;
    int infinite = 0;
    int sinfinite = 0;
    vector<vector<int>> tmp_allocation = allocation;
    vector<vector<int>> sleep;
    vector<int> s_index;
    int slcount = 0;
    int sfinite = 0;
    int snum = 0;
    int sfinish = 0;
    while (true)
    {
        fcount = 0;
        infinite = 0;
        for (int i = 0; i < work.size(); i++)
        {
            if (work[i][0] == 0)
            {
                index = arrindex[i];
                for (int j = 0; j < m; j++)
                {
                    tmp_avail[j] = avail[j];
                    tmp_avail[j] -= work[i][j + 1];
                    tmp_allocation[index][j] += work[i][j + 1];
                    if (tmp_avail[j] < 0) {
                        finish = 1;
                        cout << "리퀘판별" << endl;
                    }
                        
                        
                }
                if (finish == 1)
                {
                    finish = 0;
                    for (int j = 0; j < m; j++)
                    {
                        fout << avail[j] << " ";
                    }
                    fout << endl;
                    work[i][0] = 3;
                    sleep.push_back(work[i]);
                    s_index.push_back(index);
                    cout << "리퀘실패" << endl;
                }
                else
                {
                    if (isSafety(tmp_allocation, tmp_avail, max))
                    {
                        work[i][0] = 2;
                        avail = tmp_avail;
                        allocation = tmp_allocation;
                        for (int j = 0; j < m; j++)
                        {
                            fout << avail[j] << " ";
                        }
                        fout << endl;
                        cout << "세이프성공" << endl;
                    }
                    else
                    {
                        for (int j = 0; j < m; j++)
                        {
                            fout << avail[j] << " ";
                        }
                        fout << endl;
                        work[i][0] = 3;
                        sleep.push_back(work[i]);
                        s_index.push_back(index);
                        cout << "세이프실패" << endl;
                    }
                }
            }



            if (work[i][0] == 1)
            {
                index = arrindex[i];
                for (int j = 0; j < m; j++)
                {
                    avail[j] += work[i][j + 1];
                    allocation[index][j] -= work[i][j + 1];
                }
                work[i][0] = 2;
              
                printWork(sleep);
                while (true)
                {

                    int sinfinite = 0;
                    int num = sleep.size();
                    vector<vector<int>> sleep_allocation;
                    vector <int> sleep_avail;
                    sleep_allocation = allocation;
                    sleep_avail = avail;
                    for (int j = 0; j < num; j++)
                    {

                        if (sleep[j][0] == 3)
                        {
                            int tmp = s_index[j];
                            for (int k = 0; k < m; ++k) {

                                sleep_avail[k]  -= sleep[j][k + 1];
                                sleep_allocation[tmp][k] += sleep[j][k + 1];
                                if (sleep_avail[k] < 0)
                                    sfinish = 1;
                            }
                            if (sfinish != 1)
                            {

                                if (isSafety(sleep_allocation, sleep_avail, max))
                                {
                                    avail = sleep_avail;
                                    allocation = sleep_allocation;
                                    sleep[j][0] = 2;
                                    sinfinite = 1;

                                }
                            }

                        }
                        sfinish = 0;
                    }
                    if (sinfinite == 0) {
                       \
                        break;
                    }


                }

                for (int j = 0; j < m; j++)
                {
                    fout << avail[j] << " ";
                }
                fout << endl;

            }

            if (work[i][0] == 2)
            {
                fcount += 1;
            }
            if (work[i][0] == 3) {
                infinite += 1;
            }

        }

        if (fcount == work.size())
            break;

        else if (fcount + infinite == work.size())
            break;

    }

}