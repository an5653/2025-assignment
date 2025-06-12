#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

class Process {
public:
    int index;
    int input_time;
    int cpu[1000] = { 0 };  
    int io[1000] = { 0 };  
    int waiting;
    int finish = -1;     

    int cpu_idx = 0;
    int io_idx = 0;
    int next_ready = 0;
    bool in_io = false;
    bool done = false;

    void setIndex(int i) { index = i; }
    void setItime(int i) { input_time = i; next_ready = i; }
    void setCpu(int i, int index) { cpu[index] = i; }
    void setIo(int i, int index) { io[index] = i; }
    void setWaiting(int i) { waiting = i; }
    void setFinish(int i) { finish = i; }
    int getIndex() { return index; }
    int getItime() { return input_time; }
    int getCpu(int i) { return cpu[i]; }
    int getIo(int i) { return io[i]; }
    int getWaiting() { return waiting; }
    int getFinish() { return finish; }
};

int main() {
    int n;
    ifstream fin("sjf.inp");
    ofstream fout("sjf.out");

    fin >> n;
    Process* parr = new Process[n];

    for (int i = 0; i < n; i++) {
        parr[i].setIndex(i);
        int t;
        fin >> t;
        parr[i].setItime(t);

        int idx = 0;
        while (true) {
            fin >> t;
            if (t == -1) break;
            if (idx % 2 == 0) parr[i].setCpu(t, idx / 2);
            else parr[i].setIo(t, idx / 2);
            idx++;
        }
    }

    int time = 0;
    int idle_time = 0;
    int finished = 0;

    while (finished < n) {
        vector<int> ready;
        for (int i = 0; i < n; i++) {
            if (!parr[i].done && !parr[i].in_io && parr[i].next_ready <= time) {
                if (parr[i].cpu_idx < 1000 && parr[i].cpu[parr[i].cpu_idx] > 0) {
                    ready.push_back(i);
                }
            }
        }

        if (ready.empty()) {
            int next_event = 1e9;
            bool has_event = false;

            for (int i = 0; i < n; i++) {
                if (!parr[i].done) {
                    if (parr[i].in_io) {
                        next_event = min(next_event, parr[i].next_ready);
                        has_event = true;
                    }
                    else if (parr[i].next_ready > time) {
                        next_event = min(next_event, parr[i].next_ready);
                        has_event = true;
                    }
                }
            }

            if (!has_event) break;

            idle_time += next_event - time;
            time = next_event;

            for (int i = 0; i < n; i++) {
                if (parr[i].in_io && parr[i].next_ready <= time) {
                    parr[i].in_io = false;
                }
            }
            continue;
        }

        sort(ready.begin(), ready.end(), [&](int a, int b) {
            int ca = parr[a].cpu[parr[a].cpu_idx];
            int cb = parr[b].cpu[parr[b].cpu_idx];
            if (ca != cb) return ca < cb;
            if (parr[a].next_ready != parr[b].next_ready)
                return parr[a].next_ready < parr[b].next_ready;
            return a < b;
            });

        int curr = ready[0];
        int cputime = parr[curr].cpu[parr[curr].cpu_idx];
        time += cputime;
        parr[curr].cpu_idx++;

        if (parr[curr].io_idx < 1000 && parr[curr].io[parr[curr].io_idx] > 0) {
            int iotime = parr[curr].io[parr[curr].io_idx];
            parr[curr].io_idx++;
            parr[curr].next_ready = time + iotime;
            parr[curr].in_io = true;
        }
        else {
            parr[curr].finish = time;
            parr[curr].done = true;
            finished++;
        }

        for (int i = 0; i < n; i++) {
            if (parr[i].in_io && parr[i].next_ready <= time) {
                parr[i].in_io = false;
            }
        }
    }

    fout << idle_time << "\n";
    for (int i = 0; i < n; i++) {
        if (parr[i].finish == -1) {  
            if (!parr[i].done && parr[i].cpu_idx < 1000 && parr[i].cpu[parr[i].cpu_idx] > 0) {
                time += parr[i].cpu[parr[i].cpu_idx];
                parr[i].finish = time;
            }
            else if (parr[i].finish == -1) {
                parr[i].finish = parr[i].next_ready;
            }
        }
        fout << parr[i].getFinish() << "\n";
    }

    fin.close();
    fout.close();
    delete[] parr;
    return 0;
}