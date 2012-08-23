/**
*Author :Tharindra Galahena
*Project:simple Neural network simulation in c++
*Date   :24/08/2012
*/

#include "main.h"

int main(){
	read_config();
	srand(time(0));
	
	dpy = XOpenDisplay(NULL);
	int bc = BlackPixel(dpy, DefaultScreen(dpy));
	
	win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, width, height, 0, bc, bc);
	XSelectInput(dpy, win, StructureNotifyMask);
	XMapWindow(dpy, win);
	
	g1 = XCreateGC(dpy, win, 0, NULL);
	g2 = XCreateGC(dpy, win, 0, NULL);
	XSetForeground(dpy, g1, WhitePixel(dpy, DefaultScreen(dpy)));
	XSetForeground(dpy, g2, color(dpy, "red").pixel);

	object *bots = (object *)malloc(sizeof(object) * bot_count);
	object *food = (object *)malloc(sizeof(object) * food_count);
	
	for(int i = 0; i < food_count; i++){
		food[i].x = rand() % width;
		food[i].y = rand() % height;
	}
	
	for(int i = 0; i < bot_count; i++){
		bots[i].x = rand() % width;
		bots[i].y = rand() % height;
		bots[i].conx = rand() % 5 - 2;
		bots[i].cony = rand() % 5 - 2;
	}
	
	neural *net = (neural *)malloc(sizeof(neural) * bot_count);
	
	for(int i = 0; i < bot_count; i++){
		net[i] = neural(num_inputs, num_outputs, num_layers);
	} 
	
	int num_wt = net[0].get_num_weights();
	population p(bot_count , num_wt, crossover_rate, mutation_rate);
	float **w = p.genarate();
	int *fit = (int *)malloc(sizeof(int) * bot_count );
	for(int i = 0; i < bot_count ; i++) fit[i] = 1.0;
	
	int opperation_timer = 0;
	int grab_count = 0;
	
	FILE *total_grabs = fopen("result_total_grabs.txt", "w");
	FILE *best_fits   = fopen("result_best_fitnesses.txt", "w");
	
	char spins[4] = { '-', '\\', '|', '/'};
	int rolling_cursor = 0;
	int spin_count = 0;
	
	int gen_count = 0;
	cout << "\ngenaretion " << gen_count << " : " << endl;
	while(1){
		rolling_cursor++;
		if(rolling_cursor >= 50){
			cout << "\b\b";
			cout << "\t" << spins[spin_count];
			cout.flush();
			if(spin_count == 3) spin_count = 0;
			else spin_count++;
			rolling_cursor = 0;
		}
		if(opperation_timer == 2000){
			
			opperation_timer = 0;
			get_fitness_array(net, fit);
			while((w = p.new_gen(fit)) == NULL){
				p.operation(fit);
			}
			for(int i = 0; i < bot_count ; i++) net[i].dic_fitness();
			for(int i = 0; i < bot_count ; i++) net[i].put_weights(w[i]);
			
			int best_fit_index = p.cal_b_fit();
			
			fprintf(total_grabs, "%d %d\n", gen_count, grab_count);
			fflush(total_grabs);
			fprintf(best_fits, "%d %d\n", gen_count, fit[best_fit_index]);
			fflush(best_fits);
			
			cout << "\b\b";
			cout << "\t ";
			cout << endl;
			cout << " * grab count         - " << grab_count << endl;
			cout << " * best fitness index - " << best_fit_index << endl;
			cout << " * best fitness       - " << fit[best_fit_index] << endl;
			
			gen_count++;
			cout << "\ngenaretion " << gen_count << " : " << endl;
			grab_count = 0;
		}
		opperation_timer++;
		
		grab_count += check_grab(food, bots, net);
		
		update_bots(food, bots, net);
		
		
		get_fitness_array(net, fit);
		float avg = p.cal_avg_fit(fit);
		draw(food, bots, avg, fit);
		usleep(10000);
	}
}
















