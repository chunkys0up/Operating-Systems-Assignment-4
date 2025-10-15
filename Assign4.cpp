#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Process {
    int pID;
    int arrival_time;
    int burst_time;
};

struct Compare {
    bool operator()(const Process& a, const Process& b) {
        return a.burst_time > b.burst_time; // min heap
    }
};

struct pCompletion {
    int pID;
    int cTime;
    int TaTime; // CT - AT
};

struct Output {
    vector<pCompletion> pInfo;
    float Throughput;
};


/**
 * Shortest Job First Algorithm
 * Within valid processes in window of arrival time, find the smallest burst time
 * "Complete the process", then add all the processes < new arrival time
 * Repeat with finding the smallest burst time again until no more processes
*/
Output SJF(vector<Process> p) {
    Output output;
    int n = p.size();

    priority_queue<Process, vector<Process>, Compare> pq;
    if (n < 1) {
        return output;
    }
    pq.push(p[0]);
    int curTick = 0;
    int pIdx = 1;

    while (!pq.empty()) {
        auto cur = pq.top();
        pq.pop();

        // calculate new curTick
        curTick += cur.burst_time;

        // add to output
        pCompletion tmp;
        tmp.pID = cur.pID;
        tmp.cTime = curTick;
        tmp.TaTime = curTick - cur.arrival_time;

        output.pInfo.push_back(tmp);

        // Now add the processes under curTick
        while (pIdx < n && p[pIdx].arrival_time <= curTick) {
            pq.push(p[pIdx++]);
        }
    }

    output.Throughput = 1.0 * n / curTick;

    return output;
}


int main() {
    cout << "Enter the amount of processes to use\n";
    int n;
    cin >> n;

    vector<int> at(n), et(n);

    cout << "Enter Arrival Times P1 -> PN\n";
    for (int i = 0;i < n;i++) {
        cin >> at[i];
    }

    cout << "Enter Execution Time P1 -> pN\n";
    for (int i = 0;i < n;i++) {
        cin >> et[i];
    }

    cout << "Input works\n";

    vector<Process> processes(n);
    for (int i = 0;i < n;i++) {
        processes[i] = {i + 1, at[i], et[i]};
    }

    cout << "processes combined\n";

    // Sort by arrival time
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrival_time < b.arrival_time; 
    });

    // Output
    cout << "SJF Algorithm\n";
    Output sjfOutput = SJF(processes);
    
    printf("Throughput: %.2f\n", sjfOutput.Throughput);

    cout << "ProcessID | Completion Time | Turn Around Time\n";

    for (auto info : sjfOutput.pInfo) {
        printf("%-9d | %-15d | %d\n", info.pID, info.cTime, info.TaTime);
    }


    // cout << "FCFS Algorithm" << endl;
}


