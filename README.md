# SmartFarm 🌿
*A socket-based smart farming system for IDATA2304 – Computer Communication and Network Programming (Fall 2025)*

## 📘 Overview
SmartFarm is a simulated **IoT-style farming system** developed as part of the IDATA2304 course.  
It demonstrates **application-layer protocol design**, **socket communication**, and **networked device control** between simulated sensor/actuator nodes and control-panel nodes.

The system uses a **custom protocol built on top of TCP/UDP (no HTTP/MQTT)**.  
All hardware behavior (sensors, actuators) is simulated in software.

## ⚙️ Components
| Component           | Description                                                                                                                     |
|---------------------|---------------------------------------------------------------------------------------------------------------------------------|
| **Sensor Node**     | Simulates temperature/humidity sensors and actuators (fan, heater, etc.). Sends periodic updates and receives control commands. |
| **Control Panel**   | Displays sensor data and actuator status. Allows user commands (CLI or GUI).                                                    |
| **Server (Broker)** | Routes messages between control panels and sensor nodes. Handles registration and addressing.                                   |
| **Common Library**  | Shared protocol definitions, serialization, and message handling logic.                                                         |

## 🧩 Technologies
- **C++20**
- **Asio** (standalone)
- **CMake** for modular builds
- **Git** for version control

## 🏗️ Build Instructions
```
# Clone repository
git clone https://github.com/<your-username>/smartfarm.git
cd smartfarm

# Configure & build
cmake -B build
cmake --build build

# Run (examples)
./build/src/Server/Server
./build/src/Sensor/SensorNode
./build/src/control/ControlPanel
```

## 🧠 Planned Features
- Custom application-layer protocol (defined in `protocol.md`)
- Support for multiple sensors/actuators per node
- Scalable message structure for new sensor types
- Optional GUI for control panel
- Basic error handling and resilience to connection loss

## 👥 Contributors
| Name           | Role                     |
|----------------|--------------------------|
| Candidate #xxx | Protocol & Networking    |
| Candidate #yyy | Control Panel / GUI      |
| Candidate #zzz | Sensor Node / Simulation |

## 🕓 Course Info
> Norwegian University of Science and Technology (NTNU)  
> Course: **IDATA2304 – Computer Communication and Network Programming**  
> Semester: Fall 2025
