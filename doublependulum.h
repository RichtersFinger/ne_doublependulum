// doublependulum.h
// doublependulum system
#ifndef doublependulum_h
#define doublependulum_h

#include "mvector.h"
#include "physicalsystem.h"
#include "neural_net.h"

template <class T> 
class doublependulum : public physicalsystem<T>
{
	const int dimension = 6, pdimension = 2; // theta, ptheta, phi, pphi, x, xdot  // m, l
	mvector<T> * parameters;
	neural_net * motor; // provides xddot
public:
	bool active_motor = true;
	
	doublependulum(std::initializer_list<T> inputs, neural_net * _motor) : physicalsystem<T>(inputs), motor(_motor) {
		parameters = new mvector<T>(pdimension);
		int i = 0;
		for (auto value : inputs) {
			parameters->data[i] = value;
			i++;
		}
	}
	~doublependulum() {
		delete parameters;
	}
	mvector<T> rhs(const mvector<T>& coord) override {
		double slideracc;
		double sliderv = coord.data[5];
		double sliderx = coord.data[4];
		
		if (active_motor) {
			if (fabs(sliderx) < parameters->data[1]) {
				mvector<double> currentacceleration = motor->process(mvector<double>({coord.data[0], coord.data[1], coord.data[2], coord.data[3], coord.data[4], coord.data[5]}));
				slideracc = -80.0 + (80.0 + 80.0) * currentacceleration.data[0];
			} else {
				if (sliderx < -parameters->data[1]) {
					slideracc = 100.0*(1.0-sliderx - 1.0 * parameters->data[1]);
				}
				if (sliderx > parameters->data[1]) {
					slideracc = 100.0*(-1.0-sliderx + 1.0 * parameters->data[1]);
				}
			}		
		} else {
			slideracc = 0.0;
		}
	
		mvector<T> result(dimension);
		result.data[0] = (coord.data[3]*cos(coord.data[0]-coord.data[2]) - coord.data[1] - parameters->data[0]*sliderv*parameters->data[1]*(cos(coord.data[2])*cos(coord.data[0]-coord.data[2]) - 2.0*cos(coord.data[0])))
				  /(parameters->data[0]*parameters->data[1]*parameters->data[1]*(pow(cos(coord.data[0]-coord.data[2]), 2) - 2.0));
		result.data[2] = (coord.data[1]*cos(coord.data[0]-coord.data[2]) - 2.0 * coord.data[3] - 2.0 * parameters->data[0]*sliderv*parameters->data[1]*(cos(coord.data[0])*cos(coord.data[0]-coord.data[2]) - cos(coord.data[2])))
				  /(parameters->data[0]*parameters->data[1]*parameters->data[1]*(pow(cos(coord.data[0]-coord.data[2]), 2) - 2.0));

		result.data[1] = -2.0*parameters->data[0]*sliderv*parameters->data[1]*result.data[0]*sin(coord.data[0])
				 - parameters->data[0]*parameters->data[1]*parameters->data[1]*result.data[0]*result.data[2]*sin(coord.data[0] - coord.data[2])
				 - 2.0*parameters->data[0]*9.81*parameters->data[1]*sin(coord.data[0]);

		result.data[3] = -parameters->data[0]*sliderv*parameters->data[1]*result.data[2]*sin(coord.data[2])
				 + parameters->data[0]*parameters->data[1]*parameters->data[1]*result.data[0]*result.data[2]*sin(coord.data[0] - coord.data[2])
				 - parameters->data[0]*9.81*parameters->data[1]*sin(coord.data[2]);

		result.data[4] = sliderv;
		result.data[5] = slideracc;

		return result;
	}
	
	int getdimension() override {return dimension;}
	int getpdimension() override {return pdimension;}
	mvector<T> getparameters() override {return mvector<T>(*parameters);}
	T getparameter(int ip) override {return parameters->data[ip];}
	void setparameter(int ip, T newvalue) {
		parameters->data[ip] = newvalue;
	}
};

#endif