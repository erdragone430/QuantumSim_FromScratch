# Quantum Simulator
I wanted to undertsand more about Quantum Computing.
Quantum computing is a huge topic, and I felt that just using a ready-made library wasn't enough for me to really 'get' it. I decided to write this simulator from scratch to see what happens behind the scenes

## Features
- **In-place computation**: Memory-efficient bitwise operations.
- **Multithreading**: Optimized with OpenMP for multicore CPUs.
- **CLI Interface**: Accepts qubit count and shots as arguments.

## Compilation
```bash
g++ main.cpp QuantumSimulator.cpp -o qsim -O3 -fopenmp
```

## USAGE
```bash
./qsim <number_of_qubits> <number_of_shots>