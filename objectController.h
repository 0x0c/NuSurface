/*
 *  objectController.h
 *  opengl_test
 *
 *  Created by Akira on 10/05/30.
 *  Copyright 2010 東京工業大学付属科学技術高等学校. All rights reserved.
 *
 */

#ifndef OBJ_CONTROLLER
#define OBJ_CONTROLLER

#include <GLUT/GLUT.h>
#include "object.h"
#include "viewManager.h"
#include "touchManager.h"
#include "parameter.h"

enum animationType {
	inertia,
	fade_in,
	fade_out
};

void setMatrixTranslarte(double x,double y,Object *obj);//オブジェクトの移動
void setMatrixTranslarte(double x,double y);
void objectWithTouchAction(int button,int state,int x,int y,viewManager *manager,touchManager *tmanager);////タッチによるオブジェクトに対する当たり判定
void objectWithMotionAction(int x,int y,viewManager *manager,touchManager *tmanager);//オブジェクトの操作
bool animateObject(Object *obj,animationType type);//オブジェクトのアニメーション

#endif