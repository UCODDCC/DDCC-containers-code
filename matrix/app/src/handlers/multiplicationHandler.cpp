#include <matrix-container/handlers/multiplicationHandler.hpp>
#include <matrix-container/utils.hpp>
#include <stdexcept>



void *lineWorker_horizontal(void* params) {
    auto* thread_params = (struct lineWorkerParams*) params;
    float tmp;

    for (size_t line_a = 0; line_a < thread_params->n_lines; line_a += thread_params->n_threads) {
        // clear tmp
        tmp = 0;
        // get line result into tmp
        for (size_t line_b = 0; line_b < thread_params->n_lines; line_b += thread_params->n_threads) {
            for (size_t line_pos = 0; line_pos < thread_params->line_size; line_pos++) { // item in line
                tmp += (*thread_params->a)[line_a][line_pos] * (*thread_params->b)[line_pos][line_b];
            }
            // copy the line result to the final matrix cell
            (*thread_params->c)[line_a][line_b] = tmp;
        }
    }
    pthread_exit(nullptr);
}


void *lineWorker_vertical(void* params) {
    auto* thread_params = (struct lineWorkerParams*) params;
    float tmp;

    for (size_t line_b = 0; line_b < thread_params->n_lines; line_b += thread_params->n_threads) {
        // clear tmp
        tmp = 0;
        // get line result into tmp
        for (size_t line_a = 0; line_a < thread_params->n_lines; line_a += thread_params->n_threads) {
            for (size_t line_pos = 0; line_pos < thread_params->line_size; line_pos++) { // item in line
                tmp += (*thread_params->a)[line_a][line_pos] * (*thread_params->b)[line_pos][line_b];
            }
            // copy the line result to the final matrix cell
            (*thread_params->c)[line_a][line_b] = tmp;
        }
    }
    pthread_exit(nullptr);
}

int matrix_multiplication(float*** a, float*** b, int dim_ax, int dim_ay, int dim_bx, int dim_by, float*** result, int n_threads) {
    if (dim_ay != dim_bx)
        return 1;

    auto* params = (struct lineWorkerParams*)malloc(n_threads * sizeof(struct lineWorkerParams));
    for (size_t i = 0; i < n_threads; i++)
    {
        params[i].a=a;
        params[i].b=b;
        params[i].c=result;
        params[i].thread_id = i;
        params[i].n_threads = n_threads;
        params[i].n_lines = dim_ax;
        params[i].line_size = dim_ay;
    }

    for (size_t i = 0; i < n_threads; i++)
        pthread_create(&(params[i].thread), nullptr, lineWorker_vertical, (void*)&(params[i]));
    for (size_t i = 0; i < n_threads; i++)
        pthread_join(params[i].thread, nullptr);
    return 0;
}

std::string multiplicationHandler(const std::string& message) {
    std::string size_a, size_b;
    int size_ax, size_ay, size_bx, size_by;
    int retval;
    unsigned long int size_separator;
    std::string response, value;
    std::string metadata = getMetaFromMessage(message);
    std::string payload = getDataFromMessage(message);
    float** a;
    float** b;
    float** c;
    printf("payload:<%s>\n", payload.c_str());

    size_separator = metadata.find(',');
    size_a = metadata.substr(0, size_separator);
    size_b = metadata.substr(size_separator + 1, metadata.size());
    try {
        size_ax = std::stoi(size_a.substr(0, size_a.find('x')));
        size_ay = std::stoi(size_a.substr(size_a.find('x') + 1, size_a.size()));
        size_bx = std::stoi(size_b.substr(0, size_b.find('x')));
        size_by = std::stoi(size_b.substr(size_b.find('x') + 1, size_b.size()));
    }
    catch(std::invalid_argument& e){
        return std::string("-metadata is not complete<>");
    }

    a = (float**)malloc(size_ay* sizeof(float*));
    b = (float**)malloc(size_by* sizeof(float*));
    c = (float**)malloc(size_ay* sizeof(float*));
    for (int i = 0; i < size_ax; ++i) {
        a[i] = (float*)malloc(size_ax* sizeof(float));
    }
    for (int i = 0; i < size_bx; ++i) {
        b[i] = (float*)malloc(size_bx* sizeof(float));
        c[i] = (float*)malloc(size_bx* sizeof(float));
    }

    for (int i = 0; i < size_ay; ++i) {
        for (int j = 0; j < size_bx; ++j) {
            c[i][j] = 0;
        }
    }

    try {
        // get matrix 1
        for (int i = 0; i < size_ax; ++i) {
            for (int j = 0; j < size_ay; ++j) {
                value = popCSV(&payload);
                a[i][j] = std::atof(value.c_str());
            }
        }

        // get matrix 2
        for (int i = 0; i < size_bx; ++i) {
            for (int j = 0; j < size_by; ++j) {
                value = popCSV(&payload);
                b[i][j] = std::atof(value.c_str());
            }
        }
    }
    catch(std::invalid_argument &e){
        return std::string("-data is not coherent to metadata<>");
    }

    /*printf("matrix 1\n");
    for (int i = 0; i < size_ax; ++i) {
        for (int j = 0; j < size_ay; ++j) {
            printf("%f, ",a[i][j]);
        }
        printf("\n");
    }

    printf("matrix 2\n");
    // get matrix 2
    for (int i = 0; i < size_bx; ++i) {
        for (int j = 0; j < size_by; ++j) {
            printf("%f, ",b[i][j]);
        }
        printf("\n");
    }*/

    retval = matrix_multiplication(&a, &b, size_ax, size_ay, size_bx, size_by, &c, NTHREADS);

    response.append("+<");
    for (int i = 0; i < size_ay; ++i) {
        for (int j = 0; j < size_bx; ++j) {
            response.append(std::to_string(c[i][j])).append(",");
        }
    }
    response[response.length()-1] = '>';

    return response;
}