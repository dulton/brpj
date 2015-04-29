#include "stdafx.h"
#include "YUV2RGB.h"




const int Table_fv1[256]={ -180, -179, -177, -176, -174, -173, -172, -170, -169, -167, -166, -165, -163, -162, -160, -159, -158, -156, -155, -153, -152, -151, -149, -148, -146, -145, -144, -142, -141, -139, -138, -137, -135, -134, -132, -131, -130, -128, -127, -125, -124, -123, -121, -120, -118, -117, -115, -114, -113, -111, -110, -108, -107, -106, -104, -103, -101, -100, -99, -97, -96, -94, -93, -92, -90, -89, -87, -86, -85, -83, -82, -80, -79, -78, -76, -75, -73, -72, -71, -69, -68, -66, -65, -64, -62, -61, -59, -58, -57, -55, -54, -52, -51, -50, -48, -47, -45, -44, -43, -41, -40, -38, -37, -36, -34, -33, -31, -30, -29, -27, -26, -24, -23, -22, -20, -19, -17, -16, -15, -13, -12, -10, -9, -8, -6, -5, -3, -2, 0, 1, 2, 4, 5, 7, 8, 9, 11, 12, 14, 15, 16, 18, 19, 21, 22, 23, 25, 26, 28, 29, 30, 32, 33, 35, 36, 37, 39, 40, 42, 43, 44, 46, 47, 49, 50, 51, 53, 54, 56, 57, 58, 60, 61, 63, 64, 65, 67, 68, 70, 71, 72, 74, 75, 77, 78, 79, 81, 82, 84, 85, 86, 88, 89, 91, 92, 93, 95, 96, 98, 99, 100, 102, 103, 105, 106, 107, 109, 110, 112, 113, 114, 116, 117, 119, 120, 122, 123, 124, 126, 127, 129, 130, 131, 133, 134, 136, 137, 138, 140, 141, 143, 144, 145, 147, 148, 150, 151, 152, 154, 155, 157, 158, 159, 161, 162, 164, 165, 166, 168, 169, 171, 172, 173, 175, 176, 178 };
const int Table_fv2[256]={ -92, -91, -91, -90, -89, -88, -88, -87, -86, -86, -85, -84, -83, -83, -82, -81, -81, -80, -79, -78, -78, -77, -76, -76, -75, -74, -73, -73, -72, -71, -71, -70, -69, -68, -68, -67, -66, -66, -65, -64, -63, -63, -62, -61, -61, -60, -59, -58, -58, -57, -56, -56, -55, -54, -53, -53, -52, -51, -51, -50, -49, -48, -48, -47, -46, -46, -45, -44, -43, -43, -42, -41, -41, -40, -39, -38, -38, -37, -36, -36, -35, -34, -33, -33, -32, -31, -31, -30, -29, -28, -28, -27, -26, -26, -25, -24, -23, -23, -22, -21, -21, -20, -19, -18, -18, -17, -16, -16, -15, -14, -13, -13, -12, -11, -11, -10, -9, -8, -8, -7, -6, -6, -5, -4, -3, -3, -2, -1, 0, 0, 1, 2, 2, 3, 4, 5, 5, 6, 7, 7, 8, 9, 10, 10, 11, 12, 12, 13, 14, 15, 15, 16, 17, 17, 18, 19, 20, 20, 21, 22, 22, 23, 24, 25, 25, 26, 27, 27, 28, 29, 30, 30, 31, 32, 32, 33, 34, 35, 35, 36, 37, 37, 38, 39, 40, 40, 41, 42, 42, 43, 44, 45, 45, 46, 47, 47, 48, 49, 50, 50, 51, 52, 52, 53, 54, 55, 55, 56, 57, 57, 58, 59, 60, 60, 61, 62, 62, 63, 64, 65, 65, 66, 67, 67, 68, 69, 70, 70, 71, 72, 72, 73, 74, 75, 75, 76, 77, 77, 78, 79, 80, 80, 81, 82, 82, 83, 84, 85, 85, 86, 87, 87, 88, 89, 90, 90 };
const int Table_fu1[256]={ -44, -44, -44, -43, -43, -43, -42, -42, -42, -41, -41, -41, -40, -40, -40, -39, -39, -39, -38, -38, -38, -37, -37, -37, -36, -36, -36, -35, -35, -35, -34, -34, -33, -33, -33, -32, -32, -32, -31, -31, -31, -30, -30, -30, -29, -29, -29, -28, -28, -28, -27, -27, -27, -26, -26, -26, -25, -25, -25, -24, -24, -24, -23, -23, -22, -22, -22, -21, -21, -21, -20, -20, -20, -19, -19, -19, -18, -18, -18, -17, -17, -17, -16, -16, -16, -15, -15, -15, -14, -14, -14, -13, -13, -13, -12, -12, -11, -11, -11, -10, -10, -10, -9, -9, -9, -8, -8, -8, -7, -7, -7, -6, -6, -6, -5, -5, -5, -4, -4, -4, -3, -3, -3, -2, -2, -2, -1, -1, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 14, 14, 14, 15, 15, 15, 16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 19, 20, 20, 20, 21, 21, 22, 22, 22, 23, 23, 23, 24, 24, 24, 25, 25, 25, 26, 26, 26, 27, 27, 27, 28, 28, 28, 29, 29, 29, 30, 30, 30, 31, 31, 31, 32, 32, 33, 33, 33, 34, 34, 34, 35, 35, 35, 36, 36, 36, 37, 37, 37, 38, 38, 38, 39, 39, 39, 40, 40, 40, 41, 41, 41, 42, 42, 42, 43, 43 };
const int Table_fu2[256]={ -227, -226, -224, -222, -220, -219, -217, -215, -213, -212, -210, -208, -206, -204, -203, -201, -199, -197, -196, -194, -192, -190, -188, -187, -185, -183, -181, -180, -178, -176, -174, -173, -171, -169, -167, -165, -164, -162, -160, -158, -157, -155, -153, -151, -149, -148, -146, -144, -142, -141, -139, -137, -135, -134, -132, -130, -128, -126, -125, -123, -121, -119, -118, -116, -114, -112, -110, -109, -107, -105, -103, -102, -100, -98, -96, -94, -93, -91, -89, -87, -86, -84, -82, -80, -79, -77, -75, -73, -71, -70, -68, -66, -64, -63, -61, -59, -57, -55, -54, -52, -50, -48, -47, -45, -43, -41, -40, -38, -36, -34, -32, -31, -29, -27, -25, -24, -22, -20, -18, -16, -15, -13, -11, -9, -8, -6, -4, -2, 0, 1, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 21, 23, 24, 26, 28, 30, 31, 33, 35, 37, 39, 40, 42, 44, 46, 47, 49, 51, 53, 54, 56, 58, 60, 62, 63, 65, 67, 69, 70, 72, 74, 76, 78, 79, 81, 83, 85, 86, 88, 90, 92, 93, 95, 97, 99, 101, 102, 104, 106, 108, 109, 111, 113, 115, 117, 118, 120, 122, 124, 125, 127, 129, 131, 133, 134, 136, 138, 140, 141, 143, 145, 147, 148, 150, 152, 154, 156, 157, 159, 161, 163, 164, 166, 168, 170, 172, 173, 175, 177, 179, 180, 182, 184, 186, 187, 189, 191, 193, 195, 196, 198, 200, 202, 203, 205, 207, 209, 211, 212, 214, 216, 218, 219, 221, 223, 225 };



