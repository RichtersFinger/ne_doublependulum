// mvector.h
// mathematical vector class
#ifndef mvector_h
#define mvector_h

#include <complex>

template <class T> class mvector
{
  int dimension;
  bool subvector = false;
  
public:

  T* data;
  
  mvector(const int&);
  mvector(const mvector&);
  mvector(const int&, const T*);
  mvector(std::initializer_list<T> inputs);
  ~mvector();
  
  int getDimension();
  mvector operator()(const int&, const int&); // subvector
  mvector operator&(const mvector&); // concat
  mvector& operator=(const mvector&);
  mvector& operator=(std::initializer_list<T> inputs);
  mvector& operator=(const T&);
  mvector& operator+=(const mvector&);
  mvector& operator+=(const T&);
  mvector operator-() const;
  mvector& operator-=(const mvector&);
  mvector& operator-=(const T&);
  mvector& operator*=(const mvector&);
  mvector& operator*=(const T&);
  mvector& operator/=(const mvector&);
  mvector& operator/=(const T&);
  
  inline friend mvector operator+(const mvector& lhs, const mvector& rhs) {
    mvector result(std::max(lhs.dimension, rhs.dimension));
    result = lhs;
    result += rhs;
    return result;
  }
  inline friend mvector operator+(const T& lhs, const mvector& rhs) {
    mvector result(rhs.dimension);
    result = rhs;
    result += lhs;
    return result;
  }
  inline friend mvector operator+(const mvector& lhs, const T& rhs) {
    mvector result(lhs.dimension);
    result = lhs;
    result += rhs;
    return result;
  }
  inline friend mvector operator-(const mvector& lhs, const mvector& rhs) {
    mvector result(std::max(lhs.dimension, rhs.dimension));
    result = lhs;
    result -= rhs;
    return result;
  }
  inline friend mvector operator-(const T& lhs, const mvector& rhs) {
    mvector result(rhs.dimension);
    result = -rhs;
    result += lhs;
    return result;
  }
  inline friend mvector operator-(const mvector& lhs, const T& rhs) {
    mvector result(lhs.dimension);
    result = lhs;
    result -= rhs;
    return result;
  }
  inline friend mvector operator*(const mvector& lhs, const mvector& rhs) {
    mvector result(std::max(lhs.dimension, rhs.dimension));
    result = lhs;
    result *= rhs;
    if (lhs.dimension < rhs.dimension) {
	    for (int i = lhs.dimension; i < rhs.dimension; i++) {
		result.data[i] = rhs.data[i];    
	    }
    }
    return result;
  }
  inline friend mvector operator*(const T& lhs, const mvector& rhs) {
    mvector result(rhs.dimension);
    result = rhs;
    result *= lhs;
    return result;
  }
  inline friend mvector operator*(const mvector& lhs, const T& rhs) {
    mvector result(lhs.dimension);
    result = lhs;
    result *= rhs;
    return result;
  }
  inline friend mvector operator/(const mvector& lhs, const mvector& rhs) {
    mvector result(std::max(lhs.dimension, rhs.dimension));
    result = lhs;
    result /= rhs;
    if (lhs.dimension < rhs.dimension) {
	    for (int i = lhs.dimension; i < rhs.dimension; i++) {
		result.data[i] = rhs.data[i];    
	    }
    }
    return result;
  }
  inline friend mvector operator/(const T& lhs, const mvector& rhs) {
    mvector result(rhs.dimension);
    result = rhs;
    result /= lhs;
    for (int i = 0; i < result.dimension; i++) {
	    result.data[i] = 1.0f/result.data[i];
    }
    return result;
  }
  inline friend mvector operator/(const mvector& lhs, const T& rhs) {
    mvector result(lhs.dimension);
    result = lhs;
    result /= rhs;
    return result;
  }
  
  void print();
  void writetofile(const char*, bool = false);
  friend std::ostream& operator<<(std::ostream& out, const mvector& m){
    out << m.data[0];
    for(int i = 1; i < m.dimension; i++) {
	out << "\t" << m.data[i];
    }
    return out;
  }
  std::string toString();
  
};

template class mvector<int>;
template class mvector<long>;
template class mvector<long long>;
template class mvector<unsigned>;
template class mvector<unsigned long>;
template class mvector<unsigned long long>;
template class mvector<float>;
template class mvector<double>;
template class mvector<long double>;
template class mvector<std::complex<int>>;
template class mvector<std::complex<float>>;
template class mvector<std::complex<double>>;

#endif