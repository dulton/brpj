#ifndef __COLOR_SPACE_CONVERT_H__
#define __COLOR_SPACE_CONVERT_H__

#include "math.h"

#if 0
#define CS_M_PI       3.14159265358979323846

#define RAD2DEG(xX) (180.0f/CS_M_PI * (xX))
#define DEG2RAD(xX) (CS_M_PI/180.0f * (xX))
#endif

//白点定义
#define  WHITE_POINT_MAX_NUM 24

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
	WP_Dcip3=23,	
};

struct WHITE_POINT_ST
{
	//CIE1931xy
	double x2;
	double y2;
	//CIE1964xy
	double x10;
	double y10;
	//CIE1976u'v'
	double up;
	double vp;
	//色温
	unsigned int CCT;
};


//颜色空间
#define  COLOR_SPACE_MAX_NUM 15

struct ColorSpace1931_ST
{
	//白点色温的指针
	int W;
	//色域空间的RGB对应的CIE1931xy值
	//z=1-x-y
	double Xr;
	double Yr;
	double Xg;
	double Yg;
	double Xb;
	double Yb;
};

struct ColorSpace1976_ST
{
	//白点色温的指针
	int W;
	//色域空间的RGB对应的CIE1976u'v'值
	//z=1-x-y
	double Upr;
	double Vpr;
	double Upg;
	double Vpg;
	double Upb;
	double Vpb;
};
enum ColorSpace_EM
{
	CS_CIEXYZ=0,	
	CS_sRGB_HDTVr709=1,	
	CS_AdobeRGB98=2,	
	CS_PAL_SECAM_EBU_r470BG=3,	
	CS_NTSC1987_SMPTEC=4,	
	CS_JapanNTSC=5,	
	CS_AppleRGB=6,	
	CS_NTSC1953_FCC_r470M=7,	
	CS_UHDTV_r2020=8,	
	CS_AdobeWideRGB=9,	
	CS_ROMM_ProPhotoRGB=10,	
	CS_CIERGB=11,	
	CS_ITUr601_625Line=12,	
	CS_ITUr601_525Line=13,	
	CS_DCIP3=14,	
};

//刺激值坐标
struct ColourMatchingFunctions_ST
{
	int total;
	double step;
	void  *data;
};

//刺激值坐标
struct ColourMatchingFunctions_Lite_ST
{
	double nm;
	double x;
	double y;
	double z;
};
//色度坐标
struct ChromaticityCoordinates_ST
{
	int total;
	double step;
	void  *data;
};


//色度坐标
struct ChromaticityCoordinates_Lite_ST
{
	double nm;
	double x;
	double y;
	double z;
};
////-----------------------------------------------------------
//数学

////-----------------------------------------------------------
//与配置无关的转换公式
bool CCT_to_CIE_xy(double K,	double *x,	double *y);

void CIE_XYZ_1931_to_CIE_xy(double X,	double Y,	double Z,	double *x,	double *y,	double *z);
void CIE_XYZ_1931_to_CIE_RGB(double X,	double Y,	double Z,	double *R,	double *G,	double *B);

void CIE_RGB_to_CIE_XYZ_1931(double R,double G,double B,double *X,double *Y,double *Z);
void CIE_RGB_to_CIE_xyz_1931(double r,double g,double b,double *x,double *y,double *z);

void CIE_XYZ_1931_to_CIE_1958_L(double X,	double Y,	double Z,	double *L);
void CIE_Y_Yn1931_to_CIE_L(double Y,	double Yn,	double *L);

void CIE1976_upvp_to_CIE_xy_1931(double up,double vp,double *x,double *y);
void CIE_xy_1931_to_CIE1976_upvp(double x,	double y,	double *up,	double *vp);

void CIE_XYZ_1931_to_CIE1976_upvp(double X,	double Y,	double Z,	double *up,	double *vp);

void CIE_xy_1931_to_CIE1960_uv(double x,	double y,	double *u,	double *v);
void CIE_XYZ_1931_to_CIE1960_uv(double X,	double Y,	double Z,	double *u,	double *v);

void CIE1976_upvp_to_CIE1960_uv(double up,	double vp,	double *u,	double *v);


void GammaLine(double gamma,	double Input,	double *Output);
////------------------------------------------------------------
// 用来画色度图
void gamma_correct(	double gamma, double *c);
void gamma_correct_RGB(	double gamma,  double *r, double *g, double *b);
void norm_RGB(double *r, double *g, double *b);
int constrain_RGB(double *r, double *g, double *b);
//与配置有关
void ChromaticityCoordinates_to_RGB(struct ColorSpace1931_ST cs,
									double xc, double yc, double zc,
									double *r, double *g, double *b);

double CCT_spectrum(double CCT,double wavelength);
void spectrum_to_xyz(double CCT,struct ColourMatchingFunctions_ST *cmf,
					 double *x, double *y, double *z);


#endif

