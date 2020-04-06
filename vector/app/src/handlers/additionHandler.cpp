#include <vector-container/handlers/additionHandler.hpp>
#include <vector-container/utils.hpp>
#include <stdexcept>



void *Worker(void* params) {
    auto* thread_params = (struct workerParams*) params;

    pthread_exit(nullptr);
}

int vector_addition(float** a, float** b, int size, float** result, int n_threads) {

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