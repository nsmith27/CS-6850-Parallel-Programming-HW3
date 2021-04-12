#include "immintrin.h"
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <random>
#include <chrono>
#include <string>
#include <vector>
#include <queue>  

using namespace std;



template <typename T>
void selectionSort(int size, T * A);

template <typename T>
void bubbleSort(int size, T * A);

template <typename T> 
void aShuffle(int size, T * A );

template <typename T>
void aprint(int size, T * A);

int * getCopyArr(int n, int * start);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Helper Functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Code to help print an __m512 vector:
void printVectorInt(__m512i v, string name) {
    #if defined (__GNUC__)
    int* temp = (int*)aligned_alloc(64, sizeof(int) * 16);
    #elif defined (_MSC_VER)
    int* temp = (int*)_aligned_malloc(sizeof(int) * 16, 64);
    #endif

    _mm512_store_si512(temp, v);
    printf("The vector called %s contains: ", name.c_str());

    for (int i = 0; i < 16; i++) {
        printf("%02d ", temp[i]);   
    }
    printf("\n");

    #if defined (__GNUC__)
    free(temp);
    #elif defined (_MSC_VER)
    _aligned_free(temp);
    #endif

}
// void printVectorFloat(__m512 v, string name) {
//     #if defined (__GNUC__)
//     float* temp = (float*)aligned_alloc(64, sizeof(float) * 16);
//     #elif defined (_MSC_VER)
//     float* temp = (float*)_aligned_malloc(sizeof(float) * 16, 64);
//     #endif

//     _mm512_store_ps(temp, v);
//     printf("The vector called %s contains: ", name.c_str());

//     for (int i = 0; i < 16; i++) {
//         printf("%3f ", temp[i]);
//     }
//     printf("\n");

//     #if defined (__GNUC__)
//     free(temp);
//     #elif defined (_MSC_VER)
//     _aligned_free(temp);
//     #endif
// }

// Print array
template <typename T>
void aprint(int size, T * A){
    cout << size << "\t";  
    for (int i = 0; i < size; i++) {
        if (A[i] < 10){
            cout << "0"; 
        }
        cout << A[i];
        if (i+1 != size ){
            if (i % 16 == 15){
                cout << " +|+";  
        }                }
        cout << " ";  
    }
    cout << endl; 
    return;  
}

// Randomize elements of a vector
template <typename T> 
void aShuffle(int size, T * A ){
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

    for (int i = 0; i < size; i++){
        std::uniform_int_distribution<int> rand_elem(0, i);
        int i2 = rand_elem(gen);
        T temp = A[i];
        A[i] = A[i2];
        A[i2] = temp;  
    }
    return;  
}

