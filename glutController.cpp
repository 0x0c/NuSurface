/*
 *  glutController.cpp
 *  opengl_test
 *
 *  Created by Akira on 10/05/30.
 *  Copyright 2010 東京工業大学付属科学技術高等学校. All rights reserved.
 *
 */

#include "glutController.h"

//未完成
void displayController(viewManager *manager,touchManager *tmanager,bool debug)
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glColor3f(1.0, 1.0, 1.0);
	
	drawViewManager(manager);
	
	double x,y;
	
	for (int i = 0; i < tmanager->touchCount ; i++) {
		x = tmanager->touchTable[i].posxNow;
		y = tmanager->touchTable[i].posyNow;
		
		drawCircle(x, y);
	}
	
	if (debug == true) {
		drawDebugAxis();
		drawDebugObjectOutline(manager->objs[1]);
	}
}

//完成
void keyboardController(unsigned char key,int x,int y,displayManager *dmanager,viewManager *manager,touchManager *tmanager,MTCaptureImg *cap)
{
	int i;
	
	/*
	switch (key) {
		case :
			
			break;
		default:
			break;
	}
	*/
	
	if (key == 'q')exit(0);
	
	if (key == 'f')glutFullScreen();
	
	if (key == 'z'){
		for (i = 0; i < manager->objMemberNum; i++) {
			manager->objs[i].angle += 1.0;
			if(manager->objs[i].angle > 360)manager->objs[i].angle = 1.0;
		}
	}
	
	if (key == 'c') {
		setCameraImageWithCalibrate(cap);
	}
	
	if (key == 'd'){
		dumpViewManager(*manager);
		printf("\n");
	}
	
	if (key == 'u'){
		for (i = 0; i < manager->objMemberNum; i++) {
			manager->objs[i].posy += 0.01;
		}
	}
	
	if (key == 'j'){
		for (i = 0; i < manager->objMemberNum; i++) {
			manager->objs[i].posy -= 0.01;
		}
	}
	
	if (key == 'k'){
		for (i = 0; i < manager->objMemberNum; i++) {
			manager->objs[i].posx += 0.01;
		}
	}
	
	if (key == 'h'){
		for (i = 0; i < manager->objMemberNum; i++) {
			manager->objs[i].posx -= 0.01;
		}
	}

	if (key == 'r'){
		initUIFramework();
	}
	
	if (key == 'w') {
		for (i = 0; i < manager->objMemberNum; i++) {
			manager->objs[i].posx = 0;
			manager->objs[i].posy = 0;
			manager->objs[i].accelerationx = 0;
			manager->objs[i].accelerationy = 0;
			manager->objs[i].thit = false;
		}
	}
	
	if (key == 's') {
		for (i = 0; i < manager->objMemberNum; i++) {
			dumpSubObject(manager->objs[i]);
		}
	}
	
	if (key == ']') {
		printf("%d >> ",dmanager->diplayId);
		if (dmanager->diplayId < MAX_VIEW_MANAGER - 1) {
			dmanager->diplayId++;
		}
		printf("%d\n",dmanager->diplayId);
	}
	
	if (key == '[') {
		printf("%d >> ",dmanager->diplayId);
		if (dmanager->diplayId > 0) {
			dmanager->diplayId--;
		}
		printf("%d\n",dmanager->diplayId);
	}
	
	if (key == 'm') {
		printf("%d\n",dmanager->diplayId);
	}
	
	if (key == 't') {
		dumpTouchManager(*tmanager);
	}
	 
	glutPostRedisplay();
}