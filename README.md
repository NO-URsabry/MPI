# MPI


# Parallel Matrix Multiplication with MPI

## Overview
This project implements a parallel matrix multiplication algorithm using the Message Passing Interface (MPI) in C. It efficiently multiplies two matrices, A (X×Z) and B (Z×Y), to produce a result matrix C (X×Y) by distributing the computation across multiple processes. The code is designed for scalability and handles dynamic workload distribution, making it suitable for high-performance computing environments.

## Features
- **Parallel Processing**: Leverages MPI to distribute matrix rows across processes, reducing computation time for large matrices.
- **Dynamic Load Balancing**: Evenly divides rows among processes, accounting for remainders to ensure fair workload distribution.
- **Memory Management**: Includes proper allocation and deallocation to prevent memory leaks.
- **Input Validation**: Checks if the number of processes exceeds the number of rows in matrix A, preventing invalid configurations.
- **Random Matrix Generation**: Initializes matrices A and B with random integers for testing.
- **Clear Output**: Prints input matrices A and B, and the resulting matrix C, with formatted dimensions.

## Matrix Dimensions
- Matrix A: X×Z (6×5 by default)
- Matrix B: Z×Y (5×6 by default)
- Result Matrix C: X×Y (6×6 by default)

Dimensions can be modified via the `#define` constants `X`, `Z`, and `Y` in the source code.

## Prerequisites
- **MPI Implementation**: OpenMPI or MPICH (e.g., install `openmpi` on Linux: `sudo apt-get install openmpi-bin openmpi-common libopenmpi-dev`).
- **C Compiler**: GCC or any compatible compiler.
- **Operating System**: Linux, macOS, or Windows with an MPI-compatible environment.

## Compilation and Execution
1. **Compile**:
   ```bash
   mpicc -o matrix_mult matrix_mult.c
   ```
2. **Run**:
   ```bash
   mpirun -np <num_processes> ./matrix_mult
   ```
   Replace `<num_processes>` with the desired number of processes (must be ≤ X, the number of rows in matrix A).

## Code Structure
- **main()**: Initializes MPI, sets up matrices, broadcasts input matrices, gathers results, and handles output.
- **Mul_matrices()**: Performs matrix multiplication for a subset of rows assigned to each process.
- **print_matrix()**: Utility function to display matrices in a readable format.
- **MPI Functions**:
  - `MPI_Bcast`: Broadcasts matrices A and B to all processes.
  - `MPI_Gatherv`: Collects computed rows of matrix C from all processes.
  - `MPI_Init`, `MPI_Comm_rank`, `MPI_Comm_size`, `MPI_Finalize`: Standard MPI setup and teardown.

## Example Output
For default dimensions (X=6, Z=5, Y=6) with 4 processes:
```
Matrix A (6 x 5):
23 45 12 67 89
...
Matrix B (5 x 6):
34 56 78 90 12 34
...
Resulting Matrix C (6 x 6):
1234 5678 9012 ...
...
```

## Notes
- The code assumes the number of processes does not exceed the number of rows in matrix A (X). If exceeded, an error message is displayed, and the program exits.
- Random matrix values are generated using `rand() % 100` for simplicity. Modify as needed for specific use cases.
- The program is optimized for clarity and correctness, with potential for further optimization (e.g., cache-friendly access patterns or advanced MPI collectives).

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contributing
Contributions are welcome! Please submit a pull request or open an issue for bugs, improvements, or feature requests.
