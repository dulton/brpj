#ifndef __COLOR_CVT_H__
#define __COLOR_CVT_H__

#include "math.h"

#if 0
#define CS_M_PI       3.14159265358979323846

#define RAD2DEG(xX) (180.0f/CS_M_PI * (xX))
#define DEG2RAD(xX) (CS_M_PI/180.0f * (xX))
#endif

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


void RGB_to_Lch(double r,double g,double b,
				double *L,double *C,double *H);

void Lab_to_Lch(double L1,double A,double B,
				double *L2,double *C,double *H);

void Lch_to_RGB(double L,double C,double H,
				double *r,double *g,double *b);

void Lch_to_Lab(double L1,double C,double H,
				double *L2,double *A,double *B);

void RGB_to_Lab(double r,double g,double b,
				double *L,double *A,double *B);

void XYZ_to_Lab(double X,double Y,double Z,
				double *L,double *A,double *B);

void Lab_to_RGB(double L,double A,double B,
				double *r,double *g,double *b);

void Lab_to_XYZ(double L,double A,double B,
				double *X,double *Y,double *Z);

void RGB_to_HunterLab(double r,double g,double b,
					  double *L,double *A,double *B);

void XYZ_to_HunterLab(double X,double Y,double Z,
					  double *L,double *A,double *B);

void HunterLab_to_RGB(double L,double A,double B,
					  double *r,double *g,double *b);

void HunterLab_to_XYZ(double L,double A,double B,
					  double *X,double *Y,double *Z);

//ȱRGB2HSV
void HSV_to_RGB(double H,double S,double V,
				double *R,double *G,double *B);
void Any_to_RGB(double a, double b, double c,
				double *R,double *G,double *B);
//ȱRGB2HSL
void HSL_to_RGB(double H,double S,double L,
				double *R,double *G,double *B);

double GetColorComponent(double temp1, double temp2, double temp3);
double MoveIntoRange(double temp3);

void RGB_to_CMY(double R,double G,double B,
				double *C,double *M,double *Y);
void CMY_to_RGB(double C,double M,double Y,
				double *R,double *G,double *B);

#endif