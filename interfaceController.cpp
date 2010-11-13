/*
 *  interfaceController.cpp
 *  opengl_test
 *
 *  Created by Akira on 10/08/14.
 *  Copyright 2010 東京工業大学付属科学技術高等学校. All rights reserved.
 *
 */

#include "interfaceController.h"

#define hx 5.0
#define hy 2.5

displayManager dmanager;
//viewManager manager;
touchManager tmanager;
MTCaptureImg cap;
FRController frc;

void displayMainView(void)
{	
	displayController(&dmanager.manager[dmanager.diplayId],&tmanager,false);
	MTTimer(&cap,&dmanager.manager[dmanager.diplayId],&tmanager);
	
	for (int i = 0; i < dmanager.manager[dmanager.diplayId].objMemberNum; i++) {
		drawDebugObjectOutline(dmanager.manager[dmanager.diplayId].objs[i]);
	}
	
	//touch gesture
	for (int i = 0; i < dmanager.manager[dmanager.diplayId].objMemberNum; i++) {
		setTouchGesture(&tmanager, dmanager.manager[dmanager.diplayId].objs[i]);
		execTouchGesture(&tmanager, &dmanager.manager[dmanager.diplayId].objs[i]);
	}
	
	//FR
	//FRTimer(&frc, &dmanager);
	
	glColor3d(1.0, 0.0, 0.0);
	
	for (int i = 0; i < MAX_TOUCHES; i++) {
		if (tmanager.touchTable[i].touchId > -1) {
			//printf("%lf,%lf\n",tmanager.touchTable[i].posxNow,tmanager.touchTable[i].posyNow);
			//printf("draw\n");
			drawCircle(tmanager.touchTable[i].posxNow, tmanager.touchTable[i].posyNow);
		}
		
		//tmanager.touchTable[i].vposxPre = 0;
		//tmanager.touchTable[i].vposyPre = 0;
	}

	glLineWidth(2);
	glColor3d(1.0,1.0,1.0);
	glBegin(GL_LINE_LOOP);
	{
		glVertex2d(-hx, hy);
		glVertex2d( hx, hy);
		glVertex2d( hx,-hy);
		glVertex2d(-hx,-hy);
	}
	glEnd();
	//drawDebugAxis();
	glutSwapBuffers();
}

void MTGLUTdisplay(void)
{
	MTDisplay(&cap);
}

void timer(int value)
{
	//MTTimer(&cap);
	//dumpViewManager(manager);
	//dumpTouchManager(tmanager);
	glutTimerFunc(33,timer,0);
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);	//画面の出る範囲
	glMatrixMode(GL_PROJECTION);			//視界変換宣言
	glLoadIdentity();				//単位行列代入
	glOrtho(-hx, hx, -hy, hy, -1.0, 1.0);//視体積の設定
	glMatrixMode(GL_MODELVIEW);			//モデル変換宣言
	glLoadIdentity();				//単位行列代入	
}

void keyboard(unsigned char key,int x,int y)
{
	keyboardController(key,x,y, &dmanager,&dmanager.manager[dmanager.diplayId],&tmanager,&cap);
	MTKeyboard(&cap, key, x, y);
}

void idle(void)
{
	glutSetWindow(1);
	MTGLUTdisplay();
	glutSetWindow(2);
	displayMainView();
}

void mouse(int button,int state,int x,int y)
{
	//objectWithTouchAction(button, state, x, y, &manager,&tmanager);
}

void motion(int x,int y)
{
	//objectWithMotionAction(x, y, &manager,&tmanager);
}

void initUIFramework(void)
{
	//ディスプレイマネージャの初期化
	initWithDisplayManager(&dmanager);
	
	//タッチマネージャの初期化
	initWithTouchManager(&tmanager);
}

void initGLUT(int argc, char **argv)
{
	//window2
	glutInitWindowPosition(600, 600);
	glutInitWindowSize(640,480);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
	glutCreateWindow("cap");
	glutDisplayFunc(MTGLUTdisplay);
	glutKeyboardFunc(keyboard);
	
	//window1
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
	glutCreateWindow("NuSurface");
	glutDisplayFunc(displayMainView);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	
	glutTimerFunc(33, timer, 0);
}

void initAll(int argc, char **argv)
{
	initGLUT(argc,argv);
	initUIFramework();
	MTInit(&cap,argc,argv);
	
	//initWithFRController(&frc);
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//背景色
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}