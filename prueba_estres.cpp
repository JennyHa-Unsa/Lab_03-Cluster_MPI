#include <iostream>
#include <mpi.h>
#include <cmath>
#include <chrono>
#include <thread>

// Función que consume CPU al 100% sin generar I/O en disco
void cpu_stress(int duration_seconds) {
    auto start = std::chrono::steady_clock::now();
    volatile double x = 0; // volatile evita optimizaciones del compilador
    
    while (std::chrono::duration_cast<std::chrono::seconds>(
               std::chrono::steady_clock::now() - start).count() < duration_seconds) {
        for (int i = 0; i < 1000000; ++i) {
            x += std::sin(i) * std::cos(i);
            x = std::sqrt(std::fabs(x) + 1.0);
        }
    }
}

int main(int argc, char** argv) {
    int rank, size;
    int duration = 60; // Duración predeterminada: 60 segundos
    
    if (argc > 1) duration = std::stoi(argv[1]);
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    auto start_time = std::chrono::steady_clock::now();
    
    std::cout << "[Proceso " << rank << "/" << size 
              << "] Iniciando estrés CPU por " << duration << "s..." << std::endl;
    
    cpu_stress(duration);
    
    auto end_time = std::chrono::steady_clock::now();
    double elapsed = std::chrono::duration<double>(end_time - start_time).count();
    
    std::cout << "[Proceso " << rank << "] Completado en " 
              << elapsed << "s" << std::endl;
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    if (rank == 0) {
        std::cout << "\n=== RESUMEN DEL ESTRÉS ===" << std::endl;
        std::cout << "Nodos totales: " << size << std::endl;
        std::cout << "Duración real: " << elapsed << "s" << std::endl;
        std::cout << "Carga objetivo: 100% en todos los núcleos" << std::endl;
    }
    
    MPI_Finalize();
    return 0;
}