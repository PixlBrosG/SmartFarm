# SmartFarm Protocol Specification 🌿
*Custom application-layer protocol for IDATA2304 – Computer Communication and Network Programming (Fall 2025)*

---

> **Protocol Version:** 1.1
> **Last updated:** `19/11/2025`
> **Author:** `Candidate #10045`

---

## 1️⃣ Introduction
This document describes the **SmartFarm Protocol (SFP)**, a custom application-layer protocol for communication between **sensor/actuator nodes**, **control-panel nodes**, and the **central server** in a simulated smart farming environment.

The protocol enables:
- Exchange of **sensor readings** (e.g., temperature, humidity)
- **Actuator control commands** (e.g., fan/heater ON/OFF)
- **Status updates** between nodes  
  All communication runs directly over ```TCP```, without higher-level protocols such as HTTP or MQTT.

All communication uses a custom framing format (4-byte big-endian length prefix + encrypted JSON payload).

---

## 2️⃣ Terminology
| Term                 | Description                                                                   |
|----------------------|-------------------------------------------------------------------------------|
| **Node**             | A device participating in the SmartFarm network (sensor, control, or server). |
| **Sensor Node**      | Simulated device that measures environmental data and exposes actuators.      |
| **Control Panel**    | User-facing node that displays data and sends control commands.               |
| **Server**           | Optional message broker that routes data and manages node registration.       |
| **SFP**              | SmartFarm Protocol, the application-layer protocol defined here.              |
| **Message**          | A structured unit of communication exchanged between nodes.                   |
| **Actuator**         | A controllable output (fan, heater, window motor)                             |
| **Encryption Layer** | Simple XOR-based symmetric cipher applied to all payloads.                    |

---

## 3️⃣ Transport
| Property                 | Value                 |
|--------------------------|-----------------------|
| **Underlying Transport** | `TCP`                 |
| **Port Number**          | `5000` (default)      |
| **Connection Type**      | `Connection-oriented` |
| **Protocol State**       | `Stateful`            |

---

## 4️⃣ Architecture Overview

SmartFarm uses a server-centric architecture where all communication passes
through the central server.

Sensor Nodes and Control Panels are separate TCP clients.

Direct Sensor → Control communication does not occur; all routing is handled
by the server.

**Actors:**
- Sensor Nodes (`SNode`)
- Control Panels (`CNode`)
- Server (`Svr`, central router, required)

**Example flow:**
```
CNode  →  Svr  →  SNode   (Send control command)
SNode  →  Svr  →  CNode   (Send sensor update)
```

**Clients:** Sensor Nodes and Control Panels  
**Server:** Routes messages, manages node registry

---

## 5️⃣ Information Flow
Describe how messages move through the network:
- **Push model:** Sensor nodes periodically push updates.
- **Pull model:** Control panels request updates.
- **Hybrid:** Combination of both (recommended).

```
Each Sensor Node sends periodic SENSOR_UPDATE messages to the Server, which forwards them to all subscribed Control Panels.  
Control Panels can send COMMAND messages to the Server, which forwards them to specific Sensor Nodes.
```

---

## 6️⃣ Message Types
| Type              | Direction                 | Purpose                        |
|-------------------|---------------------------|--------------------------------|
| `HELLO`           | Node → Server             | Register node and capabilities |
| `ACK`             | Any                       | Confirm receipt or success     |
| `SENSOR_UPDATE`   | Sensor → Server → Control | Transmit sensor readings       |
| `ACTUATOR_STATUS` | Sensor → Server → Control | Report actuator states         |
| `COMMAND`         | Control → Server → Sensor | Send actuator commands         |
| `ERROR`           | Any                       | Report protocol or data errors |
| `LIST_NODES`      | Control → Server          | Request node list              |
| `NODE_LIST`       | Server → Control          | List nodes                     |

---

## 7️⃣ Message Format
All messages are encoded as JSON strings for simplicity.

Example SENSOR_UPDATE:
```
{
    "type": "SENSOR_UPDATE",
    "node_id": 7,
    "timestamp": 1731084000,
    "sensors": {
        "temperature": 22.4,
        "humidity": 46.1
    }
}
```

Example COMMAND:
```
{
    "type": "COMMAND",
    "target_node": 7,
    "target_actuator": "fan",
    "action": "OFF"
}
```

### 7.1 Framing
All messages are prefixed with a 4-byte big-endian unsigned integer indicating
the payload length.

| uint32 length | encrypted JSON payload |

### 7.2 Encryption
Before transmission, the JSON payload is XOR-encrypted using a repeating-key
symmetric cipher.

Decryption applies the same operation.


---

## 8️⃣ Special Constants
| Constant             | Description               |
|----------------------|---------------------------|
| `MAX_PACKET_SIZE`    | `4096` bytes              |
| `HEARTBEAT_INTERVAL` | `5` seconds               |
| `NODE_ID_UNASSIGNED` | `0`                       |
| `BROADCAST_ALL`      | `255` (send to all nodes) |
| `ENCRYPTION_KEY`     | `smartfarm_secret_key`    |

---

## 9️⃣ Error Handling
Possible error cases and handling strategies:

| Error Type         | Description                      | Handling                   |
|--------------------|----------------------------------|----------------------------|
| `INVALID_FORMAT`   | Malformed message                | Drop message, send `ERROR` |
| `UNKNOWN_NODE`     | Node not registered              | Send `ERROR` to sender     |
| `TIMEOUT`          | No response within `x` seconds   | Retry or reconnect         |
| `UNSUPPORTED_TYPE` | Unknown message type             | Log and ignore             |

---

## 🔒 0️⃣ Reliability and Security
This implementation uses a lightweight XOR encryption layer for payload obfuscation.
This is not cryptographically secure but demonstrates how encryption can be
integrated at the protocol layer.

Future versions could replace XOR with AES or TLS (asio::ssl::stream) without
changing the protocol design.

---

## 📘 1️⃣ Example Session
```
1. Sensor Node 7 connects to Server and sends HELLO.
2. Server assigns ID = 7.
3. Control Panel connects.
4. Sensor sends SENSOR_UPDATE { temp=22.5 }.
5. Server forwards update to Control Panels.
6. Control Panel sends COMMAND { fan=ON }.
7. Server forwards to Node 7.
8. Sensor toggles actuator, replies ACTUATOR_STATUS.
   ```

---

## 🧠 2️⃣ Design Justifications
- **TCP** chosen for reliability and ordered delivery.
- **JSON encoding** chosen for simplicity; length-prefixing chosen for robustness over streaming TCP.
- **Server-based routing** for scalability and simpler addressing.
- **Extensible message types** for future sensor/actuator additions.
- **Separation of concerns** between data, commands, and control flow.

---
