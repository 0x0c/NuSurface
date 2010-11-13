/*
 *  glutController.h
 *  opengl_test
 *
 *  Created by Akira on 10/05/30.
 *  Copyright 2010 東京工業大学付属科学技術高等学校. All rights reserved.
 *
 */
#ifndef GLUT_CONTROLLER
#define GLUT_CONTROLLER

#include <stdlib.h>
#include <GLUT/GLUT.h>
#include "interfaceController.h"
#include "objectController.h"
#include "viewManager.h"
#include "multi-touchLib.h"
#include "displayManager.h"

void displayController(viewManager *manager,touchManager *tmanager,bool debug);//display()の役割
void keyboardController(unsigned char key,int x,int y,displayManager *dmanager,viewManager *manager,touchManager *tmanager,MTCaptureImg *cap);//keyboard()の役割

#endif