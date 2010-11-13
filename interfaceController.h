/*
 *  interfaceController.h
 *  opengl_test
 *
 *  Created by Akira on 10/08/14.
 *  Copyright 2010 東京工業大学付属科学技術高等学校. All rights reserved.
 *
 */

#ifndef INTERFACE_CONTROLLER
#define INTERFACE_CONTROLLER

#include "viewManager.h"
#include "touchManager.h"
#include "multi-touchLib.h"
#include "objectController.h"
#include "glutController.h"
#include "FRController.h"
#include "displayManager.h"
#include "parameter.h"

void displayMainView(void);
void MTdisplayWithGLUT(void);
void timer(int value);
void reshape(int w, int h);
void keyboard(unsigned char key,int x,int y);
void idle(void);
void mouse(int button,int state,int x,int y);
void motion(int x,int y);
void initUIFramework(void);
void initGLUT(int argc, char **argv);
void initAll(int argc, char **argv);

#endif