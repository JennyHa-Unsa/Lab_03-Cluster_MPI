#include <iostream>
#include <mpi.h>
#include <cmath>

int main(int argc, char** argv) {
    int rank, size;
    double start_time, end_time;
    
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    start_time = MPI_Wtime();
    
    std::cout << "Hola desde el Proceso " << rank 
              << " de un total de " << size << " procesos." << std::endl;
    
    double local_val = rank * 3.14159;
    double global_sum = 0.0;
    
    MPI_Reduce(&local_val, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    end_time = MPI_Wtime();
    
    if (rank == 0) {
        std::cout << "-----------------------------" << std::endl;
        std::cout << "Resultado de la suma global: " << global_sum << std::endl;
        std::cout << "Tiempo total de ejecución: " << (end_time - start_time) 
                  << " segundos" << std::endl;
        std::cout << "-----------------------------" << std::endl;
    }
    
    MPI_Finalize();
    
    return 0;
}
