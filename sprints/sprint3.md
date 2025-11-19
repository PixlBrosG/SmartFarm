# 🟦 Sprint 3 – Routing & Multi-Node Support
**Date:** Nov 11 (afternoon/evening), 2025

## 🎯 Sprint Goals
- Support multiple nodes in the system
- Add command forwarding from ControlPanel → Server → Sensor
- Add sensor update forwarding to control panels
- Introduce dynamic ID assignment and NodeRole enum

## 📝 Planned Tasks
- Add NodeRole enum + structured identification
- Implement command forwarding
- Implement update forwarding
- Add collision-safe dynamic ID assignment

## ✅ Completed Work (git commits)
- `3da6fad` Forwarding commands to sensors
- `b81971a` Added NodeRole enum
- `7a1ffec` Added automatic ID assignment
- `e200711` Added update forwarding to control panels

## 🔍 Retrospective
**What went well**
- System achieved true multi-node communication
- Routing logic stayed clean and scalable
- ID assignment simplified node management significantly

**What to improve**
- No error responses implemented yet
- Disconnect handling still primitive
