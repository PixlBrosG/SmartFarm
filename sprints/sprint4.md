# 🟦 Sprint 4 – Robustness & Error Handling
**Dates:** Nov 11 → Nov 15, 2025

## 🎯 Sprint Goals
- Add error reporting and validation
- Handle clean disconnections
- Add diagnostic and UX improvements for the Control Panel
- Improve resilience on Sensor startup

## 📝 Planned Tasks
- Add ERROR response messages
- Disconnect detection + removal of stale entries
- Add 'list nodes' command to Control Panel
- Add auto-retry for failed Sensor connections

## ✅ Completed Work (git commits)
- `40013a4` Handle disconnections gracefully
- `852ba52` Added error feedback on invalid commands
- `0d24b6d` Added command to list active nodes
- `34dcb4c` Sensor retry failed connect on startup

## 🔍 Retrospective
**What went well**
- System now handles broken connections cleanly
- UX improved with meaningful error messages
- Control panel gained necessary administration tools

**What to improve**
- Security layer (encryption) still missing
- Documentation lagged behind development progress
