#include "stdafx.h"
#include "deltaE.h"


//http://www.codeproject.com/Articles/243610/The-Known-Colors-Palette-Tool-Revised
//支持传入 RGB Lab 
double delta_E_1976 ( double a1,double b1,double c1,
					 double a2,double b2,double c2)
{
	double delta_a = a1-a2;
	double delta_b = b1-b2;
	double delta_c = c1-c2;

	// Eudlidean distance
	return sqrt ( delta_a*delta_a + delta_b*delta_b + delta_c*delta_c);
}

//支持传入  Lab 
// http://www.codeproject.com/Articles/243610/The-Known-Colors-Palette-Tool-Revised
double delta_E_1994 ( double L1,double a1,double b1,
					 double L2,double a2,double b2 )
{
	double C1;
	double C2;
	double CIE_1_a_squared = a1 * a1;
	double CIE_1_b_squared = b1 * b1;
	double CIE_2_a_squared = a2 * a2;
	double CIE_2_b_squared = b2 * b2;
	double delta_a;
	double delta_a_squared;
	double delta_b;
	double delta_b_squared;
	double delta_C_ab;
	double delta_C_ab_divisor;
	double delta_C_ab_squared;
	double delta_E_Lab;
	double delta_H_ab;
	double delta_H_ab_divisor;
	double delta_L;
	double delta_L_squared;
	double K_1;
	double K_2 ;

	delta_L = L1 - L2;
	delta_L_squared = delta_L * delta_L;

	delta_a = a1 - a2;
	delta_a_squared = delta_a * delta_a;

	delta_b = b1 - b2;
	delta_b_squared = delta_b * delta_b;

	delta_E_Lab = sqrt ( delta_L_squared +
		delta_a_squared +
		delta_b_squared );

	C1 = sqrt ( CIE_1_a_squared + CIE_1_b_squared );
	C2 = sqrt ( CIE_2_a_squared + CIE_2_b_squared );
	delta_C_ab = C1 - C2;
	delta_C_ab_squared = delta_C_ab * delta_C_ab;


	if ( ( delta_a_squared + delta_b_squared ) >= 
		delta_C_ab_squared )
	{                       // avoid imaginary delta_H_ab
		delta_H_ab = sqrt ( delta_a_squared + 
			delta_b_squared -
			delta_C_ab_squared );
	}
	else
	{
		delta_H_ab = 0.0;
	}
	// weighting factors for 
	// graphic arts
	// K_L = 1.0;               // => no delta_L division
	K_1 = 0.045;
	K_2 = 0.015;
	//.Textiles:
	//    K_L = 2.0;
	K_1 = .048;
	K_2 = .014;
	delta_C_ab_divisor = 1.0 + ( K_1 * C1 );
	delta_H_ab_divisor = 1.0 + ( K_2 * C1 );

	delta_C_ab /= delta_C_ab_divisor;
	delta_H_ab /= delta_H_ab_divisor;

	return ( sqrt ( delta_L_squared + 
		delta_C_ab * delta_C_ab + 
		delta_H_ab * delta_H_ab ) );
}

