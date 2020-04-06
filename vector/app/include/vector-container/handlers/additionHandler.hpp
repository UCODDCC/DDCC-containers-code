#ifndef _DDCD_HANDLER_ADITION_HANDLER_
#define _DDCD_HANDLER_ADITION_HANDLER_

#include <string>


struct workerParams {
    float** a; // vector 1
    float** b; // vector 2
    float** c; // result
    int thread_id;
    int n_threads;
    int n_lines;
    int line_size;
    pthread_t thread;
};

void *worker(void* params);
int vector_addition(float** a, float** b, int dim_a, int dim_b, float** result, int n_threads);
std::string additionHandler(const std::string& message);

#endif