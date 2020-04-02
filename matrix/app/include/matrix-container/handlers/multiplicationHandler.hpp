#ifndef _DDCD_HANDLER_MULTIPLICATION_HANDLER_
#define _DDCD_HANDLER_MULTIPLICATION_HANDLER_

#include <string>

#define NTHREADS 4

struct lineWorkerParams {
    float*** a; // matrix 1
    float*** b; // matrix 2
    float*** c; // result
    int thread_id;
    int n_threads;
    int n_lines;
    int line_size;
    pthread_t thread;
};
void *lineWorker_horizontal(void* params);
void *lineWorker_vertical(void* params);
int matrix_multiplication(float*** a, float*** b, int dim_ax, int dim_ay, int dim_bx, int dim_by, float*** result, int n_threads);
std::string multiplicationHandler(const std::string& message);

#endif