bool YV12_to_RGB24(unsigned char* pYV12, unsigned char* pRGB24,int iWidth, int iHeight)
{
	if(!pYV12 || !pRGB24)
		return false;

	const long nYLen = long( iWidth* iHeight);
	const int nHfWidth = (iWidth>>1);

	if(nYLen<1 || nHfWidth<1)
		return false;

	// Y data
	unsigned char* yData = pYV12;
	// v data
	unsigned char* vData = &yData[nYLen];
	// u data
	unsigned char* uData = &vData[nYLen>>2];

	if(!uData || !vData)
		return false;

	int rgb[3];
	int i, j, m, n, x, y;
	m = -iWidth;
	n = -nHfWidth;

	bool addhalf = true;
	for(y=0; y<iHeight;y++) {
		m += iWidth;

		if(!(y % 2))
			n += nHfWidth;

		for(x=0; x<iWidth;x++)  {
			i = m + x;
			j = n + (x>>1);
			/*
			py = yData[i];

			// search tables to get rdif invgdif and bidif
			rdif = Table_fv1[vData[j]];    // fv1
			invgdif = Table_fu1[uData[j]] + Table_fv2[vData[j]]; // fu1+fv2
			bdif = Table_fu2[uData[j]]; // fu2

			rgb[2] = py+rdif;    // R
			rgb[1] = py-invgdif; // G
			rgb[0] = py+bdif;    // B
			*/
			/*
			rdif = v + ( ( v  *  103 )  >>  8 );
			invgdif = ( ( u  *  88 )  >>  8 )  + ( ( v  *  183 )  >>  8 );
			bdif = u +(  ( u * 198 )  >>  8 );
			*/


			// search tables to get rdif invgdif and bidif
			rgb[2] =yData[i]+ Table_fv1[vData[j]];    // fv1
			rgb[1] =yData[i]+ Table_fu1[uData[j]] + Table_fv2[vData[j]]; // fu1+fv2
			rgb[0] =yData[i]+Table_fu2[uData[j]]; // fu2

			j = nYLen - iWidth - m + x;
			i = (j<<1) + j;

			// copy this pixel to rgb data
			for(j=0; j<3; j++)
			{
				if(rgb[j]>=0 && rgb[j]<=255){
					pRGB24[i + j] = rgb[j];
				}
				else{
					pRGB24[i + j] = (rgb[j] < 0)? 0 : 255;
				}
			}
		}
	}
	return true;
}


