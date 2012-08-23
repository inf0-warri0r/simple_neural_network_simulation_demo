/**
*Author :Tharindra Galahena
*Project:simple Neural network simulation in c++
*Date   :24/08/2012
*/

#include <iostream>
#include <stdio.h>
#include <X11/Xlib.h>

#include "ga.h"
#include "neural_net.h"

#define PI 3.14159265

using namespace std;

Display *dpy;
Window win;
GC g1, g2;
int width          = 500;
int height         = 500;
int num_inputs     =   2;
int num_outputs    =   4;

int num_layers     =   4;
int bot_count      =  20;
int food_count     =  40;
int crossover_rate =  80;
int mutation_rate  =  30;

/*a bot or a food*/
typedef struct object{
	int x;
	int y;
	int conx;
	int cony;
} object;
/*read the config file and set the values*/
void read_config(){
	FILE *fp = fopen("config", "r");
	if(fp == NULL){
		printf("config file error !!\n");
		return;
	}
	int n;
	if(fscanf(fp, "number of layers=%d\n", &n) > 0)
		if(n != 0) num_layers = n;
		
	if(fscanf(fp, "number of bots=%d\n", &n) > 0)
		if(n != 0) bot_count = n;
		
	if(fscanf(fp, "number of food=%d\n", &n) > 0)
		if(n != 0) food_count = n;
	
	if(fscanf(fp, "crossover rate=%d\n", &n) > 0)
		if(n != 0) crossover_rate = n;
		
	if(fscanf(fp, "mutation rate=%d\n", &n) > 0)
		if(n != 0) mutation_rate = n;	
}
/*X11 color*/
XColor color(Display *dpy, string c){
	char *color = (char *)c.c_str();
	Visual* default_visual = DefaultVisual(dpy, DefaultScreen(dpy));
	Colormap screen_colormap = XCreateColormap(dpy,
		DefaultRootWindow(dpy),
		default_visual,
		AllocNone);
	XColor system_color;
	XColor exact_color;
	XAllocNamedColor(dpy,
		screen_colormap,
		color,
		&system_color,
		&exact_color);
	return system_color;
}
/*draw the objects*/
void draw(object *food, object *bots, float avg, int *fit){
	XClearWindow(dpy, win);
	for(int i = 0; i < bot_count; i++){
		bots[i].x += bots[i].conx;
		bots[i].y += bots[i].cony;
		if(bots[i].x > width) bots[i].x = 0;
		if(bots[i].y > height) bots[i].y = 0;
		if(bots[i].x < 0) bots[i].x = width;
		if(bots[i].y < 0) bots[i].y = height;
		if(fit[i] > avg)
			XFillRectangle(dpy, win, g2, bots[i].x - 5, bots[i].y - 5, 10, 10);
		else
			XFillRectangle(dpy, win, g1, bots[i].x - 5, bots[i].y - 5, 10, 10);
		
	}
	for(int i = 0; i < food_count; i++)
		XDrawRectangle(dpy, win, g1, food[i].x, food[i].y, 2, 2);
		
	XFlush(dpy);
}
/*decode the output of a neural into speed*/
int get_speed(float i, float j){
	if(i <= 0.5){
		if(j <= 0.5) return -2;
		else return -1;
	}else{
		if(j <= 0.5) return 1;
		else return 2;
	}
	return 0;
}
/*put all the fitnesses in to an array*/
void get_fitness_array(neural *net, int *fit){
	for(int i = 0; i < bot_count ; i++) fit[i] = net[i].get_fitness();
}
/*find the nearest */
int find_near(object *food, object bots){
	int min = sqrt(pow(width, 2) + pow(height, 2));
	int min_i = 0;
	float len;
	for(int i = 0; i < food_count; i++){
		len  = pow((bots.x - food[i].x), 2);
		len += pow((bots.y - food[i].y), 2);
		len  = sqrt(len);
		if(len < min){ 
			min = len;
			min_i = i;
		}
	}
	return min_i;
}
/*insert new speeds to the bots*/
void update_bots(object *food, object *bots, neural *net){
	for(int i = 0; i < bot_count; i++){
		float inputs[2];
		int near = find_near(food, bots[i]);
		inputs[0] = bots[i].x - food[near].x;
		inputs[1] = bots[i].y - food[near].y;
		float *output = net[i].update(inputs);
		bots[i].conx = get_speed(output[0], output[1]);
		bots[i].cony = get_speed(output[2], output[3]);
	}
}
/*check if a bot grabs a food*/
int check_grab(object *food, object *bots, neural *net){
	int grab_count = 0;
	for(int i = 0; i < bot_count; i++){
		for(int j = 0; j < food_count; j++){
			if(bots[i].x + 5 >= food[j].x && bots[i].x - 5 <= food[j].x){
				if(bots[i].y + 5 >= food[j].y && bots[i].y - 5 <= food[j].y){
					net[i].inc_fitness();
					grab_count++;
					food[j].x = (float)(rand() %  width);
					food[j].y = (float)(rand() % height);
				}
			}
		}
	}
	return grab_count;
}
