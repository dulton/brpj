#ifndef __COLOR_CVT_H__
#define __COLOR_CVT_H__

#include "math.h"

//http://colormine.org/

#define Epsilon (0.008856) // Intent is 216/24389
#define Kappa (903.296296) // Intent is 24389/27

#define WhiteRefX (95.047)
#define WhiteRefY (100.000)
#define WhiteRefZ (108.883)


double CubicRoot(double n);
double GetDenominator(double x,double y,double z);

bool BasicallyEqualTo( double a, double b);
bool BasicallyEqualTo3( double a, double b, double precision);


///////////////////////////////////////////////
double PivotRgb(double n);

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

void RGB_to_LUV(double R,double G,double B,
				double *L,double *U,double *V);
void XYZ_to_LUV(double x,double y,double z,
				double *L,double *U,double *V);
void LUV_to_RGB(double L,double U,double V,
				double *R,double *G,double *B);
void LUV_to_XYZ(double L,double U,double V,
				double *x,double *y,double *z);

#endif