#ifndef THRESHOLDING_H
#define THRESHOLDING_H

typedef struct
{
	double a;
	double b;
	double c;
} Ellipsoid;

bool apply_ellipsoid_threshold(Ellipsoid *ellipsoid, double x, double y, double z);

#endif
