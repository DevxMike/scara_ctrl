#include "matrix.h"

matrix& matrix::operator=(const matrix& m) {
	rows = m.rows; cols = m.cols;
	safe_free(data);
	data = new double[rows * cols];

	for (unsigned i = 0; i < rows * cols; ++i) {
		data[i] = m.data[i];
	}

	return *this;
}

matrix matrix::operator+(const matrix& m) {

		matrix tmp{ *this };

		for (unsigned i = 0; i < rows * cols; ++i) {
			tmp.data[i] += m.data[i];
		}

		return tmp;

}

matrix matrix::operator-()const {
	matrix tmp{ *this };

	for (unsigned i = 0; i < rows * cols; ++i) {
		tmp.data[i] = -tmp.data[i];
	}

	return tmp;
}

matrix matrix::operator*(const matrix& m) {

		matrix tmp{ rows, m.cols };

		for (unsigned i = 0; i < rows; i++)
			for (unsigned j = 0; j < m.cols; j++) {
				auto s = 0.0;
				for (unsigned k = 0; k < cols; k++) {
					s += at(i, k) * m.at(k, j);
				}
				tmp.at(i, j) = s;
			}
		return tmp;

}

double& matrix::at(unsigned i, unsigned j) {

		return data[i * cols + j];

}

const double& matrix::at(unsigned i, unsigned j)const {

		return data[i * cols + j];

}

matrix matrix::operator+(double d) {
	matrix tmp{ *this };

	for (unsigned i = 0; i < rows * cols; ++i) {
		tmp.data[i] += d;
	}

	return tmp;
}

void matrix::element_wise_fn(matrix& m, double (*fn)(double)) {
	for (unsigned i = 0; i < m.rows * m.cols; ++i) {
		m.data[i] = fn(m.data[i]);
	}
}
