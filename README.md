# ✈️ SkyNet Radar: Air Traffic Control System (No STL)

## 📌 Project Overview
SkyNet Radar is a high-performance simulation of an Air Traffic Control system. 
**Key Technical Achievement:** This entire system was built **from scratch using C++**, without relying on the Standard Template Library (STL). Every data structure is manually implemented using raw pointers and dynamic memory management.

## 🚀 Key Features & Data Structures

### 1. Custom Hash Table (Collision Handling)
- **Purpose:** Instant lookup of aircraft data by Flight ID.
- **Implementation:** Manual **Chaining** technique using Linked Lists.
- **Why?** To handle hash collisions efficiently without data loss, mimicking `std::unordered_map` logic from scratch.

### 2. AVL Tree (Self-Balancing)
- **Purpose:** Maintaining an ordered log of flight records.
- **Implementation:** Custom rotations (LL, RR, LR, RL) to keep tree height balanced.

### 3. Graphs & Dijkstra's Algorithm
- **Purpose:** Calculating the shortest/safest path between airports.
- **Implementation:** Adjacency matrix/list with a custom priority queue.

### 4. Min-Heap (Priority Queue)
- **Purpose:** Managing emergency landing requests.
- **Logic:** Ensures the flight with the highest emergency status (lowest integer value) is processed first.



