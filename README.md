# Ethernet Bus Simulation Project

## Overview
This project simulates Ethernet behavior using UNIX sockets in C, featuring frame transmission, collision detection, and exponential backoff using a central communication bus process (CBP).

## Files
- `comm_bus.c`: Communication Bus Process to mediate all transmissions
- `station.c`: Station code to read input frames and transmit using CSMA/CD logic
- `common.h`: Shared constants and frame structure
- `Makefile`: Compilation script
- `input/`: Directory containing frame input files for each station
- `logs/`: Runtime logs per station and CBP

## How to Compile
```bash
make
```

## How to Run
```bash
mkdir -p logs
./cbp &
./station 1 input/station1.txt &
./station 2 input/station2.txt &
./station 3 input/station3.txt &
./station 4 input/station4.txt &
```

## Output
- Log files are stored in `logs/` directory.
- Each station logs its frame transmission progress.
- CBP logs frame reception and collision events.

## Clean Project
```bash
make clean
```

---
This project meets the Spring 2025 Project requirements outlined in `sp-25-p.pdf`.
