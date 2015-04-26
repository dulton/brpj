// colorspace.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "colorspaceConvert.h"
#include "math.h"

struct WHITE_POINT_ST White_Point_Table[WHITE_POINT_MAX_NUM]=
{
	{0.44757,	0.40745,	0.45117,	0.40594,	0.25594,	0.524332,	2856},	//a
	{0.34842,	0.35161,	0.34980,	0.35270,	0.213662,	0.485243,	4874},	//b
	{0.31006,	0.31616,	0.31039,	0.31905,	0.200868,	0.460983,	6774},	//c
	{0.33333,	0.33333,	0.33333,	0.33333,	0.210527,	0.473684,	5454},	//e
	{0.42426,	0.40263,	NULL,	NULL,	0.243023,	0.518924,	3203},	//d32
	{0.34567,	0.35850,	0.34773,	0.35952,	0.20916,	0.488073,	5003},	//d50
	{0.33242,	0.34743,	0.33411,	0.34877,	0.204375,	0.480795,	5503},	//d55
	{0.33016,	0.34554,	NULL,	NULL,	0.203609,	0.479461,	5603},	//d56
	{0.31271,	0.32902,	0.31382,	0.33100,	0.197835,	0.468326,	6504},	//d65
	{0.29902,	0.31485,	0.29968,	0.31740,	0.193495,	0.458572,	7504},	//d75
	{0.28314,	0.29711,	NULL,	NULL,	0.18879,	0.445736,	9304},	//d93
	{0.31310,	0.33727,	0.31811,	0.33559,	NULL,	NULL,	6430},	//f1
	{0.37208,	0.37529,	0.37925,	0.36733,	NULL,	NULL,	4230},	//f2
	{0.40910,	0.39430,	0.41761,	0.38324,	NULL,	NULL,	3450},	//f3
	{0.44018,	0.40329,	0.44920,	0.39074,	NULL,	NULL,	2940},	//f4
	{0.31379,	0.34531,	0.31975,	0.34246,	NULL,	NULL,	6350},	//f5
	{0.37790,	0.38835,	0.38660,	0.37847,	NULL,	NULL,	4150},	//f6
	{0.31292,	0.32933,	0.31569,	0.32960,	NULL,	NULL,	6500},	//f7
	{0.34588,	0.35875,	0.34902,	0.35939,	NULL,	NULL,	5000},	//f8
	{0.37417,	0.37281,	0.37829,	0.37045,	NULL,	NULL,	4150},	//f9
	{0.34609,	0.35986,	0.35090,	0.35444,	NULL,	NULL,	5000},	//f10
	{0.38052,	0.37713,	0.38541,	0.37123,	NULL,	NULL,	4000},	//f11
	{0.43695,	0.40441,	0.44256,	0.39717,	NULL,	NULL,	3000},	//f12
	{0.314,	0.351,	0.190765,	0.4798,	0.197835,	0.468326,	5900},	//dcip3 gamma2.6
};


//http://en.wikipedia.org/wiki/Planckian_locus
bool CCT_to_CIE_xy(double K,double *x,double *y)
{
	if(K<1667 || K>25000)
		return false;
	
	if(K>=1667 && K<4000)
	{
		(*x)=-266123900.0/(K*K*K)-234358.0/(K*K)+877.6956/K+0.17991;
	}
	else 	if(K>=4000 && K<=25000)
	{
		(*x)=-3025846900.0/(K*K*K)+2107037.9/(K*K)+222.6347/K+0.24039;
	}
	else
		return false;

	if(K>=1667 && K<2222)
	{
		(*y)=-1.1063814*(*x)*(*x)*(*x)-1.34811020*(*x)*(*x)+2.18555832*(*x)-0.20219683;
	}
	else if(K>=2222 && K<4000)
	{
		(*y)=-0.9549476*(*x)*(*x)*(*x)-1.37418593*(*x)*(*x)+2.09137015*(*x)-0.16748867;
	}
	else 	if(K>=4000 && K<=25000)
	{
		(*y)=3.0817580*(*x)*(*x)*(*x)-5.87338670*(*x)*(*x)+3.75112997*(*x)-0.37001483;
	}
	else
		return false;

	return true;
}

//色彩空间转色度
void CIE_XYZ_1931_to_CIE_xy(double X,double Y,double Z,double *x,double *y,double *z)
{
	(*x)=X/(X+Y+Z);
	(*y)=Y/(X+Y+Z);
	(*z)=1-(*x)-(*y);
}