bool I420_to_RGB24(unsigned char* pI420, unsigned char* pRGB24,int iWidth, int iHeight)
{
	if(!pI420 || !pRGB24)
		return false;

	const long nYLen = long( iWidth* iHeight);
	const int nHfWidth = (iWidth>>1);

	if(nYLen<1 || nHfWidth<1)
		return false;

	// Y data
	unsigned char* yData = pI420;
	// v data
	unsigned char* uData = &yData[nYLen];
	// u data
	unsigned char* vData = &uData[nYLen>>2];

	if(!uData || !vData)
		return false;

	int rgb[3];
	int i, j, m, n, x, y,  py, rdif, invgdif, bdif;
	m = -iWidth;
	n = -nHfWidth;

	bool addhalf = true;
	for(y=0; y<iHeight;y++) {
		m += iWidth;
		if( addhalf ){
			n+=nHfWidth;
			addhalf = false;
		} else {
			addhalf = true;
		}
		for(x=0; x<iWidth;x++)  {
			i = m + x;
			j = n + (x>>1);

			py = yData[i];

			// search tables to get rdif invgdif and bidif
			rdif = Table_fv1[vData[j]];    // fv1
			invgdif = Table_fu1[uData[j]] + Table_fv2[vData[j]]; // fu1+fv2
			bdif = Table_fu2[uData[j]]; // fu2

			rgb[2] = py+rdif;    // R
			rgb[1] = py-invgdif; // G
			rgb[0] = py+bdif;    // B

			j = nYLen - iWidth - m + x;
			i = (j<<1) + j;

			// copy this pixel to rgb data
			for(j=0; j<3; j++)
			{
				if(rgb[j]>=0 && rgb[j]<=255){
					pRGB24[i + j] = rgb[j];
				}
				else{
					pRGB24[i + j] = (rgb[j] < 0)? 0 : 255;
				}
			}
		}
	}
	return true;
}

//YV12转RGB24
bool YV12toRGB24UD(unsigned char* pYV12, unsigned char* pRGB24, int iWidth, int iHeight,unsigned char *RGBW)
{
	if(!pYV12 || !pRGB24)
		return false;

	const long nYLen = long(iHeight * iWidth);
	const int nHfWidth = (iWidth>>1);

	if(nYLen<1 || nHfWidth<1) 
		return false;

	// yv12数据格式，其中Y分量长度为width * height, U和V分量长度都为width * height / 4
	// |WIDTH |
	// y......y--------
	// y......y   HEIGHT
	// y......y
	// y......y--------
	// v..v
	// v..v
	// u..u
	// u..u
	unsigned char* yData = pYV12;
	unsigned char* vData = &yData[nYLen];
	unsigned char* uData = &vData[nYLen>>2];

	if(!uData || !vData)
		return false;

	// Convert YV12 to RGB24
	// 
	// formula
	//                                       [1            1                        1             ]
	// [r g b] = [y u-128 v-128] [0            0.34375             0             ]
	//                                       [1.375      0.703125          1.734375]
	// another formula
	//                                       [1                   1                      1            ]
	// [r g b] = [y u-128 v-128] [0                   0.698001         0            ]
	//                                       [1.370705      0.703125         1.732446]
	int rgb[3];
	int i, j, m, n, x, y;
	m = -iWidth;
	n = -nHfWidth;
	for(y=0; y < iHeight; y++)
	{
		m += iWidth;
		if(!(y % 2))
			n += nHfWidth;
		for(x=0; x < iWidth; x++)
		{
			i = m + x;
			j = n + (x>>1);
			rgb[0] = int(yData[i] + 1.370705 * (vData[j] - 128)); // r分量值
			rgb[1] = int(yData[i] - 0.698001 * (uData[j] - 128)  - 0.703125 * (vData[j] - 128)); // g分量值
			rgb[2] = int(yData[i] + 1.732446 * (uData[j] - 128)); // b分量值

			j = nYLen - iWidth - m + x;
			i = (j<<1) + j;
			for(j=0; j<3; j++)
			{
				if(rgb[j]>=0 && rgb[j]<=255)
					pRGB24[i + j] = rgb[j];
				else
					pRGB24[i + j] = (rgb[j] < 0) ? 0 : 255;
			}
		}
	}

	int tempw=iWidth*3;
	int halfh=iHeight/2;
	for(y=0; y <= halfh; y++)
	{
		memcpy(RGBW,&pRGB24[y*tempw],tempw);
		memcpy(&pRGB24[y*tempw],&pRGB24[(iHeight-1-y)*tempw],tempw);
		memcpy(&pRGB24[(iHeight-1-y)*tempw],RGBW,tempw);
	}

	return true;
}

