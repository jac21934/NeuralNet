#ifndef OUTPUT_H
#define OUTPUT_H

#include <iostream>
#include <iomanip>

template <typename T>
void print_matrix(T **matrix, int rows, int cols, std::ostream &stream) {
	for (int i = 0; i < rows; i++) {
		stream << "| ";

		for (int j = 0; j < cols; j++) {
			stream << std::setw(7) << matrix[i][j] << "  ";
		}

		stream << "|" << std::endl;
	}

	stream << std::endl;
}

template <typename T>
void print_vec(T *vec, int size, std::ostream &stream) {
	stream << "{ ";
	for (int i = 0; i < size; i++) {
		stream << std::setw(7) << vec[i];
		if (i < size - 1)
			stream << ", ";
	}
	stream << " }" << std::endl;
}

#endif /*OUTPUT_H*/