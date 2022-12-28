// neuron.h
// neuron for neural_net.h
#ifndef neuron_h
#define neuron_h

#include "mvector.h"

class neuron
{
public:
	double activation;
	neuron() {
		//
	}
	~neuron() {
		//
	}
	double process(mvector<double>* weights, mvector<double>* inputs) {
		activation = 0.0;
		for (int i = 0; i < weights->getDimension(); i++) {
			activation += weights->data[i]*inputs->data[i];
		}
		activation = 1.0/(1.0 + exp(-activation));
		return activation;
	}
};

#endif