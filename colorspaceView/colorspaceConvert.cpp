// colorspace.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "colorspaceConvert.h"
#include "math.h"

struct WHITE_POINT_ST CIE1931_2_WHITE_POINT[WHITE_POINT_MAX_NUM]=
{
	{0.44757,0.40745,2856},
	{0.34842,0.35161,4874},
	{0.31006,0.31616,6774},
	{0.33333,0.33333,5454},
	{0.42426,0.40263,3203},
	{0.34567,0.35850,5003},
	{0.33242,0.34743,5503},
	{0.33016,0.34554,5603},
	{0.31271,0.32902,6504},
	{0.29902,0.31485,7504},
	{0.28314,0.29711,9304},
	{0.31310,0.33727,6430},
	{0.37208,0.37529,4230},
	{0.40910,0.39430,3450},
	{0.44018,0.40329,2940},
	{0.31379,0.34531,6350},
	{0.37790,0.38835,4150},
	{0.31292,0.32933,6500},
	{0.34588,0.35875,5000},
	{0.37417,0.37281,4150},
	{0.34609,0.35986,5000},
	{0.38052,0.37713,4000},
	{0.43695,0.40441,3000},

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