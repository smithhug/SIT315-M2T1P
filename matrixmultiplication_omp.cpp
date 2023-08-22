#include <iostream>
#include <time.h>
#include <chrono>
#include <cstdlib>
#include <fstream>

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

void multiply(int A[n][n], int B[n][n], int C[n][n])
{
    #pragma omp parallel
    {
        #pragma omp for
        for (size_t i = 0; i < n; i++)
        {
            #pragma omp for
            for (size_t j = 0; j < n; j++)
            {
                C[i][j] = 0;
                #pragma omp for
                for (size_t k = 0; k < n; k++)
                {
                    // Multiply every element of A[i][] by every element in B[][j]
                    C[i][j] += (A[i][k] * B[k][j]);
                }
                cout << C[i][j] << "\t";
            }
            cout << "\n";
        }
    }
}

int main()
{
    ofstream outFile("output_omp.txt");
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
