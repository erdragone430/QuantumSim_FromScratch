#include "QuantumSimulator.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <random>

QuantumSimulator::QuantumSimulator(int n){
    num_qubits = n;
    state_size = 1ULL << num_qubits;

    state_vector.assign(state_size, std::complex<double>(0.0, 0.0));

    state_vector[0] = std::complex<double>(1.0, 0.0);

    std::cout << "[INFO] Simulator initialized for " << num_qubits 
    << " qubit.\nNumber of states in memory: " << state_size << "\n";
}

void QuantumSimulator::apply_X(int qubit){
    long long bit_mask = 1ULL << qubit;

    #pragma omp parallel for
    for(long long i = 0; i < state_size; i++){
        if((i & bit_mask) == 0){
            long long pair_index = i | bit_mask;

            std::complex<double> temp = state_vector[i];
            state_vector[i] = state_vector[pair_index];
            state_vector[pair_index] = temp;
        }
    }
}

void QuantumSimulator::apply_H(int qubit){
    long long bit_mask = 1ULL << qubit;
    double inv_sqrt2 = 1.0 / std::sqrt(2.0);
    #pragma omp parallel for
    for(long long i = 0; i < state_size; i++){
        if((i & bit_mask) == 0){
            long long pair_index = i | bit_mask;

            std::complex<double> v0_old = state_vector[i];
            std::complex<double> v1_old = state_vector[pair_index];
            state_vector[i] = (v0_old + v1_old) * inv_sqrt2;
            state_vector[pair_index] = (v0_old - v1_old) * inv_sqrt2;
        }
    }
}
void QuantumSimulator::apply_Z(int qubit){
    long long bit_mask = 1ULL << qubit;

    #pragma omp parallel for
    for(long long i = 0; i < state_size; i++){
        if((i & bit_mask) != 0)
            state_vector[i] = -state_vector[i];
    }
}

double QuantumSimulator::get_probability(long long state_index){
    double real_part = state_vector[state_index].real();
    double imag_part = state_vector[state_index].imag();
    return (real_part * real_part) + (imag_part * imag_part);
}

void QuantumSimulator::apply_CNOT(int control, int target){
    long long control_mask = 1ULL << control;
    long long target_mask  = 1ULL << target;

    #pragma omp parallel for
    for (long long i = 0; i < state_size; ++i) {
        // Entriamo nell'if SOLO SE il bit di controllo è 1... 
        // ... E contemporaneamente il bit target è 0 (per evitare di scambiare due volte)
        if (((i & control_mask) != 0) && ((i & target_mask) == 0)) {
            
            // Il gemello si trova "accendendo" il bit target
            long long pair_index = i | target_mask;

            // Facciamo lo swap classico
            std::complex<double> temp = state_vector[i];
            state_vector[i] = state_vector[pair_index];
            state_vector[pair_index] = temp;
        }
    }
}

void QuantumSimulator::measure(int shots) {
    std::cout << "\n=== SIMULATION MEASURE (" << shots << " toss) ===\n";
    
    // Configura il generatore di numeri casuali del C++ moderno
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    // Array per contare quante volte esce ogni stato
    std::vector<int> counts(state_size, 0);

    for (int s = 0; s < shots; ++s) {
        double r = dis(gen); // Numero casuale tra 0 e 1
        double cumulative_prob = 0.0;
        
        // Cerca in quale "fetta" di probabilità è caduto il numero casuale
        for (long long i = 0; i < state_size; ++i) {
            cumulative_prob += get_probability(i);
            if (r <= cumulative_prob) {
                counts[i]++;
                break;
            }
        }
    }

    // Stampa i risultati
    for (long long i = 0; i < state_size; ++i) {
        if (counts[i] > 0) {
            std::cout << "Stato " << i << " misurato " << counts[i] << " volte.\n";
        }
    }
    std::cout << "===============================================\n";
}

void QuantumSimulator::print_state() {
    std::cout << "\n--- FINAL STATE VECTOR ---\n";

    long long limit = (state_size > 32) ? 32 : state_size;

    for(long long i = 0; i < limit; i++){
        double prob = get_probability(i);

        if(prob > 0.0001){
            std::cout << "Stato " << i << " | Amplitude: ("
            << state_vector[i].real() << ". " << state_vector[i].imag()
            << ") | Probability: " << prob*100.0 << "%\n";
        }
    }
    if( state_size >32 )
            std::cout << "... (others " << state_size - 32 << " state were omitted)\n";
    std::cout << "-------------------------------\n";
}