void CIE_XYZ_1931_to_CIE_RGB(double X,double Y,double Z,double *R,double *G,double *B)
{
	(*R)=X*0.41847+Y*(-0.15866)+Z*(-0.082835);
	(*G)=X*(-0.091169)+Y*(0.25253)+Z*(0.015708);
	(*B)=X*(0.0009209)+Y*(-0.0025498)+Z*(0.1786);
}

void CIE_rgb_to_CIE_xyz_1931(double r,double g,double b,double *x,double *y,double *z)
{
	double a=0.66697*r+1.1324*g+1.20063*b;

	(*x)=(0.49*r+0.31*g+0.2*b)/a;
	(*y)=(0.17697*r+0.8124*g+0.01063*b)/a;
	(*z)=(0.01*g+0.99*b)/a;
}

void CIE_RGB_to_CIE_XYZ_1931(double R,double G,double B,double *X,double *Y,double *Z)
{
	(*X)=R*2.7689+G*(1.7517)+B*(1.1302);
	(*Y)=R*(1.0002)+G*(4.5907)+B*(0.0601);
	(*Z)=R+G*(0.0565)+B*(5.5943);
}
void CIE_RGB_to_CIE_XYZ_1964(double R,double G,double B,double *X,double *Y,double *Z)
{
	(*X)=R*0.341427+G*(0.188273)+B*(0.390202);
	(*Y)=R*(0.138972)+G*(0.837182)+B*(0.073588);
	(*Z)=R+G*(0.0375154)+B*(2.038878);
}


void CIE_XYZ_1931_to_CIE_1958_L(double X,double Y,double Z,double *L)
{
	(*L)=25.29*pow(Y,1.0/3.0)-18.38;
}


void CIE_Y_Yn1931_to_CIE_L(double Y,double Yn,double *L)
{
	double e=0.008856;

	double yr=Y/Yn;
	double f;

	if(yr>e)
		f=pow(yr,1.0/3.0);
	else 
		f=841.0/108.0*yr+4.0/29.0;

	(*L)=116*f-16;
}


void CIE_xy_1931_to_CIE1976_upvp(double x,double y,double *up,double *vp)
{
	 (*up)=4*x/(-2*x+12*y+3);
	 (*vp)=9*y/(-2*x+12*y+3);
}

void CIE_XYZ_1931_to_CIE1976_upvp(double X,double Y,double Z,double *up,double *vp)
{
	(*up)=4*X/(X+15*Y+3*Z);
	(*vp)=9*Y/(X+15*Y+3*Z);
}

void CIE_xy_1931_to_CIE1960_uv(double x,double y,double *u,double *v)
{
	(*u)=4*x/(-2*x+12*y+3);
	(*v)=6*y/(-2*x+12*y+3);
}

void CIE_XYZ_1931_to_CIE1960_uv(double X,double Y,double Z,double *u,double *v)
{
	(*u)=4*X/(X+15*Y+3*Z);
	(*v)=6*Y/(X+15*Y+3*Z);
}

void CIE1976_upvp_to_CIE1960_uv(double up,double vp,double *u,double *v)
{
	(*u)=up;
	(*v)=2.0/3.0*vp;
}

void GammaLine(double gamma,double Input,double *Output)
{
	(*Output)=pow(Input,gamma);
}
/*
int main(int argc, char* argv[])
{
	double X;double Y;double Z;
	double x;double y;double z;
double up;double vp;
double u;double v;
double L;
double L2;
#if 1
	X=0.366;
	Y=0.309;
	Z=0.587;
#else
	X=47.816;
	Y=49.299;
	Z=56.179;
#endif

CIE_XYZ_1931_to_CIE_xyz(X,Y,Z,&x,&y,&z);
CIE_xy_1931_to_CIE1976_upvp(x,y,&up,&vp);
//CIE_XYZ_1931_to_CIE_RGB(X,Y,Z,&x,&y,&z);

CIE_XYZ_1931_to_CIE1960_uv(X,Y,Z,&u,&v);
CIE_XYZ_1931_to_CIE_1958_L(X,Y,Z,&L);
CIE_Y_Yn1931_to_CIE_L(Y,104.5,&L2);

printf("%f,%f,%f %f %f %f %f %f %f\n",x,y,z,u,v,up,vp,L,L2);


	return 0;
}

*/