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
    int TaTime; // cTime - aTime
};

struct Output {
    vector<pCompletion> pInfo;
    float Throughput;
};

/**
 * Shortest Job First Algorithm
 * Within valid processes in window of arrival time, find the smallest burst time
 * "Complete the process" (non-preemptive), then add all the processes < new arrival time
 * Repeat with finding the smallest burst time again until no more processes
*/
Output SJF(vector<Process> processes) {
    Output output;
    int n = processes.size();

    priority_queue<Process, vector<Process>, Compare> pq;
    if (n < 1) {
        return output;
    }
    pq.push(processes[0]);
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
        while (pIdx < n && processes[pIdx].arrival_time <= curTick) {
            pq.push(processes[pIdx++]);
        }
    }

    output.Throughput = 1.0 * n / curTick;

    return output;
}

/**
 * First Come First Serve Algorithm
 * Sort by arrival time and output in that order
 * Already sorted bc of SJF implementation so just iterate
 * through vector
*/
Output FCFS(vector<Process> processes) {
    Output output;
    int curTick = 0, n = processes.size();

    for (auto p : processes) {
        curTick += p.burst_time;

        pCompletion tmp;
        tmp.pID = p.pID;
        tmp.cTime = curTick;
        tmp.TaTime = curTick - p.arrival_time;

        output.pInfo.push_back(tmp);
    }

    output.Throughput = 1.0 * n / curTick;

    return output;
}

// Displays output in table format
void displayOutput(Output output) {
    printf("Throughput: %.2f\n", output.Throughput);

    cout << "ProcessID | Completion Time | Turn Around Time\n";

    for (auto info : output.pInfo) {
        printf("%-9d | %-15d | %d\n", info.pID, info.cTime, info.TaTime);
    }
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

    // Merge arrival and execution times
    vector<Process> processes(n);
    for (int i = 0;i < n;i++) {
        processes[i] = {i + 1, at[i], et[i]};
    }

    // Sort by arrival time
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrival_time < b.arrival_time; 
    });

    // Output
    Output sjfOutput = SJF(processes), fcfsOutput = FCFS(processes);
    cout << "\n**Processes ordered by completion time**\n";
    cout << "\nSJF Algorithm\n";
    displayOutput(sjfOutput);

    cout << "\nFCFS Algorithm\n";
    displayOutput(fcfsOutput);

    return 0;
}


