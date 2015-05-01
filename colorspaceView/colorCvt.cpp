#include "stdafx.h"
#include "colorCvt.h"

//http://colormine.org/
//浮点判断是否相等
bool BasicallyEqualTo( double a, double b)
{
	return BasicallyEqualTo3(a,b, 0.00001);
}

bool BasicallyEqualTo3( double a, double b, double precision)
{
	return abs(a - b) <= precision;
}
////////////////----------------------------------------------------

//会把RGB扩大到1-100之间
double PivotRgb(double n)
{
	return (n > 0.04045 ? pow((n + 0.055) / 1.055, 2.4) : n / 12.92) * 100.0;
}
//http://colormine.org/
//XyzConverter.cs
//XYZ最大值如下
//      X = 95.047,
//            Y = 100.000,
//             Z = 108.883
//RGB 在0-1.0 之间
void RGB_to_XYZ(double R,double G,double B,
				double *X,double *Y,double *Z)
{
	double r = PivotRgb(R );
	double g = PivotRgb(G );
	double b = PivotRgb(B );

	// Observer. = 2, Illuminant = D65
	(*X) = r * 0.4124 + g * 0.3576 + b * 0.1805;
	(*Y) = r * 0.2126 + g * 0.7152 + b * 0.0722;
	(*Z) = r * 0.0193 + g * 0.1192 + b * 0.9505;
}

void  XYZ_to_RGB(double X,double Y,double Z,
				 double *R,double *G,double *B)
{
	// (Observer = 2, Illuminant = D65)
	double x = X / 100.0;
	double y = Y / 100.0;
	double z = Z / 100.0;

	double r = x * 3.2406 + y * -1.5372 + z * -0.4986;
	double g = x * -0.9689 + y * 1.8758 + z * 0.0415;
	double b = x * 0.0557 + y * -0.2040 + z * 1.0570;

	(*R) = r > 0.0031308 ? 1.055 * pow(r, 1 / 2.4) - 0.055 : 12.92 * r;
	(*G) = g > 0.0031308 ? 1.055 * pow(g, 1 / 2.4) - 0.055 : 12.92 * g;
	(*B) = b > 0.0031308 ? 1.055 * pow(b, 1 / 2.4) - 0.055 : 12.92 * b;
}


//http://colormine.org/
//传入的RGB在0-1之间 YXY在0-1之间
//rgb->xyz->yxy(lxy)
void RGB_to_Yxy(double R,double G,double B,
				double *Y1,double *X,double *Y2)
{
	double x,y,z;

	RGB_to_XYZ(R,G,B,&x,&y,&z);
	XYZ_to_Yxy(x,y,z,Y1,X,Y2);
}

void XYZ_to_Yxy(double x,double y,double z,
				double *Y1,double *X,double *Y2)
{
	(*Y1) = y;

	double Dividend = x+y+z;
	(*X) = BasicallyEqualTo(Dividend,0) ? 0.0 : x/ Dividend;
	(*Y2) = BasicallyEqualTo(Dividend,0) ? 0.0 : y /Dividend;
}

//传入的RGB在0-1之间 YXY在0-1之间
//yxy(lxy)->xyz->rgb
void Yxy_to_RGB(double Y1,double X,double Y2,
				double *R,double *G,double *B)
{
	double x,y,z;

	Yxy_to_XYZ(Y1,X,Y2,&x,&y,&z);
	XYZ_to_RGB(x,y,z,R,G,B);
}

void Yxy_to_XYZ(double Y1,double X,double Y2,
					double *x,double *y,double *z)
{
	(*x)= X * (Y1 / Y2);
	(*y) = Y1;
	(*z)= (1.0 - X - Y2) * (Y1 / Y2);
}