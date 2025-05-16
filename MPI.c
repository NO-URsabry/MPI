#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

// Dimentions -> result matrix C = X x Y

#define X 5  // Rows of A and C
#define Z 5  // Columns of A, Rows of B
#define Y 6  // Columns of B and C


void Mul_matrixes(int a[X][Z], int b[Z][Y], int c[X][Y], int start, int end) {
    for (int i = start; i < end; i++) {
        for (int j = 0; j < Y; j++) {
            c[i][j] = 0;
            for (int k = 0; k < Z; k++) {
                c[i][j] += a[i][k] * b[k][j];  // Sum of products
            }
        }
    }
}

void print_matrixes(const char* name, int rows, int cols, int* matrix) {
    printf("%s (%d x %d):\n", name, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i * cols + j]);
        }
        printf("\n");
    }
}

int main(int argc, char** argv) {
    int rank, num_processes;
    int a[X][Z], b[Z][Y];
    int c[X][Y];
    int local_c[X][Y];
    int start, end, rows_per_process;
    int *recv_counts = NULL, *display = NULL;

    // MPI environment Init
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

    // Validation --> Num of proccess <= Num of Rows
    if (num_processes > X && rank == 0) {
        printf("Err: Num of processes = (%d) is GT matrix A rows (%d).\n", num_processes, X);
        MPI_Finalize();
        return 1;
    }

    // Calculate Num of rows per process
    int base_rows = X / num_processes;
    int reminder = X % num_processes;
    rows_per_process = (rank < reminder) ? base_rows + 1 : base_rows;  // Extra row for processes cuz of reminder -> (one line if)
    start = rank * base_rows + (rank < reminder ? rank : reminder);
    end = start + rows_per_process;

    // Allocate arrays & Collecting all the results from all processes
    if (rank == 0) {
        recv_counts = (int*)malloc(num_processes * sizeof(int));
        display = (int*)malloc(num_processes * sizeof(int));
        int offset = 0;
        for (int i = 0; i < num_processes; i++) {
            recv_counts[i] = ((i < reminder) ? base_rows + 1 : base_rows) * Y;
            display[i] = offset;
            offset += recv_counts[i];
        }
    }

    if (rank == 0) {
        srand(time(NULL));
        // Generate Random values for matrix A and B range from 0-99
        for (int i = 0; i < X; i++) {
            for (int j = 0; j < Z; j++) {
                a[i][j] = rand() % 100;
            }
        }

        for (int i = 0; i < Z; i++) {
            for (int j = 0; j < Y; j++) {
                b[i][j] = rand() % 100;
            }
        }

        print_matrixes("Matrix A", X, Z, (int*)a);
        print_matrixes("Matrix B", Z, Y, (int*)b);
    }

    // Broadcast matrixes A and B to all processes --> share the matrixes to all processes
    MPI_Bcast(a, X * Z, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(b, Z * Y, MPI_INT, 0, MPI_COMM_WORLD);

    Mul_matrixes(a, b, local_c, start, end);

    // collecting the results from all processes
    MPI_Gatherv(&local_c[start][0], rows_per_process * Y, MPI_INT, c, recv_counts, display, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        print_matrixes("Resulting Matrix C", X, Y, (int*)c);

        // Clean up if you don’t wanna leak memory !.. cuz, C dont have garbage collector
        free(recv_counts);
        free(display);
    }
    // MPI environment Close
    MPI_Finalize();
    return 0;
}

// lets see the run

