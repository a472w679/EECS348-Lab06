/**
 * Author : A472W679
 * Purpose: Preform misc. operations
 * on user-provided matrices. 
 */

#include <math.h>
#include <string>
#include <vector>
#include <fstream>
#include <numeric>
#include <iostream>
#include <algorithm>

#define MAX_SIZE 100

using namespace std;

void fillMatrix(vector<string> lines, int first, int* matrix, int dim);
int* matrixSum(const int* matrixA, const int* matrixB, int dim);
int* matrixDifference(const int* matrixA, const int* matrixB, int dim);
int* matrixProduct(const int* matrixA, const int* matrixB, int dim);
void printMatrix(const int* matrix, int dim);

int main(int argc, char **argv) {

    /**
     * Note, you can pass the name of the file 
     * you want to parse as an argument to the 
     * program. Default is "matrix_input.txt".
     */

    string inputFile = "matrix_input.txt";
    if (argc >= 2) inputFile = string(argv[1]);

    string line;
    vector<string> lines;

    ifstream file(inputFile);
    while (getline(file, line))
        lines.push_back(line);

    int dim = stoi(lines[0]);
    if (dim > MAX_SIZE) dim = MAX_SIZE;

    // Declare matrices
    int matrixA[dim][dim], 
        matrixB[dim][dim];

    // Fill matrices with file input
    fillMatrix(lines, 1      , *matrixA, dim);
    fillMatrix(lines, 1 + dim, *matrixB, dim);

    // Print header
    cout << "A472W679" << endl;
    cout << "Lab #6: Matrix manipulation\n" << endl;

    // Print matrices
    cout << "Matrix A:" << endl;
    printMatrix(*matrixA, dim);
    cout << "Matrix B:" << endl;
    printMatrix(*matrixB, dim);

    // Sum matrices
    cout << "Matrix Sum (A + B):" << endl;
    printMatrix(matrixSum(*matrixA, *matrixB, dim), dim);

    // Multiply Matrices
    cout << "Matrix Product (A * B):" << endl;
    printMatrix(matrixProduct(*matrixA, *matrixB, dim), dim);

    // Subtract matrices
    cout << "Matrix Difference (A - B):" << endl;
    printMatrix(matrixDifference(*matrixA, *matrixB, dim), dim);
}

void fillMatrix(vector<string> lines, int first, int* matrix, int dim) {

    /**
     * Convert vector of text lines starting
     * (and including) "first" index, into a
     * matrix by splitting on empty space. 
     */

    for (int i = first; i < first + dim; i++) {

        // Split lines into words
        int newPos, oldPos = 0;
        for (int j = 0; j < dim; j++) {
            
            // Get the next non-whitespace character
            newPos = lines[i].find(" ", oldPos + 1);

            // Set the appropriate matrix entry
            *(matrix + (i - first) * dim + j) = 
                stoi(lines[i].substr(oldPos, newPos));
            
            // Move string pointer along
            oldPos = newPos;
        }
    }
}

void printMatrix(const int* matrix, int dim) {
    int maxDigit[dim];        // Store max for padding
    fill_n(maxDigit, dim, 0); // Clear out garbage

    // Find longest entry for each column
    for (int i = 0; i < dim * dim; i++) {
        int target  = maxDigit[i % dim],
            current = *(matrix + i),
            digits  = log10(abs(current)) + (0 > current) + 1;
        maxDigit[i % dim] = max(target, digits);
    }
    
    // Print each entry with column padding
    for (int i = 0; i < dim * dim;) {
        bool firstCol = i % dim != 0;
        string pad = to_string(maxDigit[i % dim] + firstCol);
        printf(("%" + pad + "d").c_str(), *(matrix + i));
        if (!(++i % dim)) printf("\n");
    }
    
    // New line
    cout << endl;
}

int* matrixSum(const int* matrixA, const int* matrixB, int dim) {
    // Declare return matrix
    int* retMatrix = new int[dim * dim];

    // Sum each index into return matrix
    for (int i = 0; i < dim * dim; i++)
        *(retMatrix + i) = *(matrixA + i) + *(matrixB + i);

    return retMatrix;
}

int* matrixDifference(const int* matrixA, const int* matrixB, int dim) {
    // Declare return matrix
    int* retMatrix = new int[dim * dim];

    // Subtract value of A from B
    for (int i = 0; i < dim * dim; i++)
        *(retMatrix + i) = *(matrixA + i) - *(matrixB + i);

    return retMatrix;
}

int* matrixProduct(const int* matrixA, const int* matrixB, int dim) {

    /**
    * This uses the naive O(n^3) algorithm. If i had the time, 
    * I would implement the Strassen algorithm instead. That 
    * being said, if we actually cared about performance here,
    * we would just use a compute shader. 
    */

    // Declare return matrix
    int* retMatrix = new int[dim * dim];

    // Naive matrix multiplication
    for (int i = 0; i < dim; i++)
        for(int j = 0; j < dim; j++) {
            int sum = 0;
            for (int k = 0; k < dim; k++) 
                sum += *(matrixA + i * dim + k) *
                       *(matrixB + k * dim + j);
            *(retMatrix + i * dim + j) = sum;
        }

    return retMatrix;
}