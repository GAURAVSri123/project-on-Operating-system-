#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <csignal>
#include <unistd.h>
#include <algorithm>

using namespace std;

struct Task {
    string name;
    int period;             // in ms
    int deadline;           // absolute time
    int nextRelease;        // next scheduled time
    function<void()> func;  // function pointer
    int executions;
    int missedDeadlines;
};

vector<Task> tasks;
int currentTime = 0;

// Handle SIGUSR1 interrupts (simulated hardware interrupt)
void handle_interrupt(int sig) {
    cout << "[" << currentTime << " ms] INTERRUPT: SIGUSR1 received" << endl;
}

// Register a new task
void register_task(const string& name, int period, function<void()> func) {
    tasks.push_back(Task{name, period, period, 0, func, 0, 0});
}

// Sort tasks by deadline (Earliest Deadline First)
void sort_tasks_by_deadline() {
    sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return a.deadline < b.deadline;
    });
}

// Define tasks
void sensor_task() {
    cout << "[" << currentTime << " ms] Task 1: Sensor Reading" << endl;
}

void logging_task() {
    cout << "[" << currentTime << " ms] Task 2: Logging" << endl;
}

void actuator_task() {
    cout << "[" << currentTime << " ms] Task 3: Actuator Control" << endl;
}

void stress_task() {
    cout << "[" << currentTime << " ms] Task 4: CPU Stress Task" << endl;
    volatile long sum = 0;
    for (long i = 0; i < 100000; ++i) sum += i;
}

// Print task stats
void print_task_stats() {
    cout << "\n=== Task Statistics ===" << endl;
    cout << "Current time: " << currentTime << " ms" << endl;
    int idx = 1;
    for (const auto& task : tasks) {
        cout << "Task " << idx++ << ": " << task.name << endl;
        cout << "  Period: " << task.period << " ms" << endl;
        cout << "  Next release: " << task.nextRelease << " ms" << endl;
        cout << "  Deadline: " << task.deadline << " ms" << endl;
        cout << "  Executions: " << task.executions << endl;
        cout << "  Missed deadlines: " << task.missedDeadlines << "\n" << endl;
    }
}

// Main scheduler loop
void scheduler_loop() {
    while (true) {
        for (auto& task : tasks) {
            if (currentTime >= task.nextRelease) {
                if (currentTime > task.deadline)
                    task.missedDeadlines++;

                task.func();  // Run task
                task.executions++;
                task.nextRelease += task.period;
                task.deadline = task.nextRelease;
            }
        }

        sort_tasks_by_deadline();     // EDF sort
        usleep(100 * 1000);           // 100 ms sleep
        currentTime += 100;

        if (currentTime % 2000 == 0) {
            print_task_stats();
        }
    }
}

int main() {
    signal(SIGUSR1, handle_interrupt); // Register interrupt handler

    // Register tasks
    register_task("Sensor", 500, sensor_task);
    register_task("Logger", 1000, logging_task);
    register_task("Actuator", 1500, actuator_task);
    register_task("CPU Load", 1000, stress_task);

    cout << "MiniRTOS started. PID: " << getpid() << endl;
    scheduler_loop(); // Start the RTOS loop

    return 0;
}
