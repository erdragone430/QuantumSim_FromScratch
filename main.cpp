#include <iostream>
#include "QuantumSimulator.h"
#include <string>

int main(int argc, char* argv[] ){
if (argc < 2) {
        std::cerr << "Errore: Number of qubit not specified!\n";
        std::cout << "Consuming: " << argv[0] << " <num_qubits> [num_shots]\n";
        std::cout << "Example:  " << argv[0] << " 5 1000\n";
        return 1;
    }

    
    int n;
    int shots = 1000; 
    
    try {
        n = std::stoi(argv[1]);
        if (argc >= 3) {
            shots = std::stoi(argv[2]);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Args must be valid int value.\n";
        return 1;
    }

  
    if (n <= 0 || n > 30) {
        std::cerr << "Error: number of qubit must be between 1 and 30.\n";
        return 1;
    }

    std::cout << "--- Start Quantistic simulation " << n << " Qubit ---\n";

   
    QuantumSimulator sim(n);

  
    for (int i = 0; i < n; ++i) {
        sim.apply_H(i);
    }

   
    for (int i = 0; i < n - 1; ++i) {
        sim.apply_CNOT(i, i + 1);
    }


    sim.print_state();
    sim.measure(shots);

    return 0;
}