//
bool  RGB2YUV(LPBYTE RgbBuf,UINT nWidth,UINT nHeight,LPBYTE yuvBuf,unsigned long *len)  
{  
	int i, j;  
	unsigned char*bufY, *bufU, *bufV, *bufRGB,*bufYuv;  
	memset(yuvBuf,0,(unsigned int )*len);  
	bufY = yuvBuf;  
	bufV = yuvBuf + nWidth * nHeight;  
	bufU = bufV + (nWidth * nHeight* 1/4);  
	*len = 0;   
	unsigned char y, u, v, r, g, b,testu,testv;  
	unsigned int ylen = nWidth * nHeight;  
	unsigned int ulen = (nWidth * nHeight)/4;  
	unsigned int vlen = (nWidth * nHeight)/4;   
	for (j = 0; j<nHeight;j++)  
	{  
		//bufRGB = RgbBuf + nWidth * (nHeight - 1 - j) * 3 ;  
		bufRGB = RgbBuf + nWidth * (  j) * 3 ;  
		for (i = 0;i<nWidth;i++)  
		{  
			int pos = nWidth * i + j;  
			r = *(bufRGB++);  
			g = *(bufRGB++);  
			b = *(bufRGB++);  
			y = (unsigned char)( ( 66 * r + 129 * g +  25 * b + 128) >> 8) + 16  ;            
			u = (unsigned char)( ( -38 * r -  74 * g + 112 * b + 128) >> 8) + 128 ;            
			v = (unsigned char)( ( 112 * r -  94 * g -  18 * b + 128) >> 8) + 128 ;  
			*(bufY++) = max( 0, min(y, 255 ));  
			if (j%2==0&&i%2 ==0)  
			{  
				if (u>255)  
				{  
					u=255;  
				}  
				if (u<0)  
				{  
					u = 0;  
				}  
				*(bufU++) =u;  
				//存u分量  
			}  
			else  
			{  
				//存v分量  
				if (i%2==0)  
				{  
					if (v>255)  
					{  
						v = 255;  
					}  
					if (v<0)  
					{  
						v = 0;  
					}  
					*(bufV++) =v;  
				}  
			}  
		}  
	}  
	*len = nWidth * nHeight+(nWidth * nHeight)/2;  
	return true;  
}   

//测试写入BMP文件
void ZOGDramBMP(char *path,unsigned char *buffer, int w, int h)
{
	FILE *file_out;
	int len;
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
	memset( &bfh, 0, sizeof( bfh ) );
	memset( &bih, 0, sizeof( bih ) );

	file_out = fopen( path, "wb" );

	if( file_out == 0 )
	{
		return;
	}
	len = w*h*3;
	bfh.bfType = 'MB';
	bfh.bfSize = sizeof( bfh ) + len + sizeof( BITMAPINFOHEADER );
	bfh.bfOffBits = sizeof( BITMAPINFOHEADER ) + sizeof( BITMAPFILEHEADER );
	fwrite( &bfh, sizeof(bfh), 1, file_out );

	bih.biSize = sizeof( bih );
	bih.biWidth = w;
	bih.biHeight = -h;
	bih.biPlanes = 1;
	bih.biBitCount = 8*3;

	fwrite( &bih, sizeof(bih), 1, file_out );
	fwrite( buffer, len, 1, file_out );
	fclose(file_out);
}

//测试写入BMP文件
void RGB32toRGB24(char *path,unsigned char *buffer, int w, int h)
{
	FILE *file_out;
	int len;
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
	memset( &bfh, 0, sizeof( bfh ) );
	memset( &bih, 0, sizeof( bih ) );

	file_out = fopen( path, "wb" );

	if( file_out == 0 )
	{
		return;
	}
	len = w*h*3;
	bfh.bfType = 'MB';
	bfh.bfSize = sizeof( bfh ) + len + sizeof( BITMAPINFOHEADER );
	bfh.bfOffBits = sizeof( BITMAPINFOHEADER ) + sizeof( BITMAPFILEHEADER );
	fwrite( &bfh, sizeof(bfh), 1, file_out );

	bih.biSize = sizeof( bih );
	bih.biWidth = w;
	bih.biHeight = -h;
	bih.biPlanes = 1;
	bih.biBitCount = 8*3;

	fwrite( &bih, sizeof(bih), 1, file_out );
	fwrite( buffer, len, 1, file_out );
	fclose(file_out);
}

//62ms
void YV122RGB(unsigned char *yuv_image,unsigned char* rgb_image,int iWidth, int iHeight)
{

	int    i,j,y,u,v,r,g,b,rdif,invgdif,bdif;
	int    YPOS=0,UPOS=0,VPOS=0;
	long int step;

	const long nYLen = long( iWidth* iHeight);
	const int nHfWidth = (iWidth>>1);

	unsigned char* yData = yuv_image;
	unsigned char* vData = &yData[nYLen];
	unsigned char* uData = &vData[nYLen>>2];


	for(i=0; i<iHeight; i++)
	{
		for (j=0; j<iWidth; j++)
		{
			y=*(( unsigned char *)&yData[YPOS]);
			u=*(( unsigned char *)&vData[UPOS])-128;
			v=*(( unsigned char *)&uData[VPOS])-128;

			rdif = v + ((v * 103) >> 8);
			invgdif = ((u * 88) >> 8) +((v * 183) >> 8);
			bdif = u +( (u*198) >> 8);

			r = y + rdif;
			g = y - invgdif;
			b = y + bdif;

			step=(i*iWidth + j)*3 ;

			rgb_image[step + 2] =b>255?255:(b<0?0:b);
			rgb_image[step + 1] =g>255?255:(g<0?0:g);
			rgb_image[step + 0] =r>255?255:(r<0?0:r);

			YPOS++;      
			if(j & 0x01)  //u,v是二分之一y
			{
				UPOS++;
				VPOS++;      
			} 
		}
		if((i & 0x01)== 0)  //u,v是二分之一y
		{
			UPOS -= iWidth>>1;
			VPOS -= iWidth>>1;
		}
	} 
}

