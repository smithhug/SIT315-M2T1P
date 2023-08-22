#include <iostream>
#include <time.h>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <pthread.h>

using namespace std::chrono;
using namespace std;

#define n 100 //define the size of the matrices

void randomise(int matrix[n][n])
{
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            // choose number between 1-100
            matrix[i][j] = rand() % 100 + 1;
            cout << matrix[i][j] << "\t";
        }
        cout << "\n";
    }
}

struct ThreadArgs { //create a struct to allow the threads to read the matrixes created in main by the randomise method
    int (*A)[n]; //A
    int (*B)[n]; //B
    int (*C)[n]; //C
    int row = 0; //Index of the row selected
};

void* thread_multiply(void* arg)
{
    ThreadArgs* args = static_cast<ThreadArgs*>(arg);

    int i = args->row;

    for (size_t j = 0; j < n; j++)
    {
        args->C[i][j] = 0;
        for (size_t k = 0; k < n; k++)
        {
            // Multiply every element of A[i][] by every element in B[][j]
            args->C[i][j] += (args->A[i][k] * args->B[k][j]);
        }
        cout << args->C[i][j] << "\t";
    }
    cout << "\n";
    return NULL;
}

void multiply(int A[n][n], int B[n][n], int C[n][n])
{
    int num_threads = n;
    pthread_t threads[num_threads]; //create a thread for each row (n)
    ThreadArgs thread_args[num_threads]; //create a struct for each thread

    for (size_t i = 0; i < num_threads; i++) //for each row create a pthread to manage it
    {
        thread_args[i].A = A;
        thread_args[i].B = B;
        thread_args[i].C = C;
        thread_args[i].row = i;
        pthread_create(&threads[i], NULL, thread_multiply, &thread_args[i]);
        
    }
    
    for (size_t i = 0; i < num_threads; i++) //join threads when done
    {
        pthread_join(threads[i], NULL);
    }

}

int main()
{
    ofstream outFile("output_parallel.txt");
    streambuf* oldCoutBuffer = cout.rdbuf(); // Save the old cout buffer
    cout.rdbuf(outFile.rdbuf());             // Redirect cout to the file

    // Get current time
    auto start = high_resolution_clock::now();

    cout << "Matrix A: \n";
    int A[n][n];
    randomise(A);

    cout << "Matrix B: \n";
    int B[n][n];
    randomise(B);

    cout << "Matrix C (A * B): \n";
    int C[n][n];

    multiply(A, B, C);

    auto stop = high_resolution_clock::now();
    // By subtracting the start time from the stop time, return the microseconds it took to process the above code
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;

    cout.rdbuf(oldCoutBuffer);   // Restore the original cout buffer

    return 0;
}
