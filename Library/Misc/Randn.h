#ifndef RANDN_H
#define RANDN_H

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

double randn_notrig(double mu, double sigma);
double randn_trig(double mu, double sigma);
double get_rand_number(void);


#endif
