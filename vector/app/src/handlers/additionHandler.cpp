#include <vector-container/handlers/additionHandler.hpp>
#include <vector-container/utils.hpp>
#include <stdexcept>

#define BASE_MEM_BLOCK 1024

void *Worker(void* params) {
    auto* thread_params = (struct workerParams*) params;
    /**
     * The memory is divided in blocks, each thread is assigned a starting block depending on it's thread id,
     * the vector's block base index is calculated by the multiplication the block index by the block size
     * the vector's block top index is calculated by the multiplication of 1+the block index and the block
     * this last position does not belong to that block, it belongs to the next
     *
     * once the thread finishes the block, the number or threads is added to the vector block index and the operation
     * is repeated
     *
     * The operations finishes once the total index is greater or equal to the vector size
     */
    int base_index = thread_params->thread_id * BASE_MEM_BLOCK;

    for (int block_index = thread_params->thread_id; (block_index * BASE_MEM_BLOCK); block_index+=thread_params->n_threads) {
        for (int vector_index = block_index * BASE_MEM_BLOCK; vector_index < (block_index+1) * BASE_MEM_BLOCK; ++vector_index) {
            if (vector_index >= thread_params->size )
                pthread_exit(nullptr);
            else
                (*thread_params->c)[vector_index] = (*thread_params->a)[vector_index] + (*thread_params->b)[vector_index];
        }
    }
    pthread_exit(nullptr);
}

int vector_addition(float** a, float** b, int size, float** result, int n_threads) {
    auto* params = (struct workerParams*)malloc(n_threads * sizeof(struct workerParams));
    for (size_t i = 0; i < n_threads; i++)
    {
        params[i].a=a;
        params[i].b=b;
        params[i].c=result;
        params[i].thread_id = i;
        params[i].n_threads = n_threads;
        params[i].size = size;
    }

    for (size_t i = 0; i < n_threads; i++)
        pthread_create(&(params[i].thread), nullptr, Worker, (void*)&(params[i]));
    for (size_t i = 0; i < n_threads; i++)
        pthread_join(params[i].thread, nullptr);
    return 0;
}

std::string additionHandler(const std::string& message) {
    unsigned long int size_separator;
    int size_a, size_b, retval;
    float* a, *b, *c;
    std::string response, metadata, payload, value;

    metadata = getMetaFromMessage(message);
    payload = getDataFromMessage(message);

    size_separator = metadata.find(',');
    try {
        size_a = std::atoi(metadata.substr(0, size_separator).c_str());
        size_b = std::atoi(metadata.substr(size_separator + 1, metadata.size()).c_str());
    }
    catch(std::invalid_argument& e){
        return std::string("-inconsistent metadata<metadata is incomplete or missing>");
    }

    if (size_a != size_b)
        return std::string("-inconsistent metadata<the size of the vectors in not the same in both cases>");

    a = (float*)malloc(size_a* sizeof(float));
    b = (float*)malloc(size_a* sizeof(float));
    c = (float*)malloc(size_a* sizeof(float));

    try {
        // get vector 1
        for (int i = 0; i < size_a; ++i) {
            value = popCSV(&payload);
            a[i] = std::atof(value.c_str());
        }

        // get vector 2
        for (int i = 0; i < size_b; ++i) {
            value = popCSV(&payload);
            b[i] = std::atof(value.c_str());
        }
    }
    catch(std::invalid_argument &e){
        return std::string("-inconsistent metadata<data is not coherent to the metadata provided>");
    }

    retval = vector_addition(&a, &b, size_a, &c, 5);


    response.append("+<");
    for (int i = 0; i < size_a; ++i) {
        response.append(std::to_string(c[i])).append(",");
    }
    response[response.length()-1] = '>';
    return response;
}