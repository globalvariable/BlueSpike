#ifndef THRESHOLDING_H
#define THRESHOLDING_H


#include <stdbool.h>

typedef struct
{
	double r_x;		
	double r_y;		
	double r_z;
} EllipsoidThreshold;

bool apply_ellipsoid_threshold(EllipsoidThreshold *thres, double x, double y, double z, double center_r_x, double center_r_y, double center_r_z);

#endif
