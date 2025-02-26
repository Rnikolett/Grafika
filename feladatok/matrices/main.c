#include "matrix.h"

int main(int argc, char* argv[])
{
	float result[3][1];

	float a[3][3] = {
        { 0.0f, -1.0f,  0.0f},
        { 1.0f, 0.0f,  0.0f},
        {0.0f,  0.0f, 1.0f}
    };

    float b[3][1] = {
        { -3.0f},
        { 5.0f},
        {1.0f}
    };

    multiply_matrices(a, b, result);
    print_matrix(result);

	return 0;
}

