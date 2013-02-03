#include "Thresholding.h"


bool apply_ellipsoid_threshold(EllipsoidThreshold *thres, double x, double y, double z, double center_r_x, double center_r_y, double center_r_z)
{
	x = x - center_r_x;
	y = y - center_r_y;
	z = z - center_r_z;
	double res = ((x*x)/(thres->r_x*thres->r_x)) + ((y*y)/(thres->r_y*thres->r_y)) + ((z*z)/(thres->r_z*thres->r_z)) ;
	if (res > 1)
		return false; // point out of elipsoid
	else
		return true;   // surface belongs to ellipsoid
}
