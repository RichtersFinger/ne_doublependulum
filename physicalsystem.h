// physicalsystem.h
// physical system template
#ifndef physicalsystem_h
#define physicalsystem_h

#include "mvector.h"

template <class T> class physicalsystem
{
	const int dimension = 1, pdimension = 1;
	mvector<T> * parameters;
public:
	
	physicalsystem(std::initializer_list<T> inputs) {};
	~physicalsystem() {};
	virtual mvector<T> rhs(const mvector<T>& coord){return coord;};
	
	virtual int getdimension() {return dimension;}
	virtual int getpdimension() {return pdimension;}
	virtual mvector<T> getparameters() {return mvector<T>(*parameters);}
	virtual T getparameter(int ip) {return parameters->data[ip];}
};
#endif