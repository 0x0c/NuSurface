/*
 *  touchManager.cpp
 *  opengl_test
 *
 *  Created by Akira on 10/08/09.
 *  Copyright 2010 東京工業大学付属科学技術高等学校. All rights reserved.
 *
 */

#include "touchManager.h"

//未確認
void initWithTouchManager(touchManager *tmanager)
{
	printf("initWithTouchManager\n");
	touchObject nullTouch;
	initWithTouchObject(&nullTouch, 0.0, 0.0);
	nullTouch.touchId = -1;
	
	tmanager->touchCount = 0;
	
	for (int i = 0; i < MAX_TOUCHES; i++) {
		tmanager->touchTable[i] = nullTouch;
	}
}

//未確認
touchObject getTouchObject(double x,double y)
{
	//printf("getTouchObject\n");
	//printf("%lf,%lf\n",x,y);
	
	touchObject touch;
	touch.objId = -1;
	touch.movx = 0;
	touch.movy = 0;
	touch.posxStart = x;
	touch.posyStart = y;
	touch.posxNow = x;
	touch.posxPre = touch.posxNow;
	touch.posyNow = y;
	touch.posyPre = touch.posyPre;
	touch.vposxNow = 0;
	touch.vposxPre = 0;
	touch.vposyNow = 0;
	touch.vposyPre = 0;
	touch.gesture.type = noneGesture;
	for (int i = 0; i < MAX_MOVE_HISTORY; i++) {
		touch.gesture.points[i] = 0;
	}
	
	return touch;
}

//未確認
void initWithTouchObject(touchObject *touch, double x,double y)
{
	//printf("initWithTouchObject\n");
	touch->touchId = -1;
	touch->objId = -1;
	touch->posxStart = x;
	touch->posyStart = y;
	touch->posxNow = x;
	touch->posxPre = touch->posxNow;
	touch->posyNow = y;
	touch->posyPre = touch->posyPre;
	touch->vposxNow = 0;
	touch->vposxPre = 0;
	touch->vposyNow = 0;
	touch->vposyPre = 0;
	touch->gesture.type = noneGesture;
	for (int i = 0; i < MAX_MOVE_HISTORY; i++) {
		touch->gesture.points[i] = 0;
	}
	
}

//未確認
void addTouchObject(touchManager *tmanager,touchObject touch)
{
	//printf("addTouchObject\n");
	if (tmanager->touchCount > MAX_TOUCHES) {
		printf("Touch limit.\n");
		return;
	}
	for (int i = 0; i < MAX_TOUCHES; i++) {
		//printf("id%d>>%d\n",i,tmanager->touchTable[i].touchId);
		if (tmanager->touchTable[i].touchId == -1) {
			//printf("add!\n");
			touch.touchId = i;
			tmanager->touchTable[i] = touch;
			return;
		}
	}
}

//未確認
void removeTouchObject(touchManager *tmanager,int touchId)
{
	//printf("removeTouchObject\n");
	touchObject nullTouch;
	nullTouch.touchId = -1;
	nullTouch.objId = -1;
	nullTouch.posxStart = 0;
	nullTouch.posyStart = 0;
	nullTouch.posxNow = 0;
	nullTouch.posxPre = nullTouch.posxNow;
	nullTouch.posyNow = 0;
	nullTouch.posyPre = nullTouch.posyPre;
	nullTouch.vposxNow = 0;
	nullTouch.vposxPre = 0;
	nullTouch.vposyNow = 0;
	nullTouch.vposyPre = 0;
	nullTouch.gesture.type = noneGesture;
	
	tmanager->touchTable[touchId] = nullTouch;
	//tmanager->touchCount--;
}

//未確認
void setTouchPosition(touchObject *touch,double x,double y)
{	
	touch->movx = touch->posxNow - touch->posxStart;
	touch->movy = touch->posyNow - touch->posyStart;
	
	touch->vposxPre = touch->vposxNow;
	touch->vposxNow = x;
	touch->vposyPre = touch->vposyNow;
	touch->vposyNow = y;
	
	touch->posxPre = touch->posxNow;
	touch->posxNow = x;
	touch->posyPre = touch->posyNow;
	touch->posyNow = -y;
}

//未確認
void dumpTouchManager(touchManager tmanager)
{
	int touchCount = 0;
	for (int i = 0; i < MAX_TOUCHES; i++) if (tmanager.touchTable[i].touchId > -1) touchCount++;
	printf("touchObj:%d\n",touchCount);
	for (int i = 0; i < touchCount; i++) dumpTouchObject(tmanager.touchTable[i]);
}

