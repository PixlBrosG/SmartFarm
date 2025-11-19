# 🟦 Sprint 2 – Core Networking Layer
**Date:** Nov 11 (morning), 2025

## 🎯 Sprint Goals
- Implement the server’s asynchronous networking loop
- Create initial Sensor Node client
- Establish length-prefixed framed messaging
- Begin Control Panel module

## 📝 Planned Tasks
- Add Connection class handling async I/O
- Add Server with HELLO message handling
- Implement Sensor Node basic behavior
- Add Control Panel (early CLI version)

## ✅ Completed Work (git commits)
- `ebb47fb` Added common connection class
- `def2a01` Implemented basic server w/ message handling
- `44572c9` Implemented sensor node
- `763da1c` Implemented basic CLI control panel

## 🔍 Retrospective
**What went well**
- Asynchronous architecture worked smoothly
- End-to-end sensor → server → control communication started forming
- Server infrastructure proved easy to extend

**What to improve**
- Control panel UI still minimal
- Needed a clearer node identification system (added next sprint)