//http://en.wikipedia.org/wiki/Color_difference
//http://www.codeproject.com/Articles/243610/The-Known-Colors-Palette-Tool-Revised
double delta_E_2000 (  double L1,double a1,double b1,
					 double L2,double a2,double b2)
{
	double c = pow ( 25.0, 7 );
	double CIE_1_a_squared = a1 * a1;
	double CIE_1_b_squared = b1 * b1;
	double CIE_2_a_squared = a2 * a2;
	double CIE_2_b_squared = b2 * b2;
	double E00;
	double t;
	double weighting_factor_C = 1.0;
	double weighting_factor_H = 1.0;
	double weighting_factor_L = 1.0;
	double xC1;
	double xC2;
	double xCX;
	double xCY;
	double xDC;
	double xDH;
	double xDL;
	double xGX;
	double xH1;
	double xH2;
	double xHX;
	double xLX;
	double xNN;
	double xPH;
	double xRC;
	double xRT;
	double xSC;
	double xSH;
	double xSL;
	double xTX;

	xC1 = sqrt( CIE_1_a_squared + CIE_1_b_squared );
	xC2 = sqrt( CIE_2_a_squared + CIE_2_b_squared );
	xCX = ( xC1 + xC2 ) / 2.0;
	t = pow ( xCX, 7 );
	xGX = 0.5 * ( 1.0 - sqrt ( t / ( t + c ) ) );

	xNN = ( 1.0 + xGX ) * a1;
	xC1 = sqrt ( xNN * xNN + CIE_1_b_squared );
	xH1 = CIE_Lab2Hue_Only_H ( xNN, b1 );

	xNN = ( 1.0 + xGX ) * a2;
	xC2 = sqrt ( xNN * xNN + CIE_2_b_squared );
	xH2 = CIE_Lab2Hue_Only_H ( xNN, b2 );

	xDL = L2- L1;
	xDC = xC2 - xC1;
	if ( ( xC1 * xC2 ) == 0 ) 
	{
		xDH = 0.0;
	}
	else 
	{
		t = xH2 - xH1;
		xNN = math_round ( t, 12 );
		if ( abs ( xNN ) <= 180 ) 
		{
			xDH = t;
		}
		else 
		{
			if ( xNN > 180 ) 
			{
				xDH = t - 360.0;
			}
			else
			{
				xDH = t + 360.0;
			}
		}
	}
	xDH = 2.0 * sqrt ( xC1 * xC2 ) * 
		sin ( DEG2RAD ( xDH / 2.0 ) );
	xLX = ( L2-L1) / 2.0;
	xCY = ( xC1 + xC2 ) / 2.0;
	t = xH1 + xH2;
	if ( ( xC1 *  xC2 ) == 0 ) 
	{
		xHX = t;
	}
	else 
	{
		xNN = abs ( math_round ( ( xH1 - xH2 ), 12 ) );
		if ( xNN > 180 ) 
		{
			if ( t < 360.0 ) 
			{
				xHX = t + 360.0;
			}
			else
			{
				xHX = t - 360.0;
			}
		}
		else 
		{
			xHX = t;
		}
		xHX /= 2;
	}
	xTX = 1.0 - 0.17 * cos ( DEG2RAD ( xHX - 30.0 ) ) + 
		0.24 * cos ( DEG2RAD ( 2.0 * xHX ) ) + 
		0.32 * cos ( DEG2RAD ( 3.0 * xHX + 6.0 ) ) - 
		0.20 * cos ( DEG2RAD ( 4.0 * xHX - 63.0 ) );
	t = ( xHX  - 275.0 ) / 25.0;
	xPH = 30.0 * exp ( - ( t * t ) );

	t = pow ( xCY, 7 );
	xRC = 2.0 * sqrt ( t / ( t + c ) );
	t = xLX - 50.0;
	xSL = 1.0 + ( 0.015 * ( t * t ) ) /
		sqrt ( 20.0 + ( t * t ) );
	xSC = 1.0 + 0.045 * xCY;
	xSH = 1.0 + 0.015 * xCY * xTX;
	xRT = - sin ( DEG2RAD ( 2.0 * xPH ) ) * xRC;

	xDL /= ( weighting_factor_L * xSL );
	xDC /= ( weighting_factor_C * xSC );
	xDH /= ( weighting_factor_H * xSH );

	E00 = sqrt ( ( xDL * xDL ) + 
		( xDC * xDC ) + 
		( xDH * xDH ) + 
		( xRT * xDC * xDH ) );

	return ( E00 );
}

//http://en.wikipedia.org/wiki/Color_difference
//http://www.codeproject.com/Articles/243610/The-Known-Colors-Palette-Tool-Revised
// 仅供DELTA 2000使用
/// <summary>
/// helper function to return the CIE-H° value
/// </summary>
double CIE_Lab2Hue_Only_H( double a,double b )
{
	double  bias = 0.0;

	if ( ( a >= 0.0 ) && ( b == 0.0 ) ) 
	{
		return 0.0;
	}
	if ( ( a < 0.0 ) && ( b == 0.0 ) ) 
	{
		return 180.0;
	}
	if ( ( a == 0.0 ) && ( b > 0.0 ) ) 
	{
		return 90.0;
	}
	if ( ( a == 0.0 ) && ( b < 0.0 ) ) 
	{
		return 270.0;
	}
	if ( ( a > 0.0 ) && ( b > 0.0 ) ) 
	{
		bias = 0.0;
	}
	if ( a < 0.0 ) 
	{
		bias = 180.0;
	}
	if ( ( a > 0.0 ) && ( b < 0.0 ) )
	{
		bias = 360.0;
	}

	return ( DEG2RAD(atan ( b / a ) ) + bias );
}        