void YUV4202RGB(unsigned char *yuv_image,unsigned char* rgb_image,int iWidth, int iHeight)
{

	int    i,j,y,u,v,r,g,b,rdif,invgdif,bdif;
	int    YPOS=0,UPOS=0,VPOS=0;
	long int step;

	const long nYLen = long( iWidth* iHeight);
	const int nHfWidth = (iWidth>>1);

	unsigned char* yData = yuv_image;
	unsigned char* vData = &yData[nYLen];
	unsigned char* uData = &vData[nYLen>>2];


	for(i=0; i<iHeight; i++)
	{
		for (j=0; j<iWidth; j++)
		{
			y=*(( unsigned char *)&yData[YPOS]);
			u=*(( unsigned char *)&vData[UPOS])-128;
			v=*(( unsigned char *)&uData[VPOS])-128;

			rdif = v + ((v * 103) >> 8);
			invgdif = ((u * 88) >> 8) +((v * 183) >> 8);
			bdif = u +( (u*198) >> 8);

			r = y + rdif;
			g = y - invgdif;
			b = y + bdif;

			step=(i*iWidth + j)*3 ;

			rgb_image[step + 0] =b>255?255:(b<0?0:b);
			rgb_image[step + 1] =g>255?255:(g<0?0:g);
			rgb_image[step + 2] =r>255?255:(r<0?0:r);

			YPOS++;      
			if(j & 0x01)  //u,v是二分之一y
			{
				UPOS++;
				VPOS++;      
			} 
		}
		if((i & 0x01)== 0)  //u,v是二分之一y
		{
			UPOS -= iWidth>>1;
			VPOS -= iWidth>>1;
		}
	} 
}
#if 0
/************************************************************************
Function CompressRGBToJPEG:RGB图像数据压缩为JPEG文件保存
Input:
VIDEO_FRAMEINFO *yuv_image        YUV图像数据
unsigned char* rgb_image			RGB图像数据
return:
无   
************************************************************************/
void YUV2RGB(VIDEO_FRAMEINFO *yuv_image,unsigned char* rgb_image)
{
	int    stride = yuv_image->stridey;
	int    i,j,y,u,v,r,g,b,rdif,invgdif,bdif;
	int    YPOS=0,UPOS=0,VPOS=0;
	long int step;

	for(i=0; i<yuv_image->height; i++)
	{
		for (j=0; j<yuv_image->width; j++)
		{
			y=*(( unsigned char *)&yuv_image->pY[YPOS]);
			u=*(( unsigned char *)&yuv_image->pU[UPOS])-128;
			v=*(( unsigned char *)&yuv_image->pV[VPOS])-128;

			rdif = v + ((v * 103) >> 8);
			invgdif = ((u * 88) >> 8) +((v * 183) >> 8);
			bdif = u +( (u*198) >> 8);

			r = y + rdif;
			g = y - invgdif;
			b = y + bdif;

			step=(i*yuv_image->width + j)*3 ;

			rgb_image[step + 0] =b>255?255:(b<0?0:b);
			rgb_image[step + 1] =g>255?255:(g<0?0:g);
			rgb_image[step + 2] =r>255?255:(r<0?0:r);

			YPOS++;      
			if(j & 0x01)  //u,v是二分之一y
			{
				UPOS++;
				VPOS++;      
			} 
		}
		if((i & 0x01)== 0)  //u,v是二分之一y
		{
			UPOS -= yuv_image->width>>1;
			VPOS -= yuv_image->width>>1;
		}
	} 
}
#endif

/************************************************************************
Function CompressRGBToJPEG:RGB图像数据压缩为JPEG文件保存
Input:BYTE *lpImageRGB        RGB图像数据区
int   originalWidth     原始图像宽度
int   originalHeight    原始图像高度
int   jpegQuality       JPEG压缩质量[1-100]
char* jpegFileName      JPEG文件保存的名称 
int   isNeedReversal    是否需要翻转
int   isResizeImage     是否需要缩放图像的尺寸
int RGBflag; 1为RGB 0为BGR
return:
成功返回0值,异常返回其它整数;
Update:
Author         Date              Ver      Remark  
Shimingjie     2005/01/06        1.0      Create         
************************************************************************/
int CompressRGBToJPEG(unsigned char *lpImageRGB,int originalWidth,int originalHeight,
					  int RGBflag,int jpegQuality,char* jpegFileName,int isNeedReversal,int isResizeImage)
{
	int res = 0;
	int jpegImageWidth;
	int jpegImageHeight;
	IJLERR jerr;
	JPEG_CORE_PROPERTIES jcprops;

	jerr = ijlInit(&jcprops);
	if (jerr != IJL_OK)
	{
		res = 1;
		goto Exit;
	}

	if (isResizeImage == 0) //保持原始比例
	{
		jpegImageWidth = originalWidth;
		jpegImageHeight = originalHeight;
	}
	// else if (isResizeImage == 112) //宽度变为1/2，高度不变
	// {
	//  jpegImageWidth = originalWidth/2;
	//  jpegImageHeight = originalHeight;
	// }
	else //保持原始比例
	{
		jpegImageWidth = originalWidth;
		jpegImageHeight = originalHeight;
	}

	// Setup DIB
	jcprops.DIBWidth         = originalWidth;
	if (isNeedReversal == 0) //如果不需要翻转图片
	{
		jcprops.DIBHeight        = originalHeight;
	}
	else                     //如果需要翻转图片
	{
		jcprops.DIBHeight        = -originalHeight;
	}
	jcprops.DIBBytes         = lpImageRGB;
	if(RGBflag)
		jcprops.DIBColor         = IJL_RGB;
	else
		jcprops.DIBColor         = IJL_BGR;
	jcprops.DIBChannels      = 3;
	jcprops.DIBPadBytes      = IJL_DIB_PAD_BYTES(jcprops.DIBWidth,3);

	// Setup JPEG
	jcprops.JPGFile          = jpegFileName;
	jcprops.JPGWidth         = jpegImageWidth;
	jcprops.JPGHeight        = jpegImageHeight;
	jcprops.jquality         = jpegQuality;
	jcprops.JPGColor         = IJL_YCBCR;
	jcprops.JPGChannels      = 3;
	jcprops.JPGSubsampling   = IJL_411;

	jerr = ijlWrite(&jcprops,IJL_JFILE_WRITEWHOLEIMAGE);
	if(IJL_OK != jerr)
	{
		res = 2;
		goto Exit;
	}

Exit:
	jerr = ijlFree(&jcprops);
	if(IJL_OK != jerr)
	{
		res = 3;
	}

	return res;
}

