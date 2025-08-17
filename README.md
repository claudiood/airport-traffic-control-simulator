# Air Traffic Control Simulation ✈️

## Project Description
This project is a simulation, developed in **C with PThreads**, of an **air traffic control system** at a busy international airport.  

The airport receives and sends **national** and **international** flights, each following different rules for resource acquisition (runways, gates, and control tower). The simulation manages **landings, disembarkments, and takeoffs**, taking into account the priority of international flights and possible **deadlocks** due to the different resource allocation orders.  

Additionally, the simulation considers emergency situations for national flights:
- After **60 seconds**, a national flight enters a **critical state**.
- After **90 seconds**, if not attended, the flight is considered a **crash** (failure in the simulation).

---

## Airport Resources
The airport has the following limited resources:

- **3 runways**
- **5 gates**  
- **2 control tower**

---

## Airplane (Thread) Lifecycle
Each airplane (thread) goes through the following operations:

1. **Landing**
   - Requires: `1 runway + control tower`
   - Release after landing: runway + tower
   - After landing, the airplane requests a gate for disembarkment

2. **Disembarkment**
   - Requires: `1 gate + control tower`
   - Release after disembarkment: gate + tower
   - After disembarkment, the airplane waits for takeoff

3. **Takeoff**
   - Requires: `1 gate + 1 runway + control tower`
   - Releases all resources after takeoff  

---

## Difference Between National and International Flights
The main source of **deadlocks** is the difference in the **resource allocation order**:

### International Flights
- **Landing:** Runway → Tower  
- **Disembarkment:** Gate → Tower  
- **Takeoff:** Gate → Runway → Tower  

### National Flights
- **Landing:** Tower → Runway  
- **Disembarkment:** Tower → Gate  
- **Takeoff:** Tower → Gate → Runway  

**Priority:** International flights have **priority over national flights**.

---

## Critical States for National Flights
- After **60 seconds waiting**, a national flight enters a **critical state** (maximum urgency).  
- After **90 seconds waiting**, the flight is considered a **crash** (flight failure).  

---

## How to Run
1. Compile the project with:
   ```bash
   make
   ```
2. Run the simulation by specifying the time in minutes that the system should generate new threads (flights):
   ```bash
   ./main {time in minutes}
   ```
   Exemple:
   ```bash
   ./main 3
   ```
   This will make the system generate flights for 3 minutes.
3. After the flight generation ends, the application keeps running until all threads (airplanes) finish their operations.

## Notes
- The system is designed to run on Linux/Unix.
- Depending on the environment, you may need to link with -lpthread during compilation.
- The simulation can be extended with detailed flight logs, crash statistics, and average waiting times.