// http://colormine.org/
/// <summary>
/// Calculates the CMC l:c (1984) delta-e value: http://en.wikipedia.org/wiki/Color_difference#CMC_l:c_.281984.29
/// </summary>
/// <param name="colorA"></param>
/// <param name="colorB"></param>
/// <returns></returns>
double delta_E_1984_CMC_IC (double L1,double a1,double b1,
							double L2,double a2,double b2,
							double lightness=2.0,double chroma=1.0)
{

	double deltaL = L1 - L2;
	double h = atan2(b1, a1);

	double c1 = sqrt(a1 * a1 + b1 * b1);
	double c2 = sqrt(a2 * a2 + b2 * b2);
	double deltaC = c1 - c2;

	double deltaH = sqrt(
		(a1 - a2) * (a1 - a2) +
		(b1 - b2) * (b1 - b2) - 
		deltaC * deltaC);

	double c1_4 = c1 * c1;
	c1_4 *= c1_4;
	double t = 164 <= h || h >= 345
		? .56 + abs(.2 * cos(h + 168.0))
		: .36 + abs(.4 * cos(h + 35.0));
	double f = sqrt(c1_4 / (c1_4 + 1900.0));

	double sL = L1 < 16 ? .511 : (.040975 * L1) / (1.0 + .01765 * L1);
	double sC = (.0638 * c1) / (1 + .0131 * c1) + .638;
	double sH = sC * (f * t + 1 - f);

	double differences = DistanceDivided(deltaL, lightness * sL) +
		DistanceDivided(deltaC, chroma * sC) +
		DistanceDivided(deltaH, sH);

	return sqrt(differences);
}

// http://colormine.org/
/// <summary>
/// Compare colors using the Cie94 algorithm. The first color (a) will be used as the reference color.
/// </summary>
/// <param name="a">Reference color</param>
/// <param name="b">Comparison color</param>
/// <returns></returns>
enum delta_E_1994_EM
{
	deltaE_1994_ModeEM_GraphicArts=0,
	deltaE_1994_ModeEM_Textiles=1,	
};

double  delta_E_1994_Mode[2][3]=
{
	//GraphicArts	 KL,K1,K2
	{1,0.045,0.015},
	//Textiles KL,K1,K2
	{2,0.048,0.014},	
};     
double delta_E_1994_method2(double KL,double K1,double K2,
							double L1,double a1,double b1,
							double L2,double a2,double b2)
{
	double deltaL = L1 - L2;
	double deltaA = a1 - a2;
	double deltaB = b1 - b2;

	double c1 = sqrt(a1 * a1 + b1 * b1);
	double c2 = sqrt(a2 * a2 + b2 * b2);
	double deltaC = c1 - c2;

	double deltaH = deltaA * deltaA + deltaB * deltaB - deltaC * deltaC;
	deltaH = deltaH < 0 ? 0 : sqrt(deltaH);

	const double sl = 1.0;
	const double kc = 1.0;
	const double kh = 1.0;

	double sc = 1.0 + K1 * c1;
	double sh = 1.0 + K2 * c1;

	double deltaLKlsl = deltaL / (KL * sl);
	double deltaCkcsc = deltaC / (kc * sc);
	double deltaHkhsh = deltaH / (kh * sh);
	double i = deltaLKlsl * deltaLKlsl + deltaCkcsc * deltaCkcsc + deltaHkhsh * deltaHkhsh;
	return i < 0 ? 0 : sqrt(i);
}

