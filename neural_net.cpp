/**
*Author :Tharindra Galahena
*Project:simple Neural network simulation in c++
*Date   :24/08/2012
*/

#include "neural_net.h"

neural :: neural(int in, int out, int num){
	num_inputs = in;
	num_outputs = out;
	num_layers = num;
	num_weights = 0;
	fitness = 1;
	layers = (layer *)malloc(sizeof(layer) * num);
	layers[0].num_nodes = out;
	layers[0].chr = (node *)malloc(sizeof(node) * out);
	for(int i = 0; i < out; i++){
		(layers[0].chr[i]).num_inputs = in;
		num_weights += in + 1;
		(layers[0].chr[i]).weights = (float *)malloc(sizeof(float) * (in + 1));
	}
	for(int i = 1; i < num; i++){
		layers[i].chr = (node *)malloc(sizeof(node) * out);
		layers[i].num_nodes = out;
		for(int j = 0; j < out; j++){
			(layers[i].chr[j]).num_inputs = out;
			num_weights += out + 1;
			(layers[i].chr[j]).weights = (float *)malloc(sizeof(float) * (out + 1));
		}
	}
}
/*get the fitness of the network*/
int neural :: get_fitness(){
	return fitness;
}
/*incress the fitness by one*/
int neural :: inc_fitness(){
	fitness++;
}
/*get the number of weights in the network*/
int neural :: get_num_weights(){
	return num_weights;
}
/*get all weights of the network in to an array*/
float* neural :: get_weights(){
	float *weights = (float *)malloc(sizeof(float) * num_weights);
	int n = 0;
	for(int i = 0; i < num_layers; i++){
		for(int j = 0; j < layers[i].num_nodes; j++){
			for(int k = 0; k < (layers[i].chr[j]).num_inputs + 1; k++){
				weights[n] =  (layers[i].chr[j]).weights[k];
				n++;
			}
		}
	}
	return weights;
}
/*put all weights back to the network from an array*/
void neural :: put_weights(float *weights){
	int n = 0;
	for(int i = 0; i < num_layers; i++){
		for(int j = 0; j < layers[i].num_nodes; j++){
			for(int k = 0; k < (layers[i].chr[j]).num_inputs + 1; k++){
				(layers[i].chr[j]).weights[k] = weights[n];
				n++;
			}
		}
	}
}
/*make the fitness into 1*/
void neural :: dic_fitness(){
	fitness = 1;
}
/*insert inputs and get the outputs*/
float* neural :: update(float *inputs){
	int n = 0;
	float *outputs;
	for(int i = 0; i < num_layers; i++){
		outputs = (float *)malloc(sizeof(float) * num_outputs);
		for(int j = 0; j < layers[i].num_nodes; j++){
			float sum = 0.0;
			for(int k = 0; k < (layers[i].chr[j]).num_inputs; k++){
				sum += (layers[i].chr[j]).weights[k] * inputs[k];
			}
			outputs[j] = convert(
						sum - (layers[i].chr[j]).weights[(layers[i].chr[j]).num_inputs]
						);
		}
		inputs = outputs;
	}
	return outputs;
}
/*encode a number in to 1 or 0*/
float neural::convert(float input){
	return ( 1 / ( 1 + exp(-input)));
}
