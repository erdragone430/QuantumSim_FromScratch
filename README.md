# Quantum Simulator
I wanted to undertsand more about Quantum Computing.
Quantum computing is a huge topic, and I felt that just using a ready-made library wasn't enough for me to really 'get' it. I decided to write this simulator from scratch to see what happens behind the scenes

## Features
- **In-place computation**: Memory-efficient bitwise operations.
- **Multithreading**: Optimized with OpenMP for multicore CPUs.
- **CLI Interface**: Accepts qubit count and shots as arguments.

## Mathematical Model

This simulator models a quantum system of $N$ qubits. 
The global state of the system is represented by a complex state vector $\left| \psi \right\rangle$ of size $2^N$:

$$\left| \psi \right\rangle = \sum_{i=0}^{2^N-1} \alpha_i \left| i \right\rangle$$

where $\alpha_i \in \mathbb{C}$ represents the probability amplitude of the basis state $\left| i \right\rangle$. 
The simulation enforces the normalization condition $\sum_{i} |\alpha_i|^2 = 1$.

Quantum gates (X, H, Z, CNOT) are applied not via naive matrix multiplication (which would require sparse matrices of size $2^N \times 2^N$), but through **in-place bitwise operations** that directly manipulate the pairs of probability amplitudes involved.

## Memory Complexity Warning

Simulating quantum systems on classical hardware requires exponential memory $\mathcal{O}(2^N)$. 
Each state amplitude uses a `std::complex<double>` (16 bytes). 
- 10 qubits $\approx$ 16 KB
- 20 qubits $\approx$ 16 MB
- **30 qubits $\approx$ 16 GB of RAM**

*Do not simulate more than 28-30 qubits unless you have sufficient physical RAM, or your system will trigger the OOM (Out-Of-Memory) killer.*

## Compilation
```bash
g++ main.cpp QuantumSimulator.cpp -o qsim -O3 -fopenmp
```

## USAGE
```bash
./qsim <number_of_qubits> <number_of_shots>
