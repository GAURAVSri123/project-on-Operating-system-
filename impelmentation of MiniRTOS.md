
# ⚙️ STEP-BY-STEP IMPLEMENTATION OF MiniRTOS

---

## 📁 Step 1: Create Project Directory

```bash
mkdir -p ~/MiniRTOS/src
cd ~/MiniRTOS
```

---

## 📄 Step 2: Create the C++ Source File

```bash
nano src/minirtos.cpp
```

Paste the following C++ code:

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <csignal>
#include <unistd.h>
#include <algorithm>

using namespace std;

// Task structure
struct Task {
    string name;
    int period;
    int deadline;
    int nextRelease;
    function<void()> func;
    int executions;
    int missedDeadlines;
};

vector<Task> tasks;
int currentTime = 0;

// Interrupt signal handler
void handle_interrupt(int sig) {
    cout << "[" << currentTime << " ms] INTERRUPT: SIGUSR1 received" << endl;
}

// Register task
void register_task(const string& name, int period, function<void()> func) {
    tasks.push_back(Task{name, period, period, 0, func, 0, 0});
}

// Sort tasks by earliest deadline first
void sort_tasks_by_deadline() {
    sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return a.deadline < b.deadline;
    });
}

// Task functions
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

// Print task statistics
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

// Main RTOS scheduler loop
void scheduler_loop() {
    while (true) {
        for (auto& task : tasks) {
            if (currentTime >= task.nextRelease) {
                if (currentTime > task.deadline)
                    task.missedDeadlines++;

                task.func();
                task.executions++;
                task.nextRelease += task.period;
                task.deadline = task.nextRelease;
            }
        }

        sort_tasks_by_deadline();
        usleep(100 * 1000);  // 100 ms
        currentTime += 100;

        if (currentTime % 2000 == 0) {
            print_task_stats();
        }
    }
}

int main() {
    signal(SIGUSR1, handle_interrupt);

    register_task("Sensor", 500, sensor_task);
    register_task("Logger", 1000, logging_task);
    register_task("Actuator", 1500, actuator_task);
    register_task("CPU Load", 1000, stress_task);

    cout << "MiniRTOS started. PID: " << getpid() << endl;
    scheduler_loop();
    return 0;
}
```

Save with `Ctrl+O`, then `Enter`, then exit with `Ctrl+X`.

---

## ⚙️ Step 3: Create Makefile

```bash
nano Makefile
```

Paste the following:

```make
# Makefile for MiniRTOS
CXX = g++
CXXFLAGS = -std=c++17 -Wall
SRC = src/minirtos.cpp
OUT = minirtos

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
```

Save and close.

---

## 🚀 Step 4: Build and Run the Program

### ✅ Build:

```bash
make
```

You will get an output file: `minirtos`

### ▶️ Run:

```bash
make run
```

Sample output:

```yaml
MiniRTOS started. PID: 12345
[0 ms] Task 1: Sensor Reading
[0 ms] Task 2: Logging
...
```

---

## 💥 Send Signal (Interrupt)

Open another terminal and run:

```bash
kill -SIGUSR1 <PID>
```

> Replace `<PID>` with the printed process ID from the program output.

---

## 🧹 Step 5: Clean Build

```bash
make clean
```
