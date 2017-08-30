#ifndef OUTPUT_H
#define OUTPUT_H

#include <iostream>
#include <iomanip>

template <typename T>
void print_matrix(T **matrix, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		std::cout << "| ";

		for (int j = 0; j < cols; j++) {
			std::cout << std::setw(7) << matrix[i][j] << "  ";
		}

		std::cout << "|" << std::endl;
	}

	std::cout << std::endl;
}

template <typename T>
void print_vec(T *vec, int size) {
	std::cout << "{ ";
	for (int i = 0; i < size; i++) {
		std::cout << std::setw(7) << vec[i];
		if (i < size - 1)
			std::cout << ", ";
	}
	std::cout << " }" << std::endl;
}

#endif /*OUTPUT_H*/