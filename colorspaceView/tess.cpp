#include "stdafx.h"
#include "tess.h"

void CALLBACK errorCallback(GLenum errorCode)
{
   const GLubyte *estring;

   estring = gluErrorString(errorCode);
   fprintf(stderr, "Tessellation Error: %s\n", estring);
   exit(0);
}

void CALLBACK vertexCallback(GLvoid *vertex)
{
   const GLdouble *pointer;

   pointer = (GLdouble *) vertex;
   glColor3dv(pointer+2);
   glVertex2dv((GLdouble *)vertex);
}

/*  combineCallback is used to create a new vertex when edges
 *  intersect.  coordinate location is trivial to calculate,
 *  but weight[4] may be used to average color, normal, or texture
 *  coordinate data.  In this program, color is weighted.
 */
void CALLBACK combineCallback(GLdouble coords[3], 
                     GLdouble *vertex_data[4],
                     GLfloat weight[4], GLdouble **dataOut )
{
   GLdouble *vertex;
   int i;

   vertex = (GLdouble *) malloc(5 * sizeof(GLdouble));

   vertex[0] = coords[0];
   vertex[1] = coords[1];
   //vertex[2] = coords[2];
   for (i = 2; i < 5; i++)
      vertex[i] = weight[0] * vertex_data[0][i] 
                  + weight[1] * vertex_data[1][i];
   *dataOut = vertex;
}

//传入 XYZ RGB
void Tessdraw(double **data,long total)
{

	GLUtesselator *tobj;

	tobj = gluNewTess();

	//声明下面分格化对象用到的回调函数
	//即分格化五角星的绘制
	gluTessCallback(tobj, GLU_TESS_VERTEX,
		(void (CALLBACK *)())vertexCallback);
	gluTessCallback(tobj, GLU_TESS_BEGIN,
		(void (CALLBACK *)())glBegin);
	gluTessCallback(tobj, GLU_TESS_END,
		(void (CALLBACK *)())glEnd);
	gluTessCallback(tobj, GLU_TESS_ERROR,
		(void (CALLBACK *)())errorCallback);
	gluTessCallback(tobj, GLU_TESS_COMBINE,
		(void (CALLBACK *)())combineCallback);

	//指定分格化属性，哪部分位于内部（要绘制颜色），哪一部分位于外部（不用绘制颜色）
	//指定环绕规则
	// gluTessProperty(tobj,GLU_TESS_BOUNDARY_ONLY,TRUE);
	gluTessProperty(tobj, GLU_TESS_WINDING_RULE,   GLU_TESS_WINDING_ODD);
	gluTessProperty(tobj,GLU_TESS_TOLERANCE,1.0);

	gluTessBeginPolygon(tobj, NULL);
	gluTessBeginContour(tobj);
	for (long i=0;i<total;i++)
	{
		gluTessVertex(tobj, data[i], data[i]);
	}

	gluTessEndContour(tobj);
	gluTessEndPolygon(tobj);

	gluDeleteTess(tobj);


}
/*



void S57draw_faceTESSdraw(double *** tessdata,unsigned  int  *tesslength,
						  unsigned int p, unsigned int j)
{
	unsigned int k, m;
	unsigned int q;

	////////////////////////////////////////
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	gluTessBeginPolygon(tobj, NULL);

	for(k = 0; k < smx_mapdata[p].smdFRIDinfo[j].fspt_total; k++)
	{
		

		glColor4f((p+1) * (k+1) *(q+1)* 0.05, (k+1)* (j+1) * 0.01, (q+1)*(j+1) * 0.005, 0.4);

		if(0==tesslength[k])
			continue;

		gluTessBeginContour(tobj);

		for(m = 0; m < tesslength[k]; m++)
		{
			//必须传入3 DOUBLE
			gluTessVertex(tobj, tessdata[k][m], tessdata[k][m]);
		}
		gluTessEndContour(tobj);
	}
	gluTessEndPolygon(tobj);

	glDisable (GL_BLEND);

	////////////////////////////////////////////////////////
	for(k = 0; k < smx_mapdata[p].smdFRIDinfo[j].fspt_total; k++)
	{
		for(m = 0; m < tesslength[k]; m++)
			free(tessdata[k][m]);

		free(tessdata[k]);
	}
	free(tessdata);
	free(tesslength);
}
*/