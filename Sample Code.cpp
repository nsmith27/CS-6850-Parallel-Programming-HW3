#include <cstdlib>
#include "immintrin.h"
#include <chrono>
#include <cstdio>

int main() {
    int max = 10000000;
    int *a = (int*)aligned_alloc(64, sizeof(int) * max);
    int *output = (int*)aligned_alloc(64, sizeof(int) * 16);
    for (int i = 0, j = max - 1; i < max; i++, j--) {
        a[i] = j;
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    __m512i first = _mm512_load_si512(&a[0]); // Go get the first vector
 
    for (int i = 16; i < max; i += 16) {
        __m512i second = _mm512_load_si512(&a[i]); // Get the next vector
        first = _mm512_max_epi32(first, second);
    }
 
    _mm512_store_si512(&output[0], first);
    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
 
    for (int i = 0; i < 16; i++) {
        printf("%d ", output[i]);
    }
    
    printf("\n");
    printf("It took %g milliseconds\n", fp_ms.count());
    t1 = std::chrono::high_resolution_clock::now();
 
    unsigned int maxIndex = 0;
    for (int i = 0; i < max; i++) {
        if (a[i] > a[maxIndex]) {
            maxIndex = i;
        }
    }
 
    t2 = std::chrono::high_resolution_clock::now();
    fp_ms = t2 - t1;
 
    printf("The max was %d at index %u\n", a[maxIndex], maxIndex);
    printf("It took %g milliseconds\n", fp_ms.count());
    free(a);
    free(output);
    return 0;
}


// Compile with: g++ vector_avx_512_max.cc -o vector_avx_512_max.x -mavx512f
// g++ hw3.cpp -o hw3.x -mavx512f