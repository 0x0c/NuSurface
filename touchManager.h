/*
 *  touchManager.h
 *  opengl_test
 *
 *  Created by Akira on 10/08/09.
 *  Copyright 2010 東京工業大学付属科学技術高等学校. All rights reserved.
 *
 */

#ifndef TOUCH_MANAGER
#define TOUCH_MANAGER

#include "object.h"
#include "parameter.h"

typedef enum {
	noneGesture,
	pinch,
	flickRight,
	flickLeft,
	flickUp,
	flickDown,
	rotate
}gestureType;

typedef struct {
	gestureType type;//ジェスチャタイプ
	double points[MAX_MOVE_HISTORY];//ジェスチャ判別のための軌跡
	//マルチタッチジェスチャに必要な情報を貯めておく
}touchGesture;

typedef struct {
	int touchId;//タッチのID
	int objId;//タッチしているオブジェクトのID
	//移動量関係
	double movx;
	double movy;
	double posxStart;
	double posyStart;
	double posxNow;//一定間隔内の現在の点(x)
	double posxPre;//一定間隔内の前回の点(x)
	double posyNow;//一定間隔内の現在の点(y)
	double posyPre;//一定間隔内の前回の点(y)
	//運動方向関係
	double vposxNow;//xの移動方向
	double vposxPre;//xの移動方向
	double vposyNow;//yの移動方向
	double vposyPre;//yの移動方向
	touchGesture gesture;
}touchObject;

typedef struct {
	int touchCount;//タッチの数
	touchObject touchTable[MAX_TOUCHES];//タッチ一覧
}touchManager;

void initWithTouchManager(touchManager *tmanager);//タッチマネージャの初期化
touchObject getTouchObject(double x,double y);//タッチの生成
void initWithTouchObject(touchObject *touch, double x,double y);//タッチの初期化
void addTouchObject(touchManager *tmanager,touchObject touch);//タッチの追加
void removeTouchObject(touchManager *tmanager,int touchId);//タッチの解放
void releaseAllTouchManager(touchManager *tmanager);//タッチの解放
void setTouchPosition(touchObject *touch,double x,double y);//タッチの位置情報を設定
void dumpTouchManager(touchManager tmanager);//タッチマネージャのダンプ
void dumpTouchManagerWithBool(touchManager tmanager);
void dumpTouchObject(touchObject tobj);//タッチオブジェクトのダンプ

void setTouchGesture(touchManager *tmanager,Object obj);
void execTouchGesture(touchManager *tmanager,Object *obj);

#endif