#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <ctime>

struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int start_time = -1;
    int finish_time = -1;
    int waiting_time = 0; 
};


std::vector<Process> generateProcesses(int num_processes) {
    std::vector<Process> processes;
    srand(time(0));
    for (int i = 0; i < num_processes; ++i) {
        processes.push_back({
            i + 1,
            rand() % 10,        
            rand() % 10 + 1,   
            rand() % 5 + 1,    
            0                  
            });
        processes.back().remaining_time = processes.back().burst_time;
    }
    return processes;
}


void sjf(std::vector<Process> processes) {
    std::cout << "\nShortest Job First (SJF) Simulation\n";
    int time = 0;
    std::vector<Process*> ready_queue;
    std::vector<Process*> completed;

    while (completed.size() < processes.size()) {
        for (auto& process : processes) {
            if (process.arrival_time <= time && process.remaining_time > 0 &&
                std::find(ready_queue.begin(), ready_queue.end(), &process) == ready_queue.end()) {
                ready_queue.push_back(&process);
            }
        }

        if (!ready_queue.empty()) {
            std::sort(ready_queue.begin(), ready_queue.end(), [](Process* a, Process* b) {
                return a->remaining_time < b->remaining_time;
                });

            Process* current = ready_queue.front();
            ready_queue.erase(ready_queue.begin());

            current->start_time = (current->start_time == -1) ? time : current->start_time;
            time += current->remaining_time;
            current->finish_time = time;
            current->remaining_time = 0;
            current->waiting_time = current->finish_time - current->arrival_time - current->burst_time;
            completed.push_back(current);

            std::cout << "Process ID: " << current->id
                << ", Start Time: " << current->start_time
                << ", Finish Time: " << current->finish_time
                << ", Waiting Time: " << current->waiting_time << "\n";
        }
        else {
            ++time;
        }
    }
}

void prioritySchedulingWithAging(std::vector<Process> processes) {
    std::cout << "\nPriority Scheduling with Aging Simulation\n";
    int time = 0;
    std::vector<Process*> ready_queue;
    const int aging_threshold = 5;

    while (true) {
        for (auto& process : processes) {
            if (process.arrival_time <= time && process.remaining_time > 0 &&
                std::find(ready_queue.begin(), ready_queue.end(), &process) == ready_queue.end()) {
                ready_queue.push_back(&process);
            }
        }

        if (ready_queue.empty()) {
            if (std::all_of(processes.begin(), processes.end(),
                [](const Process& p) { return p.remaining_time == 0; })) {
                break;
            }
            ++time;
            continue;
        }

        for (auto& process : ready_queue) {
            if (time - process->arrival_time >= aging_threshold) {
                process->priority = std::max(1, process->priority - 1); 
            }
        }

        std::sort(ready_queue.begin(), ready_queue.end(), [](Process* a, Process* b) {
            return a->priority < b->priority;
            });

        Process* current = ready_queue.front();
        ready_queue.erase(ready_queue.begin());

        current->start_time = (current->start_time == -1) ? time : current->start_time;
        time += current->burst_time;
        current->finish_time = time;
        current->remaining_time = 0;

        std::cout << "Process ID: " << current->id
            << ", Priority: " << current->priority
            << ", Start Time: " << current->start_time
            << ", Finish Time: " << current->finish_time
            << "\n";
    }
}


void analyzePerformance(const std::vector<Process>& processes) {
    double total_waiting_time = 0;
    double total_turnaround_time = 0;

    std::cout << "\nPerformance Analysis\n";
    std::cout << "Process ID | Waiting Time | Turnaround Time\n";
    for (const auto& process : processes) {
        int turnaround_time = process.finish_time - process.arrival_time;
        total_waiting_time += process.waiting_time;
        total_turnaround_time += turnaround_time;

        std::cout << std::setw(10) << process.id << " | "
            << std::setw(12) << process.waiting_time << " | "
            << std::setw(15) << turnaround_time << "\n";
    }

    std::cout << "Average Waiting Time: " << total_waiting_time / processes.size() << "\n";
    std::cout << "Average Turnaround Time: " << total_turnaround_time / processes.size() << "\n";
}


int main() {
    int num_processes = 5;
    int quantum = 3;

    std::vector<Process> processes = generateProcesses(num_processes);

    std::cout << "Generated Processes:\n";
    for (const auto& process : processes) {
        std::cout << "ID: " << process.id
            << ", Arrival Time: " << process.arrival_time
            << ", Burst Time: " << process.burst_time
            << ", Priority: " << process.priority << "\n";
    }

    sjf(processes);
    prioritySchedulingWithAging(processes);
    analyzePerformance(processes);

    return 0;
}
