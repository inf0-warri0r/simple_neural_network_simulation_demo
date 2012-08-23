/**
*Author :Tharindra Galahena
*Project:simple Neural network simulation in c++
*Date   :24/08/2012
*/

#ifndef GN_H_
#define GN_H_

#include <cstdlib>

using namespace std;

class population{

	private:
		int size;
		int new_count;
		int b_fit;
		int w_fit;
		float avg_fit;
		int mutation_rate;
		int crossover_rate;
		int num;
		int *sums;
		int *fitness;
		float **chromosoms;
		float **chromosoms_new;
		int get_total();							//get sum of all fitnesses*/
		
	public:
		population(int s, int n, int cross, int mutation);
		float **genarate(); 						//genarate random set of chromosomes
		int choose();								//choose a chromosome
		void mutate(int i1, int rate);				//mutate
		void cross_over(int i1, int i2, int rate);	//crossover
		float** new_gen(int *fit);					//return the array of new genaretion chromosomes
		void operation(int *fit);					//the process of creating two new chrosomes
		int cal_b_fit();							//calculate the weast fitness
		void cal_w_fit();							//calculate the weast fitness
		float cal_avg_fit(int *fit);				//calculate the average fitness
};

#endif