int * getCopyArr(int n, int * start){
    int * result = (int*)aligned_alloc(64, sizeof(int) * n);
    for (int i = 0; i < n; i++){
        result[i] = start[i];
    }
    return result;  
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Primary Functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Selection Sort
template <typename T>
void selectionSort(int size, T * A){
    int swaps;
    for (int i = 0; i < size-1; i++){
        int min = i;
        for (int j = i+1; j < size; j++){
            if (A[j] < A[min]){
                min = j;
            }
        }
        T temp = A[i];
        A[i] = A[min];
        A[min] = temp;   
    }
    return;  
}

// Bubble Sort 
template <typename T>
void bubbleSort(int size, T * A){
    int swaps;
    do {
        swaps = 0;
        for (int k = 0; k < size-1; k++){
            if ( A[k] > A[k+1] ) {
                T temp = A[k];
                A[k] = A[k+1];
                A[k+1] = temp;
                swaps += 1;  
            }
        }
    } while (swaps);

    return;
}



// Takes in a pointer to 32 ints (32bit each) 
void bitonicSort(__m512i &A1, __m512i &A2, __m512i &A1out, __m512i &A2out) {
    cout << endl << "bitonicSort() CALLED! " << endl << endl;  

    printVectorInt(A1, "A1");
    printVectorInt(A2, "A2");


    __m512i LA;
    __m512i HA;

    __m512i reverse = _mm512_set_epi32(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
    __m512i idx_L1 = _mm512_set_epi32(23, 22, 21, 20, 19, 18, 17, 16, 7, 6, 5, 4, 3, 2, 1, 0);
    __m512i idx_H1 = _mm512_set_epi32(31, 30, 29, 28, 27, 26, 25, 24, 15, 14, 13, 12, 11, 10, 9, 8);
    __m512i idx_L2 = _mm512_set_epi32(27, 26, 25, 24, 11, 10, 9, 8, 19, 18, 17, 16, 3, 2, 1, 0);
    __m512i idx_H2 = _mm512_set_epi32(31, 30, 29, 28, 15, 14, 13, 12, 23, 22, 21, 20, 7, 6, 5, 4);
    __m512i idx_L3 = _mm512_set_epi32(29, 28, 13, 12, 25, 24, 9, 8, 21, 20, 5, 4, 17, 16, 1, 0);
    __m512i idx_H3 = _mm512_set_epi32(31, 30, 15, 14, 27, 26, 11, 10, 23, 22, 7, 6, 19, 18, 3, 2);
    __m512i idx_L4 = _mm512_set_epi32(30, 14, 28, 12, 26, 10, 24, 8, 22, 6, 20, 4, 18, 2, 16, 0);
    __m512i idx_H4 = _mm512_set_epi32(31, 15, 29, 13, 27, 11, 25, 9, 23, 7, 21, 5, 19, 3, 17, 1);
    __m512i idx_L5 = _mm512_set_epi32(23, 7, 22, 6, 21, 5, 20, 4, 19, 3, 18, 2, 17, 1, 16, 0);
    __m512i idx_H5 = _mm512_set_epi32(31, 15, 30, 14, 29, 13, 28, 12, 27, 11, 26, 10, 25, 9, 24, 8);

    // Reverse Step
    A2 = _mm512_permutexvar_epi32(reverse, A2);


    // L1
    LA = _mm512_min_epi32(A1, A2);
    HA = _mm512_max_epi32(A1, A2);
    A1 = _mm512_permutex2var_epi32(LA, idx_L1, HA);
    A2 = _mm512_permutex2var_epi32(LA, idx_H1, HA);


    // L2
    LA = _mm512_min_epi32(A1, A2);
    HA = _mm512_max_epi32(A1, A2);
    A1 = _mm512_permutex2var_epi32(LA, idx_L2, HA);
    A2 = _mm512_permutex2var_epi32(LA, idx_H2, HA);

    // L3
    LA = _mm512_min_epi32(A1, A2);
    HA = _mm512_max_epi32(A1, A2);
    A1 = _mm512_permutex2var_epi32(LA, idx_L3, HA);
    A2 = _mm512_permutex2var_epi32(LA, idx_H3, HA);

    // L4
    LA = _mm512_min_epi32(A1, A2);
    HA = _mm512_max_epi32(A1, A2);
    A1 = _mm512_permutex2var_epi32(LA, idx_L4, HA);
    A2 = _mm512_permutex2var_epi32(LA, idx_H4, HA);

    // L5
    LA = _mm512_min_epi32(A1, A2);
    HA = _mm512_max_epi32(A1, A2);
    A1out = _mm512_permutex2var_epi32(LA, idx_L5, HA);
    A2out = _mm512_permutex2var_epi32(LA, idx_H5, HA);

    printVectorInt(A1out, "A1out");
    printVectorInt(A2out, "A2out");


    return;  
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(){


    cout << "Program Start..... " << endl << endl;  
    // int example[32] = {26, 61, 29, 47, 67, 28, 49, 35, 95, 99, 9, 20, 43, 45, 42, 42, 4, 56, 33, 72, 0, 70, 50, 4, 06, 68, 98, 43, 64, 47, 76, 48};

    int n = 256;
    int * test = (int*)aligned_alloc(64, sizeof(int) * n);
    for (int i = 0; i < n; i++){
        test[i] = i; 
    }

    aShuffle(n, test);
    aprint(n, test); 

    // bubbleSort(128,test);
    // bubbleSort(128,test+128);

    selectionSort(128,test);
    selectionSort(128,test+128);

    cout << endl; 
    aprint(64, test);
    aprint(64, test+64);
    // aprint(16, test+32);
    // aprint(16, test+48);

    __m512i A1 = _mm512_load_si512(test); 
    __m512i A2 = _mm512_load_si512(test+16);
    __m512i A1out;  
    __m512i A2out;
    // bitonicSort(A1, A2, A1out, A2out);  
    // _mm512_store_si512(&test[0], A1out);
    // _mm512_store_si512(&test[16], A2out);

    // A1 = _mm512_load_si512(test+32); 
    // A2 = _mm512_load_si512(test+48); 
    // bitonicSort(A1, A2, A1out, A2out);  
    // _mm512_store_si512(&test[32], A1out);
    // _mm512_store_si512(&test[48], A2out);

    // aprint(32, test);
    // aprint(32, test+32);


    cout << endl << "-------------------------------------------" << endl << endl;  
    int sortedBlockSize = 128;
    int startA1 = 0;
    int endA1 = startA1 + 128;
    int startA2 = sortedBlockSize;
    int endA2 = startA2+128;
    A1 = _mm512_load_si512(test); 
    A2 = _mm512_load_si512(test+sortedBlockSize);
    int * Acopy = getCopyArr(sortedBlockSize * 2, &test[startA1]);
    aprint(128, Acopy);
    aprint(128, Acopy+128);
    for (int i = 0; i < (sortedBlockSize/8)-1; i++){
        cout << i+1 << " -- ";  
        bitonicSort(A1, A2, A1out, A2out);
        cout << startA1 << ", " << startA2 << endl;  
        _mm512_store_si512(&test[i*16], A1out);
        A1 = A2out; 
        if (i == ((sortedBlockSize/8)-2) ){
            cout << "Last Iteration" << endl;  
            _mm512_store_si512(&test[i*16+16], A2out);
        }
        else if (startA1+16 == endA1) {
            cout << "If 1" << endl;  
            // Finished 1’s side but not 2’s side
            startA2 += 16;
            A2 = _mm512_load_si512(&Acopy[startA2]);
        } 
        else if (startA2+16 == endA2) {
            cout << "If 2" << endl;  
            // Finished 2's side but not 1’s side
            startA1 += 16;
            A2 = _mm512_load_si512(&Acopy[startA1]);
        } 
        else if (Acopy[startA1 + 16] < Acopy[startA2 + 16] ){
            cout << "If 3" << endl;  
            // use A1’s value
            startA1 += 16;
            A2 = _mm512_load_si512(&Acopy[startA1]); 
        } 
        else if (Acopy[startA1 + 16] >= Acopy[startA2 + 16] ){
            cout << "If 4" << endl;  
            // Store A2's source and swap A1's source with A2's source 
            startA2 += 16;
            A2 = _mm512_load_si512(&Acopy[startA2]); 
        }
        cout << startA1 << ", " << startA2 << endl;  
        aprint(128, test);
        aprint(128, test+128);
    }


    return 0;
}