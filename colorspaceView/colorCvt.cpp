#include "stdafx.h"
#include "colorCvt.h"

//http://colormine.org/
double CubicRoot(double n)
{
	return pow(n, 1.0 / 3.0);
}

double PivotXyz(double n)
{
	return n > Epsilon ? CubicRoot(n) : (Kappa * n + 16) / 116;
}

//会把RGB扩大到1-100之间
double PivotRgb(double n)
{
	return (n > 0.04045 ? pow((n + 0.055) / 1.055, 2.4) : n / 12.92) * 100.0;
}
double GetDenominator(double x,double y,double z)
{
	return x+ 15.0 * y + 3.0 *z;
}

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
double Max(double a, double b)
{
	return a > b ? a : b;
}

double Min(double a, double b)
{
	return a < b ? a : b;
}

////////////////----------------------------------------------------


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

void RGB_to_LUV(double R,double G,double B,
				double *L,double *U,double *V)
{
	double x,y,z;

	RGB_to_XYZ(R,G,B,&x,&y,&z);
	XYZ_to_LUV(x,y,z,L,U,V);
}

void XYZ_to_LUV(double x,double y,double z,
				double *L,double *U,double *V)
{
	double Y = y / WhiteRefY;
	(*L) = Y>Epsilon ? 116.0 * CubicRoot(Y) - 16.0 : Kappa * Y;

	double targetDenominator = GetDenominator(x,y,z);
	double referenceDenominator = GetDenominator(WhiteRefX,WhiteRefY,WhiteRefZ);
	// ReSharper disable CompareOfFloatsByEqualityOperator
	double xTarget = targetDenominator == 0 ? 0 : ((4.0 * x / targetDenominator) - (4.0 * WhiteRefX/ referenceDenominator));
	double yTarget = targetDenominator == 0 ? 0 : ((9.0 * y / targetDenominator) - (9.0 * WhiteRefY/ referenceDenominator));
	// ReSharper restore CompareOfFloatsByEqualityOperator

	(*U) = 13.0 * (*L) * xTarget;
	(*V) = 13.0 * (*L)* yTarget;
}

void LUV_to_RGB(double L,double U,double V,
				double *R,double *G,double *B)
{
	double x,y,z;

	LUV_to_XYZ(L,U,V,&x,&y,&z);
	XYZ_to_RGB(x*100,y*100,z*100,R,G,B);
}

void LUV_to_XYZ(double L,double U,double V,
				double *x,double *y,double *z)
{
	const double c = -1.0 / 3.0;
	double uPrime = (4.0 * WhiteRefX) / GetDenominator(WhiteRefX,WhiteRefY,WhiteRefZ);
	double vPrime = (9.0 * WhiteRefY) / GetDenominator(WhiteRefX,WhiteRefY,WhiteRefZ);
	double a = (1.0 / 3.0) * ((52.0 * L) / (U+ 13 * L * uPrime) - 1.0);
	double imteL_16_116 = (L+ 16.0) / 116.0;
	(*y) = L> Kappa * Epsilon
		? imteL_16_116 * imteL_16_116 * imteL_16_116
		: L / Kappa;
	double b = -5.0 * (*y);
	double d = (*y) * ((39.0 *L) / (V + 13.0 *L * vPrime) - 5.0);
	(*x) = (d - b) / (a - c);
	(*z) = (*x) * a + b;
}

//////////////////////////////
void RGB_to_Lch(double r,double g,double b,
				double *L,double *C,double *H)
{
	double L2,A,B;

	RGB_to_Lab(r,g,b,&L2,&A,&B);
	Lab_to_Lch(L2,A,B,L,C,H);

}

void Lab_to_Lch(double L1,double A,double B,
				double *L2,double *C,double *H)
{
	double h = atan2(B, A);

	// convert from radians to degrees
	if (h > 0)
	{
		h = RAD2DEG(h);
	}
	else
	{
		h = 360 - RAD2DEG(abs(h)) ;
	}

	if (h < 0)
	{
		h += 360.0;
	}
	else if (h >= 360)
	{
		h -= 360.0;
	}
	(*L2)=L1;
	(*C) = sqrt(A * A + B * B);
	(*H) = h;
}

void Lch_to_RGB(double L,double C,double H,
				double *r,double *g,double *b)
{
	double L2,A,B;

	Lch_to_Lab(L,C,H,&L2,&A,&B);
	Lab_to_RGB( L2, A, B,r,g,b);
}

void Lch_to_Lab(double L1,double C,double H,
				double *L2,double *A,double *B)
{
	double hRadians =DEG2RAD(H) ;

	(*L2)=L1;
	(*A) = cos(hRadians) * C;
	(*B) = sin(hRadians) * C;
}

void RGB_to_Lab(double r,double g,double b,
				double *L,double *A,double *B)
{
	double X,Y,Z ;
	RGB_to_XYZ(r,g,b,&X,&Y,&Z);
	XYZ_to_Lab(X,Y,Z,L,A,B);

}

void XYZ_to_Lab(double X,double Y,double Z,
				double *L,double *A,double *B)
{
	double x = PivotXyz(X /WhiteRefY);
	double y = PivotXyz(Y /WhiteRefY);
	double z = PivotXyz(Z /WhiteRefZ);

	(*L) = Max(0, 116 * y - 16);
	(*A) = 500 * (x - y);
	(*B) = 200 * (y - z);
}

