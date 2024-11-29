CPU Scheduling Algorithms Web Application

Overview

This project implements a web-based application to demonstrate various CPU scheduling algorithms. It supports the following scheduling algorithms:


First-Come, First-Served (FCFS)


Shortest Job First (SJF)


Priority Scheduling


Round Robin (RR)

The frontend allows users to input process details and select a scheduling algorithm. The backend processes the data and returns the scheduling results.

Prerequisites


A C++ compiler (e.g., g++)


Crow framework


ASIO library (standalone)


nlohmann/json library

Usage
Enter the number of processes.
Select a scheduling algorithm from the dropdown menu.
Enter the arrival time, burst time, and priority (for Priority Scheduling) for each process.
For Round Robin, enter the quantum time.
Click "Submit" to send the data to the server.
The scheduling results will be displayed below the form.
