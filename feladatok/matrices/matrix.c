#include "matrix.h"

#include <stdio.h>

void print_matrix(float matrix[3][1])
{

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 1; ++j) {
            printf("%4.4f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void multiply_matrices(const float a[3][3], const float b[3][1], float result[3][1]){

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 1; ++j) {
            result[i][j] = 0;
            for(int k = 0; k < 3; k++){
                result[i][j] += a[i][k] * b[k][j];
            }    
        }
    }    
}