int CompressRGBAToJPEG(unsigned char *lpImageRGB,int originalWidth,int originalHeight,
					  int jpegQuality,char* jpegFileName,int isNeedReversal,int isResizeImage)
{
	int res = 0;
	int jpegImageWidth;
	int jpegImageHeight;
	IJLERR jerr;
	JPEG_CORE_PROPERTIES jcprops;

	jerr = ijlInit(&jcprops);
	if (jerr != IJL_OK)
	{
		res = 1;
		goto Exit;
	}

	if (isResizeImage == 0) //保持原始比例
	{
		jpegImageWidth = originalWidth;
		jpegImageHeight = originalHeight;
	}
	// else if (isResizeImage == 112) //宽度变为1/2，高度不变
	// {
	//  jpegImageWidth = originalWidth/2;
	//  jpegImageHeight = originalHeight;
	// }
	else //保持原始比例
	{
		jpegImageWidth = originalWidth;
		jpegImageHeight = originalHeight;
	}

	// Setup DIB
	jcprops.DIBWidth         = originalWidth;
	if (isNeedReversal == 0) //如果不需要翻转图片
	{
		jcprops.DIBHeight        = originalHeight;
	}
	else                     //如果需要翻转图片
	{
		jcprops.DIBHeight        = -originalHeight;
	}
	jcprops.DIBBytes         = lpImageRGB;
	jcprops.DIBColor         = IJL_RGBA_FPX;
	jcprops.DIBChannels      = 4;
	jcprops.DIBPadBytes      = IJL_DIB_PAD_BYTES(jcprops.DIBWidth,4);

	// Setup JPEG
	jcprops.JPGFile          = jpegFileName;
	jcprops.JPGWidth         = jpegImageWidth;
	jcprops.JPGHeight        = jpegImageHeight;
	jcprops.jquality         = jpegQuality;
	jcprops.JPGColor         = IJL_YCBCR;
	jcprops.JPGChannels      = 3;
	jcprops.JPGSubsampling   = IJL_411;

	jerr = ijlWrite(&jcprops,IJL_JFILE_WRITEWHOLEIMAGE);
	if(IJL_OK != jerr)
	{
		res = 2;
		goto Exit;
	}

Exit:
	jerr = ijlFree(&jcprops);
	if(IJL_OK != jerr)
	{
		res = 3;
	}

	return res;
}

/************************************************************************
Function EncodeToJPEGBuffer:RGB图像数据压缩为JPEG文件保存
Input:
BYTE *lpRawBuffer			RGB图像数据区
DWORD   dwWidth			原始图像宽度
DWORD   dwWidth			原始图像高度
BYTE *lpJPEGBuffer		JPEG图像数据区
DWORD *lpjpgBufferSize	JPEG图像大小

return:
成功返回0值,异常返回其它整数;   
************************************************************************/
BOOL EncodeToJPEGBuffer(
						BYTE *lpRawBuffer, 
						DWORD dwWidth, 
						DWORD dwHeight,  
						BYTE *lpJPEGBuffer,
						DWORD *lpjpgBufferSize)
{
	BOOL	bres;
	IJLERR	jerr = IJL_OK;
	DWORD	rawBufSize;
	JPEG_CORE_PROPERTIES jcprops;

	bres = TRUE;
	jerr = ijlInit(&jcprops);
	if(jerr != IJL_OK){
		printf("INIT IJL ERR -- %s\n", ijlErrorStr(jerr));
		bres = FALSE;
		return bres;
	}

	rawBufSize = dwWidth * dwHeight * 3;

	jcprops.DIBWidth = dwWidth;
	jcprops.DIBHeight = dwHeight;
	jcprops.DIBBytes = lpRawBuffer;
	jcprops.DIBPadBytes = IJL_DIB_PAD_BYTES(jcprops.DIBWidth,3);
	jcprops.DIBChannels = 3;
	jcprops.DIBColor =IJL_BGR;
	//jcprops.DIBSubsampling = IJL_422;

	jcprops.JPGWidth = dwWidth;
	jcprops.JPGHeight = dwHeight;
	jcprops.JPGFile = NULL;
	jcprops.JPGBytes = lpJPEGBuffer;
	jcprops.JPGSizeBytes = rawBufSize;
	jcprops.JPGChannels = 3;
	jcprops.JPGColor = IJL_YCBCR;
	jcprops.JPGSubsampling = IJL_411;
	jcprops.jquality = 80;

	jerr = ijlWrite(&jcprops, IJL_JBUFF_WRITEWHOLEIMAGE);
	if(jerr != IJL_OK){
		printf("ijlWrite() failed -- %s\n", ijlErrorStr(jerr));
		bres = FALSE;
		goto ENCODE_ERROR;
	}else{
		*lpjpgBufferSize = jcprops.JPGSizeBytes;
	}
ENCODE_ERROR:
	ijlFree(&jcprops);
	return bres;
}


