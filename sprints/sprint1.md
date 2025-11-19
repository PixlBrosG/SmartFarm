# 🟦 Sprint 1 – Foundations
**Dates:** Nov 9–10, 2025

## 🎯 Sprint Goals
- Establish overall project structure (CMake, folders)
- Define first version of the SmartFarm Protocol (v1.0)
- Implement logging and JSON utilities
- Prototype basic HELLO → ACK message exchange

## 📝 Planned Tasks
- Create repository + initial CMake structure
- Add protocol.md draft with terminology + message format
- Add JSON serialization support (nlohmann/json)
- Integrate spdlog logging
- Implement early Connection abstraction
- Test early communication flow

## ✅ Completed Work (git commits)
- `48c265b` Initial commit
- `9a46159` Initial project setup with CMake structure
- `965bedc` Added basic protocol v1.0
- `81adcac` Added spdlog logging
- `ee9d0c9` Defined protocol v1.0
- `c1eea92` Added JSON serialization
- `463b514` Tested simple HELLO → ACK handshake

## 🔍 Retrospective
**What went well**
- Solid structure created early; no refactors needed later
- Logging + serialization dramatically improved debugging
- Early protocol experimentation clarified message needs

**What to improve**
- Needed more clarity on architecture (server roles)
- Should plan message framing earlier (added later in sprint 2)