void dumpTouchManagerWithBool(touchManager tmanager)
{
	for (int i = 0; i < MAX_TOUCHES; i++) {
		if (tmanager.touchTable[i].touchId > -1) printf("1,");
		else printf("0,");
	}
	printf("\n");
	return;
}

//未確認
void dumpTouchObject(touchObject tobj)
{
	printf("-------------------------\n");
	printf("touchId:%d\n",tobj.touchId);
	printf("objId:%d\n",tobj.objId);
	printf("posxS:%lf\n",tobj.posxStart);
	printf("posyS:%lf\n",tobj.posyStart);
	printf("posxP:%lf\n",tobj.posxPre);
	printf("posxN:%lf\n",tobj.posxNow);
	printf("posyP:%lf\n",tobj.posyPre);
	printf("posyN:%lf\n",tobj.posyNow);
	
	printf("vposxP:%lf\n",tobj.vposxPre);
	printf("vposxN:%lf\n",tobj.vposxNow);
	printf("vposyP:%lf\n",tobj.vposyPre);
	printf("vposyN:%lf\n",tobj.vposyNow);
	printf("movx:%lf\n",tobj.movx);
	printf("movy:%lf\n",tobj.movy);
}

void setTouchGesture(touchManager *tmanager,Object obj)
{
	return;
}

void execTouchGesture(touchManager *tmanager,Object *obj)
{
	static bool cnt = false;
	
	//multi-touch gesture
	if (obj->touchNum >= 2) {
		printf("multi-touch gesture\n");
		
		//scaling
		static double defaultScale;
		double tmpScale = 1.0;
		
		if (cnt == false) {
			defaultScale = pow(fabs(tmanager->touchTable[obj->touchIds[0]].posxNow) - fabs(tmanager->touchTable[obj->touchIds[1]].posxNow), 2.0) - pow(fabs(tmanager->touchTable[obj->touchIds[0]].posyNow) - fabs(tmanager->touchTable[obj->touchIds[1]].posyNow), 2.0);
			if (defaultScale < 0) {
				defaultScale *= -1;
				defaultScale = sqrt(defaultScale);
			}
		}
		tmpScale = pow(fabs(tmanager->touchTable[obj->touchIds[0]].posxNow) - fabs(tmanager->touchTable[obj->touchIds[1]].posxNow), 2.0) - pow(fabs(tmanager->touchTable[obj->touchIds[0]].posyNow) - fabs(tmanager->touchTable[obj->touchIds[1]].posyNow), 2.0);
		if (tmpScale < 0) {
			tmpScale *= -1;
			tmpScale = sqrt(tmpScale);
		}
		
		obj->scale = (tmpScale / defaultScale) * .05;
		if (obj->scale < .1) obj->scale = .1;
		else if (obj->scale > 3.0) obj->scale = 3.0;
		
		//rotate
		static double defaultAngle;
		double tmpAngle = 0.0;
		
		if (cnt == false) {
			defaultAngle = (atan((tmanager->touchTable[obj->touchIds[0]].posyNow - tmanager->touchTable[obj->touchIds[1]].posyNow) / (tmanager->touchTable[obj->touchIds[0]].posxNow - tmanager->touchTable[obj->touchIds[1]].posyNow)) * 180.0)/ M_PI;
		}
		tmpAngle = (atan((tmanager->touchTable[obj->touchIds[0]].posyNow - tmanager->touchTable[obj->touchIds[1]].posyNow) / (tmanager->touchTable[obj->touchIds[0]].posxNow - tmanager->touchTable[obj->touchIds[1]].posyNow)) * 180.0)/ M_PI;
		//printf("angle:%lf",defaultAngle-tmpAngle);
		obj->angle = tmpAngle - defaultAngle;
		
		cnt = true;
		
		return;
	}
	//single-touch gesture
	else {
		for (int i = 0; i < MAX_TOUCHES; i++) {
			switch (tmanager->touchTable[i].gesture.type) {
				case noneGesture:
					
					
					break;
					
				case flickRight:
					
					cnt = false;
					break;
					
				case flickLeft:
					
					cnt = false;
					break;
					
				case flickUp:
					
					cnt = false;
					break;
					
				case flickDown:
					
					cnt = false;
					break;
			}
		}
	}
}