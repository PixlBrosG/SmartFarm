# SmartFarm 🌿
*A socket-based smart farming system for IDATA2304 – Computer Communication and Network Programming (Fall 2025)*

## 📘 Overview
SmartFarm is a simulated **IoT-style farming system** built as part of the IDATA2304 course.  
It demonstrates **custom application-layer protocol design**, **TCP socket communication**, and **networked interaction** between distributed sensor/actuator nodes and control-panel nodes.

All hardware behavior (sensors, actuators, timers) is **simulated in software**.

Communication uses a **custom protocol defined in `protocol.md`**, built directly on top of **TCP**  
(no HTTP, MQTT, REST APIs, or other high-level protocols).

---

## 🏛️ Architecture
SmartFarm uses a **server-centric architecture**:

```
+---------------------------+
|       Control Panel       |
|   (User commands, CLI)    |
+-------------+-------------+
|                ^
| Commands       | updates 
▼                |
+-------------+--------------+
|             Server         |
| (Routing, registry, logic) |
+------+------+--------------+
^               |
| Updates       |   Commands
|               ▼
+-----------+------+-----------+
|             Sensor Nodes     |
|  (Temp/Humidity + Actuators) |
+------------------------------+
```

- Sensor nodes periodically send **SENSOR_UPDATE** messages.
- Server forwards updates to all connected control panels.
- Control panels send **COMMAND** messages that the server routes to the target sensor.
- All messages use an XOR-encrypted, length-prefixed JSON format.

---

## ⚙️ Components
| Component           | Description                                                                                                   |
|---------------------|---------------------------------------------------------------------------------------------------------------|
| **Sensor Node**     | Simulates temperature/humidity sensors and actuators. Sends periodic updates and receives control commands.   |
| **Control Panel**   | CLI interface for sending commands and displaying live sensor data.                                           |
| **Server (Broker)** | Central message router. Handles node registration, addressing, message forwarding, and disconnect cleanup.    |
| **Common Library**  | Shared utilities: protocol definitions, message framing, JSON serialization, encryption helpers, and logging. |

---

## 🧩 Technologies
- **C++20**
- **Asio** (standalone, no Boost)
- **spdlog** (structured logging)
- **nlohmann/json** (JSON serialization)
- **CMake** (modular project structure)
- **Git** (version control)

---

## 📦 Project Structure
```
smartfarm/
│
├── CMakeLists.txt
├── protocol.md
│
├── include/           # Include directory
│
├── src/
│   ├── Common/        # Shared utilities
│   ├── Server/        # Server program
│   ├── Sensor/        # Sensor node program
│   └── ControlPanel/  # Control panel program
│
└── docs/
└── ```optional-files```
```

---

## 🏗️ Build Instructions

```
# Clone repository
git clone `https://github.com/PixlBrosG/SmartFarm` smartfarm
cd smartfarm

# Configure & build
cmake -B build
cmake --build build --parallel

# Run (examples)
./build/src/Server/Server
./build/src/Sensor/SensorNode ```args```
./build/src/ControlPanel/ControlPanel
```

---

## 🚀 Implemented Features
- Custom **application-layer protocol**
- **Length-prefixed JSON messages**
- **XOR encryption layer** for payload obfuscation
- **Dynamic node registration** w/ server-assigned IDs
- **Bi-directional messaging** using TCP
- **Periodic sensor updates**
- **Actuator command routing**
- **Central node registry**
- **Graceful disconnect handling**
- **Automatic reconnection** for sensor nodes
- **Forwarding of sensor data** to all control panels
- CLI commands:
    - `send <node> <actuator> <action>`
    - `list`
    - `help`

---

## ⭐ Extra Features (Beyond Requirements)
These contribute to A-level grading:

- 🔐 Custom **encryption layer**
- 🔁 **Auto-reconnect** logic
- 🆔 **Automatic ID assignment**
- 🗑️ **Node cleanup** on disconnect
- 📡 **True multi-node architecture** (separate processes)
- 🧱 **Asynchronous architecture** built around Asio
- 🛠️ Extensible protocol structure (easy to add new types)

---

## 👥 Contributors
| Name / Candidate Number | Role                       |
|-------------------------|----------------------------|
| Candidate #10045        | Developer (entire project) |

---

## 🕓 Course Info
> NTNU – Norwegian University of Science and Technology  
> Course: **IDATA2304 – Computer Communication and Network Programming**  
> Semester: **Fall 2025**

---

## 📄 License
Created for educational purposes under NTNU coursework.  
Reuse is allowed with attribution.
