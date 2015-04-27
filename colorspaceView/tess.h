#ifndef __TESS_GL_H__
#define __TESS_GL_H__

#if 1
#include <GL/glut.h>
#else 
#include <GL/gl.h>
#include <GL/glu.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#endif

void Tessdraw(double **data,long total);

#endif