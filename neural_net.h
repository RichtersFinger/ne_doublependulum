// neural_net.h
// neural net
#ifndef neural_net_h
#define neural_net_h

#include "mvector.h"
#include "neuron.h"
#include <vector>

class neural_net
{
	mvector<double>*** weights; // 2d array of pointers, [layer][neuron]
	neuron** neurons; // 2d array of pointers, [layer][neuron]
	int nlayers; // number of layers in the network; first layer are peceptrons, then hidden layers, then output
	int ninput, noutput; // input/output dimensions
	int* nneurons; // first layer are peceptrons, then hidden layers, then output
public:
	neural_net(int _nlayers, int* _nneurons) : nlayers(_nlayers) {
		nneurons = new int[nlayers];
		for (int i = 0; i < nlayers; i++) {
			nneurons[i] = _nneurons[i];
		}
		weights = new mvector<double>**[nlayers];
		for (int i = 0; i < nlayers; i++) {
			weights[i] = new mvector<double>*[nneurons[i]];
			if (i == 0) {
				for (int j = 0; j < nneurons[i]; j++) {
					weights[i][j] = new mvector<double>(nneurons[0]);
				}
			} else {
				for (int j = 0; j < nneurons[i]; j++) {
					weights[i][j] = new mvector<double>(nneurons[i-1]);
				}
			}
		}
		neurons = new neuron*[nlayers];
		for (int i = 0; i < nlayers; i++) {
			neurons[i] = new neuron[nneurons[i]];
		}
	}
	~neural_net() {
		for (int i = 0; i < nlayers; i++) {
			if (i == 0) {
				for (int j = 0; j < nneurons[i]; j++) {
					delete weights[i][j];
				}
			} else {
				for (int j = 0; j < nneurons[i]; j++) {
					delete weights[i][j];
				}
			}
			delete [] weights[i];
		}
		delete [] weights;
		for (int i = 0; i < nlayers; i++) {
			delete [] neurons[i];
		}
		delete [] neurons;
		delete nneurons;
	}
	void initializeweights(char* cmdlineweights[]) {
		int l = 0;
		for (int i = 1; i < nlayers; i++) {
			for (int j = 0; j < nneurons[i]; j++) {
				for (int k = 0; k < nneurons[i-1]; k++) {
					weights[i][j]->data[k] = std::stof(cmdlineweights[l++]);
				}
			}
		}
	}
	mvector<double> process(mvector<double> input) {
		// perception layer
		for (int j = 0; j < nneurons[0]; j++) {
			neurons[0][j].activation = input.data[j];
		}

		// process
		for (int i = 1; i < nlayers; i++) {
			// read previous layer
			mvector<double>* activations = new mvector<double>(nneurons[i-1]);
			for (int j = 0; j < nneurons[i-1]; j++) {
				activations->data[j] = neurons[i-1][j].activation;
			}
			//activations->print();
			// execute
			for (int j = 0; j < nneurons[i]; j++) {
				neurons[i][j].process(weights[i][j], activations);
			}
			delete activations;
		}

		mvector<double> result(nneurons[nlayers-1]);
		for (int j = 0; j < nneurons[nlayers-1]; j++) {
			result.data[j] = neurons[nlayers-1][j].activation;
		}

		return result;
	}
};

#endif