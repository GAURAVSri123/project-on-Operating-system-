# project-on-Operating-system-

# MiniRTOS: A Lightweight Real-Time Scheduler for IoT Devices on Linux 


 
## What Is MiniRTOS?
MiniRTOS is a lightweight simulation of a Real-Time Operating System (RTOS) made for educational or IoT development purposes. It runs entirely in Linux userspace, meaning you don’t need real embedded hardware to understand how an RTOS works.
It mimics core RTOS features like: 

1.Task scheduling  

2.Deadline management  

3.Interrupt handling

## Why Build MiniRTOS?

IoT devices (like sensors and microcontrollers) often need to:

(i) Run tasks periodically.  

(ii) Respond quickly to events.  

(iii) Use low system resources.  

MiniRTOS is designed to simulate these behaviors, making it perfect for learning how real embedded RTOSes (like FreeRTOS) work.

### Key Components
Here are the core building blocks of MiniRTOS:

1. Task Structure
Each task (e.g., reading a sensor) is defined with:

(i) A name  

(ii) A period (how often it runs)  

(iii) A deadline  

(iv) A function pointer to the task code  


3. Earliest Deadline First (EDF) Scheduler
   Tasks are executed based on their next deadline The one with the closest (earliest) deadline runs first After running, a task’s next deadline is updated
This is a popular real-time scheduling strategy.

5. Timer/Tick Simulation
   
   Every 100ms, the system checks which task is due This simulates a hardware "tick" in real RTOS

7. Interrupt Simulation with Unix Signals
   
   RTOS systems often respond to hardware interrupts MiniRTOS uses SIGUSR1 to simulate this
If you send a signal from another terminal, it logs an “interrupt” occurred

## what is the major difference between RTOS and general puprose OS -
RTOS (Real-Time Operating System):
Focuses on predictable, timely execution of tasks. It guarantees that high-priority tasks are completed within a specific time limit (real-time deadlines).

General OS (like Windows/Linux):
Focuses on overall performance and multitasking, but does not guarantee when a specific task will finish. It may delay time-sensitive tasks due to background activity, UI management, etc.

🔁 In Simple Terms:
RTOS = "Always finish important tasks on time."  

OS = "Finish tasks eventually, aiming for efficiency and fairness."








