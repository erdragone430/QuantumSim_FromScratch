#ifndef QUANTUM_SIMULATOR_H
#define QUANTUM_SIMULATOR_H
#include <iostream>
#include <vector>
#include <complex>


class QuantumSimulator{
private: 
    int num_qubits;
    long long state_size;
    std::vector<std::complex<double>> state_vector;
public:
    QuantumSimulator(int n);
    // NOT GATE
    void apply_X(int qubit);
    // Hadamard
    void apply_H(int qubit);
    // Phase change
    void apply_Z(int qubit);
    // To link 2 qubit
    void apply_CNOT(int control, int target);
    
    double get_probability(long long state_index);
    void print_state();
    //Simulate coin toss
    void measure(int shots);
};

#endif