BOOL JPEGFile2BMP(char* lpszFilename,
				  unsigned char* RGBdata, int *w,int *h,int *channel,long *wstep,long *size)   
{   
	IJLERR ijlErr;   

	JPEG_CORE_PROPERTIES m_jcprops;
	memset(&m_jcprops,0,sizeof(JPEG_CORE_PROPERTIES));   

	// Initialize the IntelR JPEG Library.   
	ijlErr = ijlInit(&m_jcprops);   

	if(IJL_OK != ijlErr)   
	{   
		printf("INIT IJL ERR -- %s\n", ijlErrorStr(ijlErr));
		return FALSE;   
	}   

	m_jcprops.JPGFile = lpszFilename;   
	ijlErr = ijlRead(&m_jcprops,IJL_JFILE_READPARAMS);   

	if(IJL_OK != ijlErr)   
	{   
		printf("ijlRead IJL ERR -- %s\n", ijlErrorStr(ijlErr));
		return FALSE;   
	}   

	(*w) = m_jcprops.JPGWidth;   
	(*h) = m_jcprops.JPGHeight;   
	(*channel) = m_jcprops.JPGChannels;   

	// Compute DIB padding   
	long dib_line_width = (*w)* (*channel);   
	long dib_pad_bytes = IJL_DIB_PAD_BYTES(*w,*channel);   
	// Compute size of desired pixel buffer.   
	(*size)= ( dib_line_width + dib_pad_bytes ) * (*h);   
	(*wstep)= dib_line_width + dib_pad_bytes;

	// Set up the info on the desired DIB properties.   
	m_jcprops.DIBWidth = (*w);   
	m_jcprops.DIBHeight = (*h); // Implies a bottom-up DIB.   
	m_jcprops.DIBChannels = (*channel);   
	m_jcprops.DIBColor = IJL_BGR;   
	m_jcprops.DIBPadBytes = dib_pad_bytes;   
	m_jcprops.DIBBytes = RGBdata;   
	// Set the JPG color space ... this will always be   
	// somewhat of an educated guess at best because JPEG   
	// is "color blind" (i.e., nothing in the bit stream   
	// tells you what color space the data was encoded from).   
	// However, in this example we assume that we are   
	// reading JFIF files which means that 3 channel images   
	// are in the YCbCr color space and 1 channel images are   
	// in the Y color space.   
	switch(m_jcprops.JPGChannels)   
	{   
	case 1:   
		{   
			m_jcprops.JPGColor = IJL_G;   
			break;   
		}   
	case 3:   
		{   
			m_jcprops.JPGColor = IJL_YCBCR;   
			break;   
		}   
	default:   
		{   
			// This catches everything else, but no   
			// color twist will be performed by the IJL.   
			m_jcprops.DIBColor = (IJL_COLOR)IJL_OTHER;   
			m_jcprops.JPGColor = (IJL_COLOR)IJL_OTHER;   
			break;   
		}   
	}   
	// Now get the actual JPEG image data into the pixel buffer.   
	ijlErr = ijlRead(&m_jcprops,IJL_JFILE_READWHOLEIMAGE);   

	if(IJL_OK != ijlErr)   
	{   
		printf("ijlRead2 IJL ERR -- %s\n", ijlErrorStr(ijlErr));
		return FALSE;          
	}   

	ijlFree(&m_jcprops);   

	return TRUE;   
}   


