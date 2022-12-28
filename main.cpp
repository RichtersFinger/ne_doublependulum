// compile as: g++ mvector.cpp main.cpp -o run -O3
#include <iostream>
#include "mvector.h"
#include "physicalsystem.h"
#include "rk4integrator.h"
#include "doublependulum.h"
#include "neuron.h"
#include "neural_net.h"
#include <cstdlib>
#include <cstring>

int getCommandlinePos(char* argv[], int n, const char* option) {
	for (int i = 1; i < n; i++) {
		if (std::strcmp(argv[i],option) == 0) {
			return i;
		}
	}
	return -1;
}

int parseInt(char* input) {
	return std::stoi(input);
}

double parseDouble(char* input) {
	return std::stof(input);
}

double totalEnergy(rk4integrator<double>* integrator, double mass, double length) {
	double theta = integrator->getcoordinates().data[0];
	double phi = integrator->getcoordinates().data[2];
	double ptheta = integrator->getcoordinates().data[1];
	double pphi = integrator->getcoordinates().data[3];
	double xdot = integrator->getcoordinates().data[5];
	double thetadot = (pphi*cos(theta-phi) - ptheta - mass*xdot*length*(cos(phi)*cos(theta-phi) - 2.0*cos(theta)))/(mass*length*length*(pow(cos(theta-phi), 2) - 2.0));
	double phidot = (ptheta*cos(theta-phi) - 2.0 * pphi - 2.0 * mass*xdot*length*(cos(theta)*cos(theta-phi) - cos(phi)))/(mass*length*length*(pow(cos(theta-phi), 2) - 2.0));

	return 0.5*mass*(2.0*xdot*xdot + 2.0*length*length*thetadot*thetadot + 4.0*xdot*length*thetadot*cos(theta) + length*length*phidot*phidot + 2.0*xdot*length*phidot*cos(phi)+2.0*length*length*thetadot*phidot*cos(theta-phi)) - mass*9.81*length*(2.0*cos(theta) + cos(phi) - 3.0);
}

int main(int argc, char* argv[]) {
	/* #########################################
	       Process command line arguments
	############################################ */

	bool verbose = getCommandlinePos(argv, argc, "-v") > 0;
	bool useMotor = getCommandlinePos(argv, argc, "-disable_nn") < 0;

	int pos = getCommandlinePos(argv, argc, "-nnlayout");
	neural_net* neural_net_motor;		
	if (pos > 0) {
		int nlayers = parseInt(argv[pos + 1]);
		int nneurons[nlayers];
		for (int i = 0; i < nlayers; i++) {
			nneurons[i] = parseInt(argv[pos + 2 + i]);
		}

		neural_net_motor = new neural_net(nlayers, nneurons);
	} else {
		if (useMotor) {
			std::cout << "Expected some config info for neural net (-nnlayout).\n";
			return -1;
		} else {
			// generate dummy
			int nneurons[2];
			nneurons[0] = 1;
			nneurons[1] = 1;
			neural_net_motor = new neural_net(2, nneurons);		
		}
	}

	pos = getCommandlinePos(argv, argc, "-w");
	if (pos > 0) {
		neural_net_motor->initializeweights(&argv[pos + 1]);
	} else {
		if (useMotor) {
			std::cout << "No weights provided (-w).\n";
			return -1;
		}
	}

	pos = getCommandlinePos(argv, argc, "-i");
	double theta0, ptheta0, phi0, pphi0;
	if (pos > 0) {
		theta0 = parseDouble(argv[pos + 1]);
		ptheta0 = parseDouble(argv[pos + 2]);
		phi0 = parseDouble(argv[pos + 3]);
		pphi0 = parseDouble(argv[pos + 4]);
	} else {
		theta0 = 1.5;
		ptheta0 = 0.0;
		phi0 = -0.5;
		pphi0 = 0.0;
	}
	if (verbose) std::cout << "Initial condition: (" << theta0 << ", " << ptheta0 << ", " << phi0 << ", " << pphi0 << ")\n";

	pos = getCommandlinePos(argv, argc, "-p");
	double mass, length;
	if (pos > 0) {
		mass = parseDouble(argv[pos + 1]);
		length = parseDouble(argv[pos + 2]);
	} else {
		mass = 1.0;
		length = 1.0;
	}
	if (verbose) std::cout << "Parameters: (" << mass << ", " << length << ")\n";

	pos = getCommandlinePos(argv, argc, "-dt");
	double dt;
	if (pos > 0) {
		dt = parseDouble(argv[pos + 1]);
	} else {
		dt = 0.01;
	}
	if (verbose) std::cout << "dt = " << dt << "\n";

	pos = getCommandlinePos(argv, argc, "-duration");
	double duration;
	if (pos > 0) {
		duration = parseDouble(argv[pos + 1]);
	} else {
		duration = 2.0;
	}
	if (verbose) std::cout << "duration = " << duration << "\n";

	/* #########################################
	     Initialize integrator and components
	############################################ */

	doublependulum<double> system({mass, length}, neural_net_motor);
	rk4integrator<double> integrator(&system, dt);

	if (!useMotor) {
		system.active_motor = false;
		std::cout << "\033[91mWARNING: Motor disabled.\033[0m\n";
	}


	/* #########################################
	  Set initial condition and begin simulation
	############################################ */
	
	integrator.setcoordinates(mvector<double>({theta0, ptheta0, phi0, pphi0, 0.0, 0.0}));

	if (verbose) {
		mvector<double>({integrator.time, 
			integrator.getcoordinates().data[0], 
			integrator.getcoordinates().data[1], 
			integrator.getcoordinates().data[2], 
			integrator.getcoordinates().data[3],
			integrator.getcoordinates().data[4], 
			integrator.getcoordinates().data[5],
			totalEnergy(&integrator, mass, length)
		  	  }).writetofile("output.dat", true);
		for (int i = 0; i < floor(duration/integrator.dt/10); i++) {
			integrator.step(10);
			mvector<double>({integrator.time, 
				integrator.getcoordinates().data[0], 
				integrator.getcoordinates().data[1], 
				integrator.getcoordinates().data[2], 
				integrator.getcoordinates().data[3],
				integrator.getcoordinates().data[4], 
				integrator.getcoordinates().data[5],
				totalEnergy(&integrator, mass, length)
			 	   }).writetofile("output.dat", false);
		}
	} else {
		integrator.step(floor(duration/integrator.dt));
	}
	std::cout << totalEnergy(&integrator, mass, length) << std::endl;
	
	delete neural_net_motor;

	return 0;
}