#ifndef __COLOR_SPACE_CONVERT_H__
#define __COLOR_SPACE_CONVERT_H__


enum WHITE_POINT_EM
{
	WP_A=0,
		WP_B=1,
		WP_C=2,
		WP_E=3,	
		WP_D32=4,
		WP_D50=5,
		WP_D55=6,	
		WP_D56=7,
		WP_D65=8,
		WP_D75=9,
		WP_D93=10,
		WP_F1=11,
		WP_F2=12,
		WP_F3=13,
		WP_F4=14,
		WP_F5=15,
		WP_F6=16,	
		WP_F7=17,
		WP_F8=18,
		WP_F9=19,	
		WP_F10=20,
		WP_F11=21,
		WP_F12=22,
		//WP_Dcip3=23,
};

struct WHITE_POINT_ST
{
	double X;
	double Y;
	unsigned int K;
};

#define  WHITE_POINT_MAX_NUM 23


bool CCT_to_CIE_xy(double K,double *x,double *y);

void CIE_XYZ_1931_to_CIE_xy(double X,double Y,double Z,double *x,double *y,double *z);
void CIE_XYZ_1931_to_CIE_RGB(double X,double Y,double Z,double *R,double *G,double *B);


void CIE_XYZ_1931_to_CIE_1958_L(double X,double Y,double Z,double *L);
void CIE_Y_Yn1931_to_CIE_L(double Y,double Yn,double *L);
void CIE_xy_1931_to_CIE1976_upvp(double x,double y,double *up,double *vp);

void CIE_XYZ_1931_to_CIE1976_upvp(double X,double Y,double Z,double *up,double *vp);

void CIE_xy_1931_to_CIE1960_uv(double x,double y,double *u,double *v);
void CIE_XYZ_1931_to_CIE1960_uv(double X,double Y,double Z,double *u,double *v);

void CIE1976_upvp_to_CIE1960_uv(double up,double vp,double *u,double *v);
void GammaLine(double gamma,double Input,double *Output);

#endif