BOOL JPEGData2BMP(unsigned char *Jpegdata,long JpegSize,
				  unsigned char* RGBdata, int *w,int *h,int *channel,long *wstep,long *size)   
{   
	IJLERR ijlErr;   

	JPEG_CORE_PROPERTIES m_jcprops;
	memset(&m_jcprops,0,sizeof(JPEG_CORE_PROPERTIES));   

	// Initialize the IntelR JPEG Library.   
	ijlErr = ijlInit(&m_jcprops);   

	if(IJL_OK != ijlErr)   
	{   
		printf("INIT IJL ERR -- %s\n", ijlErrorStr(ijlErr));
		return FALSE;   
	}   

	m_jcprops.JPGBytes=Jpegdata;   
	m_jcprops.JPGSizeBytes=JpegSize;   
	ijlErr = ijlRead(&m_jcprops,IJL_JBUFF_READPARAMS);   

	if(IJL_OK != ijlErr)   
	{   
		printf("ijlRead IJL ERR -- %s\n", ijlErrorStr(ijlErr));
		return FALSE;   
	}   

	(*w) = m_jcprops.JPGWidth;   
	(*h) = m_jcprops.JPGHeight;   
	(*channel) = m_jcprops.JPGChannels;   

	// Compute DIB padding   
	long dib_line_width = (*w)* (*channel);   
	long dib_pad_bytes = IJL_DIB_PAD_BYTES(*w,*channel);   
	// Compute size of desired pixel buffer.   
	(*size)= ( dib_line_width + dib_pad_bytes ) * (*h);   
	(*wstep)= dib_line_width + dib_pad_bytes;

	// Set up the info on the desired DIB properties.   
	m_jcprops.DIBWidth = (*w);   
	m_jcprops.DIBHeight = (*h); // Implies a bottom-up DIB.   
	m_jcprops.DIBChannels = (*channel);   
	m_jcprops.DIBColor = IJL_BGR;   
	m_jcprops.DIBPadBytes = dib_pad_bytes;   
	m_jcprops.DIBBytes = RGBdata;   
	// Set the JPG color space ... this will always be   
	// somewhat of an educated guess at best because JPEG   
	// is "color blind" (i.e., nothing in the bit stream   
	// tells you what color space the data was encoded from).   
	// However, in this example we assume that we are   
	// reading JFIF files which means that 3 channel images   
	// are in the YCbCr color space and 1 channel images are   
	// in the Y color space.   
	switch(m_jcprops.JPGChannels)   
	{   
	case 1:   
		{   
			m_jcprops.JPGColor = IJL_G;   
			break;   
		}   
	case 3:   
		{   
			m_jcprops.JPGColor = IJL_YCBCR;   
			break;   
		}   
	default:   
		{   
			// This catches everything else, but no   
			// color twist will be performed by the IJL.   
			m_jcprops.DIBColor = (IJL_COLOR)IJL_OTHER;   
			m_jcprops.JPGColor = (IJL_COLOR)IJL_OTHER;   
			break;   
		}   
	}   
	// Now get the actual JPEG image data into the pixel buffer.   
	ijlErr = ijlRead(&m_jcprops,IJL_JBUFF_READWHOLEIMAGE);   

	if(IJL_OK != ijlErr)   
	{   
		printf("ijlRead2 IJL ERR -- %s\n", ijlErrorStr(ijlErr));
		return FALSE;          
	}   

	ijlFree(&m_jcprops);   

	return TRUE;   
}   

void BGRA32toRGB24(
				  unsigned char *RGB32data,
				  unsigned char *RGB24data,
				  int w,
				  int h,
				  int RGB32wstep,
				  int RGB24wstep)
{
	long i,j,m,n;

	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			m=i*RGB32wstep+j*4;
			n=i*RGB24wstep+j*3;

			RGB24data[n]=RGB32data[m+2];
			RGB24data[n+1]=RGB32data[m+1];
			RGB24data[n+2]=RGB32data[m];
		}
	}
}


/*
*裁剪图片
*src 图片rgb数据
*w 图片宽
*h 图片高
*x1,y1 裁剪左上角
*x2,y2 裁剪右下角
*dst 外部需要出入开辟空间的buf
*nCutWidth 裁剪的宽
*nCutHeight 裁剪的高
*nBitCount 图片通道值
*返回裁剪后的数据大小
*/
int CutImage(unsigned char *src, int w, int h,
			 int x1, int y1, int x2, int y2,
			 unsigned char *dst, int &nCutWidth, int &nCutHeight, int nBitCount)
{
	//	int nCutHeight, nCutWidth;
	nCutWidth = x2 - x1 ;
	if(y2>=h)
		nCutHeight = h - y1-1 ;
	else
		nCutHeight = y2 - y1 ;

	if (src == NULL || nCutHeight > h || 
		nCutHeight <= 0 || nCutWidth > w || nCutWidth <= 0)
		return 0;
	int m = 0;
	m = nCutWidth % 16;
	if (m != 0)
	{// 判断裁剪的宽凑成16的倍数是否大于原始图片的宽
		if (nCutWidth + m < w)
			nCutWidth += 16-m;
		else
			nCutWidth -= m;
	}

	// 用循环进行裁剪
	int i, j;
	unsigned char *pTemp = src + y1*w*nBitCount + x1*nBitCount;
	unsigned char *p = NULL;
	p = dst;
	for (i = 0; i < nCutHeight; i++)
	{
		for (j = 0; j < nCutWidth*nBitCount; j += nBitCount)
		{
			*(p++) = pTemp[j];
			*(p++) = pTemp[j+1];
			*(p++) = pTemp[j+2];
		}
		// 偏移一行
		pTemp += w*nBitCount;
	}
	return (nCutWidth*nCutHeight*nBitCount);
}