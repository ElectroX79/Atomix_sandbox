# Atomix Engine

Atomix Engine is a C++20 data processing engine designed as the computational core for high-performance applications.

Its first target application is a quantum computer simulator, but the architecture is intended to be reusable for other scientific computing and data-intensive workloads.

> **Project Status:** Early development. The architecture is actively evolving.

---

## Goals

The main goals of this project are:

- Build a modular and reusable data processing engine.
- Explore Data-Oriented Design (DOD) techniques.
- Optimize memory access patterns for CPU-intensive workloads.
- Learn modern C++ systems programming through a long-term engineering project.

---

## Architecture

Current high-level pipeline:

```
Input Data
     │
     ▼
 Reader
     │
     ▼
 Parser
     │
     ▼
 TabularData
     │
     ▼
 Engine
```

The internal architecture is intentionally modular so that individual components can evolve independently.

---

## Current Components

| Component | Description |
|-----------|-------------|
| `arena.hpp` | Arena allocator and memory management |
| `reader.hpp` | Input loading utilities |
| `parser.hpp` | Parsing infrastructure |
| `tabular_data.hpp` | Column-oriented table representation |
| `column.hpp` | Column abstraction |
| `engine.hpp` | Public engine interface |
| `security_check.hpp` | Input validation utilities |

---

## Technologies

- C++20
- CMake
- Catch2
- vcpkg

---

## Building

```bash
git clone <repository>

cmake -B build

cmake --build build
```

---

## Roadmap

- [x] Initial project structure
- [x] Reader implementation
- [x] Tabular data abstraction
- [ ] Initial parser
- [ ] Query execution layer
- [ ] Performance profiling
- [ ] SIMD optimizations
- [ ] Parallel execution
- [ ] Public C API

---

## Philosophy

This project is developed by Computer Engineering students as a long-term learning initiative.

Rather than maximizing features as quickly as possible, the focus is on understanding how high-performance systems are designed and implemented—from memory management and cache locality to modular software architecture.

---

## License

This project is licensed under the Apache-2.0 License.