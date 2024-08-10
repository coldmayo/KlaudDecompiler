#include <cstdlib>
#include <iostream>
#include <cmath>

#include "utils.h"

using namespace std;

std::vector<std::vector<float>> transpose(const std::vector<std::vector<float>>& matrix) {
    if (matrix.empty()) return {};

    int rows = matrix.size();
    int cols = matrix[0].size();

    std::vector<std::vector<float>> transposed(cols, std::vector<float>(rows));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            transposed[j][i] = matrix[i][j];
        }
    }

    return transposed;
}

std::vector<std::vector<float>> dot(const std::vector<std::vector<float>>& A, const std::vector<std::vector<float>>& B) {
    int rowsA = A.size();
    int colsA = A[0].size();
    int colsB = B[0].size();

    std::vector<std::vector<float>> C(rowsA, std::vector<float>(colsB, 0));


    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            for (int k = 0; k < colsA; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}

int len(float * arr) {
	return end(arr)-begin(arr);
}

float sum(float * arr, int len) {
	float su = 0;

	for (int i = 0; i < len; i++) {
		su += arr[i];
	}
	return su;
}

float max(float * arr, int len) {
	float mac = 0;

	for (int i = 0; i < len; i++) {
		if (arr[i] > mac) {
			mac = arr[i];
		}
	}
	return mac;
}

int randint(int lb, int ub) {
	return (rand() % (ub - lb + 1)) + lb << " ";
}

float * ReLU(float * x, int size) {
	float * out;
	for (int i = 0; i < size; i++) {
		if (x[i] > 0) {
			out[i] = x[i];
		} else {
			out[i] = 0;
		}
	}
	return out;
}

float * dReLU(float * x, int size) {
	float * out;
	for (int i = 0; i < size; i++) {
		if (x[i] > 0) {
			out[i] = 0;
		} else {
			out[i] = 1;
		}
	}
	return out;
}

float * softmax(float * x, int size) {
	float * e_x = exp(x - max(x, size));
	return e_x / sum(e_x, max);
}
