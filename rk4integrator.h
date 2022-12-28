// rk4integrator.h
// rk4 integrator
#ifndef rk4integrator_h
#define rk4integrator_h

#include "mvector.h"
#include "physicalsystem.h"

template <class T> 
class rk4integrator
{
	mvector<T>* coordinates;
public:
	physicalsystem<T>* system;
	T dt, time = 0;
	
	rk4integrator(physicalsystem<T>* _system, T _dt) : system(_system), dt(_dt) {
		coordinates = new mvector<T>(system->getdimension());
	}
	~rk4integrator() {
		delete coordinates;
	}
	void step(int nsteps) {
		mvector<T> k1(system->getdimension()), k2(system->getdimension()), k3(system->getdimension()), k4(system->getdimension());
		for(int i = 0; i < nsteps; i++) {
			k1 = system->rhs(*coordinates);
			k2 = system->rhs(*coordinates + 0.5 * dt * k1);
			k3 = system->rhs(*coordinates + 0.5 * dt * k2);
			k4 = system->rhs(*coordinates + dt * k3);
			*coordinates += dt / 6.0 * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
		}
		time += nsteps * dt;
	}
	void setcoordinates(mvector<T> coords) {
		*coordinates = coords;
	}
	mvector<T> getcoordinates() {return mvector<T>(*coordinates);}
	void writetofile(const char* filename, bool overwrite) {
		(mvector<float>({time})&*coordinates).writetofile(filename, overwrite);
	}
};

#endif