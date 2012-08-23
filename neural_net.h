/**
*Author :Tharindra Galahena
*Project:simple Neural network simulation in c++
*Date   :24/08/2012
*/

#ifndef NEURAL_NET_H_
#define NEURAL_NET_H_

#include <cstdlib>
#include <math.h>

using namespace std;

/*one node of the network*/
typedef struct node{
	int num_inputs;
	float *weights;
} node;

/*layer with set of nodes*/
typedef struct layer{
	int num_nodes;
	node *chr;
} layer;

class neural{
	private:
		int num_inputs;
		int num_outputs;
		int num_layers;
		int num_weights;
		layer *layers;
		int fitness;
		int total_fitness();
	public:
		neural(int in, int out, int num);
		int get_fitness(); 					//get the fitness of the network
		int inc_fitness(); 					//incress the fitness by one
		int get_num_weights(); 				//get the number of weights in the network
		float *get_weights();  				//get all weights of the network in to an array
		void put_weights(float *weights); 	//put all weights back to the network from an array
		float* update(float *inputs);		//insert inputs and get the outputs
		void dic_fitness();					//make the fitness into 1
		float convert(float input);			//encode a number in to 1 or 0
} ;

#endif
