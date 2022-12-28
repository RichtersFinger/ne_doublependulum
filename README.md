# Neuroevolution: Double Pendulum Suspended from Slider
The source code provided in this repository contains a very light-weight neural network coupled into a numerical integrator (using rk4) of a double pendulum that is suspended from a slider. This simulation has been used to generate the animations in https://youtu.be/XOzv_tmvAjI . As a supplement, the gnuplot script `plot_demo.plt` can be used to test the compilation and serves as a demo for the functionality of the code.

For a more elegant formulation the new class `mvector` is included. This class allows to more compactly perform arithmetic operations on arrays and similar. Compile using, for example, `g++` with
   ```console
   $ g++ mvector.cpp main.cpp -o run -O3 
   ```
The compiled executable can be run using the following command line arguments:
* **-v**: use to enable the verbose mode where settings are printed on screen and the trajectory is written to `output.dat` 
*  **-disable_nn**: use to disable the integrated neural network component
*  **-nnlayout X .. X**: this option defines the neural networks structure, i.e. it expects the number of layers followed by the number of neurons per layer
*  **-w X .. X**: this option defines the weights of the neural network; the numbers are required to be ordered by neuron starting with the first neuron of the first layer after the input layer and continuing through a layer first
*  **-i X X X X**: this defines the initial condition of the pendulum (the slider defaults to zero velocity at the center)
*  **-p X X**: this defines the pendulum's mass and length
*  **-dt X**: sets a desired time step length; if `-v` is used, the time steps written to a file are spaced as `10dt`
*  **-duration X**: determines the duration over which setup is integrated 


By editing the contents of `doublependulum.h` one may define a completely different physical system. In particular, the equations of motion in `rhs(..)` as well as the dimensions of the phase- and parameter-space have to be adjusted accordingly. 