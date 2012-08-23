/**
*Author :Tharindra Galahena
*Project:simple Neural network simulation in c++
*Date   :24/08/2012
*/

#include "ga.h"

population :: population(int s, int n, int cross, int mutation){
	crossover_rate = cross;
	mutation_rate = mutation;
	size = s;
	num = n;
	new_count = 0;
	chromosoms = (float **)malloc(sizeof(float *) * size);
	chromosoms_new = (float **)malloc(sizeof(float *) * size);
	for(int i = 0; i < size; i++){
		chromosoms[i] = (float *)malloc(sizeof(float) * num);
		chromosoms_new[i] = (float *)malloc(sizeof(float) * num);
	}
	sums = (int *)malloc(sizeof(int) * size);
	fitness = (int *)malloc(sizeof(int) * size);
}
/*genarate random set of chromosomes*/
float** population :: genarate(){
	for(int i = 0; i < size; i++){
		for(int j = 0; j < num; j++){
			chromosoms[i][j] = (float)(rand() % 1000) - 500;
		}
	}
	return chromosoms;
}
/*get sum of all fitnesses*/
int population :: get_total(){
	int sum = 0;
	for(int i = 0; i < size; i++) sum += fitness[i];
	return sum;
}
/*choose a chromosome*/
int population :: choose(){
	int i = 0;
	float ft = get_total();
	float rd = (float)(rand() % 100);
	float count = 0.0;
	for(i = 0; count < rd && i < size; i++){	
		float f;
		f = fitness[i];
		f = (f / ft) * 100.0;
		count += f;
	}
	if(i <= 0) i = 1;
	return i - 1;
}
/*mutate*/
void population :: mutate(int i1, int rate){
	int shift = rand() % 100;
	shift -= 50;
	for(int i = 0; i < num && i1 >= 0; i++){
		if(rand() % 100 < rate){ 
			chromosoms_new[i1][i] += shift;
		
		}
	}
}
/*crossover*/
void population :: cross_over(int i1, int i2, int rate){
	
	unsigned int mask = 0b11111111111111111111111111111111;
	
	int shift = rand() % 32;
	int mask1 = mask >> shift;
	int mask2 = ~mask1;
	
	if(rand() % 100 > rate && new_count < size - 1) {
		for(int i = 0; i < num; i++){
			int p1 = static_cast<unsigned int>(chromosoms[i1][i]);
			int p2 = static_cast<unsigned int>(chromosoms[i2][i]);
			chromosoms_new[new_count][i] = static_cast<float>((p1 & mask1) | (p2 & mask2));
			chromosoms_new[new_count + 1][i] = static_cast<float>((p1 & mask2) | (p2 & mask1));
		}
		new_count += 2;
	}else if(new_count < size - 1){
		for(int i = 0; i < num; i++){
			chromosoms_new[new_count][i] = chromosoms[i1][i];
			chromosoms_new[new_count + 1][i] = chromosoms[i2][i];
		}
		new_count += 2;
	}
}
/*return the array of new genaretion chromosomes*/
float** population :: new_gen(int *fit){
	int i, count = 0;
	fitness = fit;
	cal_b_fit();
	cal_w_fit();
	cal_avg_fit(fitness);
	chromosoms_new[0] = chromosoms[0];
	chromosoms_new[1] = chromosoms[1];
	if(new_count >= size - 1){
		int max1 = 1;
		int max2 = 1;
		for(int i = 0; i < size; i++){
			if(max1 < fit[i]){
				max1 = fit[i];
				chromosoms_new[0] = chromosoms[i];
			}
			if(max1 > fit[i] && max2 < fit[i]){
				max2 = fit[i];
				chromosoms_new[1] = chromosoms[i];
			}
		}
		float **tmp = chromosoms;
		chromosoms = chromosoms_new;
		chromosoms_new = chromosoms;
		new_count = 2;
		return chromosoms;
	}
	return NULL;
}
/*calculate the best fitness*/
int population :: cal_b_fit(){
	int max = 1;
	for(int i = 0; i < size; i++){
		if(max < fitness[i]){
			max = fitness[i];
			b_fit = i;
		}
	}
	return b_fit;
}
/*calculate the weast fitness*/
void population :: cal_w_fit(){
	int min = b_fit;
	for(int i = 0; i < size; i++){
		if(min > fitness[i]){
			min = fitness[i];
			w_fit = i;
		}
	}
}
/*calculate the average fitness*/
float population :: cal_avg_fit(int *fit){
	fitness = fit;
	avg_fit = (float)get_total() / (float)size;
	return avg_fit;
}
/*the process of creating two new chrosomes*/
void population :: operation(int *fit){
	
	fitness = fit;
	int i1 = choose();
	int i2 = choose();
	cross_over(i1, i2, crossover_rate);
	mutate(new_count - 2, mutation_rate);
	mutate(new_count - 1, mutation_rate);
}

