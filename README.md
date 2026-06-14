# Locality-Aware Task Scheduler & Multi-Level Cache Simulator

A high-performance C++ simulation of an operating system task scheduler paired with a multi-level memory hierarchy. The project uses a data-locality-aware **Shortest Job First (SJF)** heuristic to group tasks sharing memory requirements, minimizing total execution latency across cascading **Least Recently Used (LRU)** cache tiers.

## 🏗️ System Architecture

The simulation models a modern hardware memory pipeline from the processor core down to system RAM:

*   **L1 Cache:** 32 Blocks | 4 Cycle Latency
*   **L2 Cache:** 128 Blocks | 12 Cycle Latency
*   **L3 Cache:** 512 Blocks | 40 Cycle Latency
*   **RAM:** Infinite Capacity | 200 Cycle Latency

## 🚀 Key Features

*   **Locality-Driven SJF Scheduling:** Groups incoming tasks based on shared memory blocks to maximize cache reuse, sorting clusters internally by shortest CPU burst time.
*   **Custom LRU Implementation:** Built entirely using custom Doubly Linked Lists (`doublelinkedlist`) for $O(1)$ eviction and re-indexing operations without relying on standard library sequence containers.
*   **Cascading Eviction Pipeline:** Automatically handles capacity overflows by cascading evicted data downward through the memory tiers (`L1 -> L2 -> L3 -> Evicted`).
*   **Performance Analytics:** Tracks total processor execution cycles, exact cache penalty cycles, and total RAM hits.

## 📊 Memory Penalty Model

Total latency is calculated dynamically based on how deeply a memory address request misses through the hierarchy:

| Condition | Cumulative Cycle Cost Formula |
| :--- | :--- |
| **L1 Hit** | $4 \text{ cycles}$ |
| **L2 Hit** | $4 + 12 = 16 \text{ cycles}$ |
| **L3 Hit** | $4 + 12 + 40 = 56 \text{ cycles}$ |
| **RAM Hit** | $4 + 12 + 40 + 200 = 256 \text{ cycles}$ |

Additionally, an execution penalty is assessed for mismatch overhead:
$$\text{Overhead Penalty} = |\text{Burst Time} - \text{Memory Blocks Requested}|$$

## 🛠️ Getting Started

### Prerequisites
*   A C++11 compliant compiler (GCC, Clang, or MSVC)

### Input File Format (`tasks.txt`)
The application expects a `tasks.txt` file in the execution directory formatted as follows:
```text
TASK task_alpha BURST 15 MEM block_A block_B block_C
TASK task_beta  BURST 8  MEM block_B block_D
Compilation & Execution
Compile the project via terminal:

Bash
g++ -std=c++11 main.cpp -o cache_simulator && ./cache_simulator
📈 Example Output
Plaintext
Current Queue:[task_beta task_alpha ]
running task_beta Burst Time:8
Memory Block 1:block_B
Cache :L1 Cache:[] ...
L1MISS->L2 MISS->L3 MISS->RAM 

RESULT
CYCLES: 524
RAM Hits: 2
