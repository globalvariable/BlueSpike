#include "Thresholding.h"



bool apply_ellipsoid_threshold(Ellipsoid *ellipsoid, double x, double y, double z)
{
	double res = ((x*x)/(ellipsoid->a*ellipsoid->a)) + ((y*y)/(ellipsoid->b*ellipsoid->b)) + ((z*z)/(ellipsoid->c*ellipsoid->c)) ;
	if (res > 1)
		return FALSE;
	else
		return TRUE;   // surface belongs to ellipsoid
}