void Lab_to_RGB(double L,double A,double B,
				double *r,double *g,double *b)
{
	double X,Y,Z ;

	Lab_to_XYZ(L,A,B,&X,&Y,&Z);
	XYZ_to_RGB(X,Y,Z,r,g,b);
}
void Lab_to_XYZ(double L,double A,double B,
				double *X,double *Y,double *Z)
{
	double y = (L + 16.0) / 116.0;
	double x = A / 500.0 + y;
	double z = y - B / 200.0;

	double x3 = x * x * x;
	double z3 = z * z * z;

	(*X) = WhiteRefX * (x3 > Epsilon ? x3 : (x - 16.0 / 116.0) / 7.787);
	(*Y) = WhiteRefY * (L > (Kappa * Epsilon) ? pow(((L + 16.0) / 116.0), 3) : L / Kappa);
	(*Z) = WhiteRefZ * (z3 > Epsilon ? z3 : (z - 16.0 / 116.0) / 7.787);

}

void RGB_to_HunterLab(double r,double g,double b,
					  double *L,double *A,double *B)
{
	double X,Y,Z ;

	RGB_to_XYZ(r,g,b,&X,&Y,&Z);
	XYZ_to_HunterLab(X,Y,Z,L,A,B);
}

void XYZ_to_HunterLab(double X,double Y,double Z,
					  double *L,double *A,double *B)
{
	(*L) = 10.0 * sqrt(Y);
	(*A) = Y != 0 ? 17.5 * (((1.02 * X) - Y) / sqrt(Y)) : 0;
	(*B) = Y != 0 ? 7.0 * ((Y - (.847 * Z)) / sqrt(Y)) : 0;
}

void HunterLab_to_RGB(double L,double A,double B,
					  double *r,double *g,double *b)
{
	double X,Y,Z ;

	HunterLab_to_XYZ(L,A,B,&X,&Y,&Z);
	XYZ_to_RGB(X,Y,Z,r,g,b);
}

void HunterLab_to_XYZ(double L,double A,double B,
					  double *X,double *Y,double *Z)
{
	double x = (A / 17.5) * (L / 10.0);
	double itemL_10 = L / 10.0;
	double y = itemL_10 * itemL_10;
	double z = B / 7.0 * L / 10.0;

	(*X) = (x + y) / 1.02;
	(*Y) = y;
	(*Z) = -(z - y) / 0.847;
}

// Code lovingly copied from StackOverflow (and tweaked a bit)
// Question/Answer: http://stackoverflow.com/questions/359612/how-to-change-rgb-color-to-hsv/1626175#1626175
// Submitter: Greg http://stackoverflow.com/users/12971/greg
// License: http://creativecommons.org/licenses/by-sa/3.0/
void HSV_to_RGB(double H,double S,double V,
				double *R,double *G,double *B)
{
	long range = (long)(floor(H / 60.0)) % 6;
	double f =H / 60.0 - floor(H / 60.0);

	double v = V * 255.0;
	double p = v * (1 -S);
	double q = v * (1 - f * S);
	double t = v * (1 - (1 - f) *S);

	switch (range)
	{
	case 0:
		return Any_to_RGB(v, t, p,R,G,B);
	case 1:
		return Any_to_RGB(q, v, p,R,G,B);
	case 2:
		return Any_to_RGB(p, v, t,R,G,B);
	case 3:
		return Any_to_RGB(p, q, v,R,G,B);
	case 4:
		return Any_to_RGB(t, p, v,R,G,B);
	}
	return Any_to_RGB(v, p, q,R,G,B);
}

void Any_to_RGB(double a, double b, double c,
				double *R,double *G,double *B)
{
	(*R)=a;
	(*G)=b;
	(*B)=c;
}

//RGB 在0-1.0 之间
void HSL_to_RGB(double H,double S,double L,
				double *R,double *G,double *B)
{
	double rangedH = H / 360.0;

	double s = S / 100.0;
	double L1= L / 100.0;

	if (!BasicallyEqualTo(L1,0))
	{
		if (BasicallyEqualTo(s,0))
		{
			(*R) = (*G) = (*B) = L1;
		}
		else
		{
			double temp2 = (L1 < 0.5) ? L1 * (1.0 + s) : L1 + s - (L1 * s);
			double temp1 = 2.0 * L1 - temp2;

			(*R) = GetColorComponent(temp1, temp2, rangedH + 1.0 / 3.0);
			(*G)= GetColorComponent(temp1, temp2, rangedH);
			(*B) = GetColorComponent(temp1, temp2, rangedH - 1.0 / 3.0);
		}
	}

}

double GetColorComponent(double temp1, double temp2, double temp3)
{
	temp3 = MoveIntoRange(temp3);
	if (temp3 < 1.0 / 6.0)
	{
		return temp1 + (temp2 - temp1) * 6.0 * temp3;
	}

	if (temp3 < 0.5)
	{
		return temp2;
	}

	if (temp3 < 2.0 / 3.0)
	{
		return temp1 + ((temp2 - temp1) * ((2.0 / 3.0) - temp3) * 6.0);
	}

	return temp1;
}

double MoveIntoRange(double temp3)
{
	if (temp3 < 0.0) return temp3 + 1.0;
	if (temp3 > 1.0) return temp3 - 1.0;
	return temp3;
}

//RGB 在0-1.0 之间
//CMY在0-1.0 之间
void RGB_to_CMY(double R,double G,double B,
				double *C,double *M,double *Y)
{
	(*C) = 1 - R ;
	(*M) = 1 - G;
	(*Y) = 1 - B;
}

void CMY_to_RGB(double C,double M,double Y,
				double *R,double *G,double *B)
{
	(*R) = 1 - C;
	(*G) = 1 -M;
	(*B) = 1 - Y;
}
