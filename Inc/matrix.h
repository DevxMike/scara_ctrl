#ifndef matrix_h
#define matrix_h

#include <cmath>

class matrix {
private:
	unsigned rows;
	unsigned cols;
	double* data;
	void safe_free(void* f) {
		if (f == nullptr) {
			return;
		}
		free(f);
	}

public:
	matrix(unsigned _rows, unsigned _cols, double* init_data):
		rows{ _rows }, cols{ _cols }, data{ new double [_rows * _cols] }{
		for (unsigned i = 0; i < rows * cols; ++i) {
			data[i] = init_data[i];
		}
	}

	matrix(unsigned _rows, unsigned _cols):
		rows{ _rows }, cols{ _cols }, data{ new double [_rows * _cols] }{
		for (unsigned i = 0; i < rows * cols; ++i) {
			data[i] = 0.0;
		}
	}

	matrix(const matrix& m) {
		*this = m;
	}

	~matrix() {
		safe_free(data);
	}

	matrix& operator=(const matrix& m);

	matrix operator+(const matrix& m);
	matrix operator-()const;
	matrix operator-(const matrix& m) { return *this + (-m); }
	matrix operator*(const matrix& m);
	matrix operator+(double d);
	matrix operator-(double d) { return *this + (-d); }
	double& operator[](unsigned i){ return data[i]; }
	const double& operator[](unsigned i)const { return data[i]; }
	double& at(unsigned i, unsigned j);
	const double& at(unsigned i, unsigned j)const;
	static void element_wise_fn(matrix& m, double (*fn)(double));
	static double logsig(double x) {
		return 1.0f / (1 + exp(-x));
	}
};

#endif
