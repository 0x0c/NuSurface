/*
 *  parameter.h
 *  opengl_test
 *
 *  Created by Akira on 10/09/25.
 *  Copyright 2010 東京工業大学付属科学技術高等学校. All rights reserved.
 *
 */

#ifndef PARAMETER
#define PARAMETER

//object
#define MAX_SUB_OBJECT 10

//objectController
#define FRICTION .000001//小さいほど滑る
#define REG 5//小さいほど滑る距離が伸びる

//viewManager
#define MAX_OBJECT 100
#define MAX_TEXTURE 7

#define hitConditionV \
manager->objs[i].posx + manager->objs[i].sizex > tobj->posxNow && \
manager->objs[i].posx - manager->objs[i].sizex < tobj->posxNow && \
manager->objs[i].posy + manager->objs[i].sizey > tobj->posyNow && \
manager->objs[i].posy - manager->objs[i].sizey < tobj->posyNow

#define hx 5.0
#define hy 2.5

//touchManager
#define MAX_TOUCHES 10
#define MAX_MOVE_HISTORY 50

//multi-touchLib
#define ISIGHT_RES_WIDTH 640.0
#define ISIGHT_RES_WIDTH_HALF 320.0
#define ISIGHT_RES_HEIGHT 480.0
#define ISIGHT_RES_HEIGHT_HALF 240.0

#define hitCondition \
manager->objs[j].posx + manager->objs[j].sizex > tmanager->touchTable[i].posxStart && \
manager->objs[j].posx - manager->objs[j].sizex < tmanager->touchTable[i].posxStart && \
manager->objs[j].posy + manager->objs[j].sizey > tmanager->touchTable[i].posyStart && \
manager->objs[j].posy - manager->objs[j].sizey < tmanager->touchTable[i].posyStart 

#define TOUCH_DIST 10
#define TOUCH_C_DIST 0
#define TOUCH_A_DIST 0

//interfaceController
#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 600

//FRController
#define CHAR_MAX_LENGTH 128
#define BAUDRATE B9600
//#define MODEMDEVICE "/dev/cu.usbserial-A800eGr2"//学校
#define MODEMDEVICE "/dev/cu.usbserial-A900acz3"//原

//displayManager
#define MAX_VIEW_MANAGER 5
#define APP_MANAGER 0
#define FIRST_APP 1
#define SECOND_APP 2
#define THIRD_APP 3
#define FIFTH_APP 4

#endif