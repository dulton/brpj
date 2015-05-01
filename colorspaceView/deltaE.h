#ifndef __DELTA_E_H__
#define __DELTA_E_H__

#include "math.h"

#if 0
#define CS_M_PI       3.14159265358979323846

#define RAD2DEG(xX) (180.0f/CS_M_PI * (xX))
#define DEG2RAD(xX) (CS_M_PI/180.0f * (xX))
#endif


#define DistanceDivided(x,y) (((x)/(y))*((x)/(y)))


double math_round(double val, int places) ;
////-----------------------------------------------------------
//算各种DELTA 与配置无关
//一般传入Lab值
double delta_E_1976 (double a1,double b1,double c1,
					 double a2,double b2,double c2);
double delta_E_1994 (double L1,double a1,double b1,
					 double L2,double a2,double b2);
double delta_E_2000 (double L1,double a1,double b1,
					 double L2,double a2,double b2);
// 仅供DELTA 2000使用 算H度								 
double CIE_Lab2Hue_Only_H(double a, double b );

double delta_E_1984_CMC_IC (double L1,double a1,double b1,
							double L2,double a2,double b2,
							double lightness,double chroma);

double delta_E_1994_method2(double KL,double K1,double K2,
							double L1,double a1,double b1,
							double L2,double a2,double b2);

double delta_E_2000_method2(double L1,double a1,double b1,
							double L2,double a2,double b2);


#endif