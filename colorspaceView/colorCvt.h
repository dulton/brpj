#ifndef __COLOR_CVT_H__
#define __COLOR_CVT_H__

#include "math.h"



bool BasicallyEqualTo( double a, double b);
bool BasicallyEqualTo3( double a, double b, double precision);

void RGB_to_XYZ(double R,double G,double B,
				double *X,double *Y,double *Z);

void XYZ_to_RGB(double X,double Y,double Z,
				 double *R,double *G,double *B);
void RGB_to_Yxy(double R,double G,double B,
				double *Y1,double *X,double *Y2);
void Yxy_to_RGB(double *Y1,double *X,double *Y2,
				double *R,double *G,double *B);

void XYZ_to_Yxy(double x,double y,double z,
				double *Y1,double *X,double *Y2);

void Yxy_to_XYZ(double Y1,double X,double Y2,
				double *x,double *y,double *z);

#endif