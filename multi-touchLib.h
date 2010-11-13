/*
 *  multi-touchLib.h
 *  opengl_test
 *
 *  Created by Akira on 10/07/31.
 *  Copyright 2010 東京工業大学付属科学技術高等学校. All rights reserved.
 *
 */

#ifndef MULTI_TOUCH_LIB
#define MULTI_TOUCH_LIB

#include <stdio.h>
#include <OpenCV/cv.h>
#include <OpenCV/highgui.h>
#include <GLUT/glut.h>
#include "viewManager.h"
#include "touchManager.h"
#include "parameter.h"

typedef struct{
	int x;
	int y;
	unsigned int index;
} Point;

typedef struct{
	double xNow;
	double yNow;
	double xPre;
	double yPre;
} tmpPoint;

typedef struct{
	int trackbarValue01,trackbarValue02,trackbarValue03;
	int tnum;
	double a[8],b[8];
	bool calibrated;
	CvRect roi;
	
	CvCapture *cap;
	IplImage *capImg, *maskImg, *hsvImg, *maskedImg;
	IplImage *distImg, *distNormImg, *tmpImg, *resultImg;
	
	CvMemStorage *contourStorage;
	CvSeq *contours;
	CvTreeNodeIterator it;
	CvPoint *point;
	Point touchNow[MAX_TOUCHES],touchPre[MAX_TOUCHES];
	CvFont font;
}MTCaptureImg;

void MTFinalaze(MTCaptureImg *cap);
void MTDisplay(MTCaptureImg *cap);
void MTTimer(MTCaptureImg *cap,viewManager *manager,touchManager *tmanager);
void MTKeyboard(MTCaptureImg *cap,unsigned char key, int x, int y);
void initWithMTCaptureImg(MTCaptureImg *cap);
void MTInit(MTCaptureImg *cap,int argc,char **argv);
void MTCalibration(touchManager *tmanager,int x,int y);
void setTouches(MTCaptureImg *cap,touchManager *tmanager,viewManager *manager);
void MTDrug(touchManager *tmanager,viewManager *manager);
void setCameraImageWithCalibrate(MTCaptureImg *cap);
void getCameraImageWithCalibrate(MTCaptureImg *cap);
void mouseClickWithCalibration(int event, int x, int y, int flags, void *pram);

#endif