// mvector.cpp
#include <iostream>
#include "mvector.h"

template <class T> 
mvector<T>::mvector(const int& ndim) : dimension(ndim) {
	data = new T[dimension];
	for (int i = 0; i < dimension; i++) {
		data[i] = 0;
	}
}

template <class T> 
mvector<T>::mvector(const mvector& reference) : dimension(reference.dimension) {
	data = new T[dimension];
	for (int i = 0; i < dimension; i++) {
		data[i] = reference.data[i];
	}
}

template <class T> 
mvector<T>::mvector(const int& ndim, const T* narr) : dimension(ndim) {
	data = new T[dimension];
	for (int i = 0; i < dimension; i++) {
		data[i] = narr[i];
	}
}

template <class T> 
mvector<T>::mvector(std::initializer_list<T> inputs) : dimension(inputs.size()) {
	data = new T[dimension];
	int i = 0;
	for (auto value : inputs) {
		data[i] = value;
		i++;
	}
}

template <class T> 
mvector<T>::~mvector() {
	if (!subvector) delete [] data;
}

template <class T> 
int mvector<T>::getDimension() {
	return dimension;
}

template <class T> 
mvector<T> mvector<T>::operator&(const mvector& right) {
	mvector<T> result(this->dimension + right.dimension);
	for (int i = 0; i < this->dimension; i++) {
		result.data[i] = this->data[i];
	}
	for (int i = this->dimension; i < result.dimension; i++) {
		result.data[i] = right.data[i - this->dimension];
	}
	return result;
}

template <class T> 
mvector<T> mvector<T>::operator()(const int& a, const int& b) {
	mvector<T> result(b - a + 1);
	result.subvector = true;
	delete [] result.data;
	result.data = &this->data[a];
	return result;
}

// assign up to right.dimension //
template <class T> 
mvector<T>& mvector<T>::operator=(const mvector& right) {
	for (int i = 0; i < std::min(right.dimension, this->dimension); i++) {
		this->data[i] = right.data[i];
	}
	return *this;
}

// assign up to this->dimension //
template <class T> 
mvector<T>& mvector<T>::operator=(std::initializer_list<T> inputs) {
	int i = 0;
	for (auto value : inputs) {
		data[i] = value;
		if (i == this->dimension - 1) break;
		i++;
	}
	return *this;
}

// assign constant value //
template <class T> 
mvector<T>& mvector<T>::operator=(const T& right) {
	for (int i = 0; i < this->dimension; i++) {
		this->data[i] = right;
	}
	return *this;
}

// add up to common dimension; copy remaining //
template <class T> 
mvector<T>& mvector<T>::operator+=(const mvector& right) {
	for (int i = 0; i < std::min(right.dimension, this->dimension); i++) {
		this->data[i] += right.data[i];
	}
	return *this;
}

// add constant //
template <class T> 
mvector<T>& mvector<T>::operator+=(const T& right) {
	for (int i = 0; i < this->dimension; i++) {
		this->data[i] += right;
	}
	return *this;
}

// change sign //
template <class T> 
mvector<T> mvector<T>::operator-() const {
	mvector result(this->dimension);
	for (int i = 0; i < this->dimension; i++) {
		result.data[i] = -this->data[i];
	}
	return result;
}

// subtract up to common dimension; copy remaining incl. sign //
template <class T> 
mvector<T>& mvector<T>::operator-=(const mvector& right) {
	for (int i = 0; i < std::min(right.dimension, this->dimension); i++) {
		this->data[i] -= right.data[i];
	}
	return *this;
}

// subtract constant //
template <class T> 
mvector<T>& mvector<T>::operator-=(const T& right) {
	for (int i = 0; i < this->dimension; i++) {
		this->data[i] -= right;
	}
	return *this;
}

// multiply up to common dimension; others unchanged //
template <class T> 
mvector<T>& mvector<T>::operator*=(const mvector& right) {
	for (int i = 0; i < std::min(right.dimension, this->dimension); i++) {
		this->data[i] *= right.data[i];
	}
	return *this;
}

// multiply constant //
template <class T> 
mvector<T>& mvector<T>::operator*=(const T& right) {
	for (int i = 0; i < this->dimension; i++) {
		this->data[i] *= right;
	}
	return *this;
}

// division up to common dimension; others unchanged //
template <class T> 
mvector<T>& mvector<T>::operator/=(const mvector& right) {
	for (int i = 0; i < std::min(right.dimension, this->dimension); i++) {
		this->data[i] /= right.data[i];
	}
	return *this;
}

// division constant //
template <class T> 
mvector<T>& mvector<T>::operator/=(const T& right) {
	for (int i = 0; i < this->dimension; i++) {
		this->data[i] /= right;
	}
	return *this;
}

template <class T> 
void mvector<T>::print() {
	std::cout << "(" << data[0];
	for (int i = 1; i < dimension; i++){
		std::cout << ",\t" << data[i];
	}
	std::cout << ")" << std::endl;
}

template <class T> 
void mvector<T>::writetofile(const char* filename, bool overwrite) {
	FILE * file;
	if (overwrite) 
		file = fopen(filename, "w");
	else 
		file = fopen(filename, "a+");
	
	fprintf(file, "%s\n", this->toString().c_str());
	fclose(file);
}

template<class T> 
std::string mvector<T>::toString() { 
	std::ostringstream stream; 
	stream << *this; 
	return stream.str(); 
} 