// http://colormine.org/
double delta_E_2000_method2(double L1,double a1,double b1,
							double L2,double a2,double b2)
{
	//Set weighting factors to 1
	double k_L = 1.0;
	double k_C = 1.0;
	double k_H = 1.0;

	//Calculate Cprime1, Cprime2, Cabbar
	double c_star_1_ab = sqrt(a1 * a1 + b1 * b1);
	double c_star_2_ab = sqrt(a2 * a2 + b2 * b2);
	double c_star_average_ab = (c_star_1_ab + c_star_2_ab) / 2;

	double c_star_average_ab_pot7 = c_star_average_ab * c_star_average_ab * c_star_average_ab;
	c_star_average_ab_pot7 *= c_star_average_ab_pot7 * c_star_average_ab;

	double G = 0.5 * (1 - sqrt(c_star_average_ab_pot7 / (c_star_average_ab_pot7 + 6103515625))); //25^7
	double a1_prime = (1 + G) * a1;
	double a2_prime = (1 + G) * a2;

	double C_prime_1 = sqrt(a1_prime * a1_prime + b1 * b1);
	double C_prime_2 = sqrt(a2_prime * a2_prime + b2 * b2);
	//Angles in Degree.
	double h_prime_1 = (long)((atan2(b1, a1_prime) * 180.0 / CS_M_PI) + 360) % 360;
	double h_prime_2 = (long)((atan2(b2, a2_prime) * 180.0 / CS_M_PI) + 360) % 360;

	double delta_L_prime = L2 - L1;
	double delta_C_prime = C_prime_2 - C_prime_1;

	double h_bar = abs(h_prime_1 - h_prime_2);
	double delta_h_prime;
	if (C_prime_1 * C_prime_2 == 0) delta_h_prime = 0;
	else
	{
		if (h_bar <= 180.0)
		{
			delta_h_prime = h_prime_2 - h_prime_1;
		}
		else if (h_bar > 180.0 && h_prime_2 <= h_prime_1)
		{
			delta_h_prime = h_prime_2 - h_prime_1 + 360.0;
		}
		else
		{
			delta_h_prime = h_prime_2 - h_prime_1 - 360.0;
		}
	}
	double delta_H_prime = 2 * sqrt(C_prime_1 * C_prime_2) * sin(delta_h_prime * CS_M_PI / 360.0);

	// Calculate CIEDE2000
	double L_prime_average = (L1 + L2) / 2.0;
	double C_prime_average = (C_prime_1 + C_prime_2) / 2.0;

	//Calculate h_prime_average

	double h_prime_average;
	if (C_prime_1 * C_prime_2 == 0)
		h_prime_average = 0;
	else
	{
		if (h_bar <= 180.0)
		{
			h_prime_average = (h_prime_1 + h_prime_2) / 2;
		}
		else if (h_bar > 180.0 && (h_prime_1 + h_prime_2) < 360.0)
		{
			h_prime_average = (h_prime_1 + h_prime_2 + 360.0) / 2;
		}
		else
		{
			h_prime_average = (h_prime_1 + h_prime_2 - 360.0) / 2;
		}
	}
	double L_prime_average_minus_50_square = (L_prime_average - 50);
	L_prime_average_minus_50_square *= L_prime_average_minus_50_square;

	double S_L = 1 + ((.015 * L_prime_average_minus_50_square) / sqrt(20 + L_prime_average_minus_50_square));
	double S_C = 1 + .045 * C_prime_average;
	double T = 1
		- .17 * cos(DEG2RAD(h_prime_average - 30))
		+ .24 * cos(DEG2RAD(h_prime_average * 2))
		+ .32 * cos(DEG2RAD(h_prime_average * 3 + 6))
		- .2 * cos(DEG2RAD(h_prime_average * 4 - 63));
	double S_H = 1 + .015 * T * C_prime_average;
	double h_prime_average_minus_275_div_25_square = (h_prime_average - 275) / (25);
	h_prime_average_minus_275_div_25_square *= h_prime_average_minus_275_div_25_square;
	double delta_theta = 30 * exp(-h_prime_average_minus_275_div_25_square);

	double C_prime_average_pot_7 = C_prime_average * C_prime_average * C_prime_average;
	C_prime_average_pot_7 *= C_prime_average_pot_7 * C_prime_average;
	double R_C = 2 * sqrt(C_prime_average_pot_7 / (C_prime_average_pot_7 + 6103515625));

	double R_T = -sin(DEG2RAD(2 * delta_theta)) * R_C;

	double delta_L_prime_div_k_L_S_L = delta_L_prime / (S_L * k_L);
	double delta_C_prime_div_k_C_S_C = delta_C_prime / (S_C * k_C);
	double delta_H_prime_div_k_H_S_H = delta_H_prime / (S_H * k_H);

	double CIEDE2000 = sqrt(
		delta_L_prime_div_k_L_S_L * delta_L_prime_div_k_L_S_L
		+ delta_C_prime_div_k_C_S_C * delta_C_prime_div_k_C_S_C
		+ delta_H_prime_div_k_H_S_H * delta_H_prime_div_k_H_S_H
		+ R_T * delta_C_prime_div_k_C_S_C * delta_H_prime_div_k_H_S_H
		);

	return CIEDE2000;
}


double math_round(double val, int places) 
{
	double t;
	double f = pow(10.0, (double) places);
	double x = val * f;

	if (x >= 0.0) {
		t = ceil(x);
		if ((t - x) > 0.50000000001) {
			t -= 1.0;
		}
	} else {
		t = ceil(-x);
		if ((t + x) > 0.50000000001) {
			t -= 1.0;
		}
		t = -t; 
	}
	x = t / f;

	return (x) ? x : t;
}