/*
 *  object.h
 *  opengl_test
 *
 *  Created by Akira on 10/05/30.
 *  Copyright 2010 東京工業大学付属科学技術高等学校. All rights reserved.
 *
 */
#ifndef OBJECT
#define OBJECT

#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include "glpng.h"
#include "parameter.h"

enum objType {
	buttonObj,
	switchObj = 2,
	rectObj = 4,
	alartObj,
	imageObj,
	planeObj,
	menuObj,
	tableObj,
	nullObject = -1
};

enum objTexture {
	texButtonObj_unpushed,
	texButtonObj_pushed,
	texSwitchObj_off,
	texSwitchObj_on,
	texRectObj,
	texAlartObj,
	texTableObj
};

enum objTexture2 {
	texOne,
	texTwo,
	texThree,
	texFour,
	texFive,
	texSix,
	texSeven,
	texEight,
	texNine,
	texZero,
	texAdd,
	texSub,
	texMul,
	texDiv,
	texEqual
};

enum subObjType {
	cellObj,
	indexObj,
	scrollObj
};

typedef struct{
	GLuint texture;//texture
	bool status;//switchObject
	char message[3];//alartObject
}Resource;

typedef struct{
	enum subObjType subType;
	double	sizex,sizey;//縦横サイズ
	double	posx,posy,//位置
	tposx,tposy;//ドラッグ中の位置
	double accelerationx;//加速度x
	double accelerationy;//加速度y
	double angle;//回転角度
	bool touchMode;
	Resource resource;
	void (*action)(objType type,...);//オブジェクトのアクション
}subObject;

typedef struct{
	enum objType type;
	int objId;//オブジェクトの番号
	int subObjMemberNum;//サブオブジェクトの数
	int t;//モード切り替え
	int thit;//当たり判定切り替え
	int touchNum;
	int touchIds[2];
	double	sizex,sizey;//縦横サイズ
	double	posx,posy,//位置
	tposx,tposy;//ドラッグ中の位置
	double angle;//回転角度
	double scale;
	double alpha;
	double accelerationx;//加速度x
	double accelerationy;//加速度y
	bool touchMode;//タッチの状態
	bool accMode;
	bool drugMode;
	Resource resource;
	subObject subObjs[MAX_SUB_OBJECT];
	
	void (*action)(objType type,...);//オブジェクトのアクション
}Object;

void setTextureList(GLuint *textures,pngInfo *textureInfo);//テクスチャの読み込み
void setTexture(GLuint *textures,pngInfo *textureInfo,int texNum,char *path);
void addSubObject(Object *obj,subObject subObj);//サブオブジェクトの追加
void drawDebugObjectOutline(Object obj);//オブジェクトの境界線を描画
void drawDebugAxis(void);//x,y軸の描画
void drawCircle(double x,double y);
void initObjectWithType(double x,double y,double posx,double posy,Object *obj,objType type,int objId, bool status, const GLuint textureList);//オブジェクトを位置情報付きで初期化
Object getObjectWithType(double x,double y,double posx,double posy,objType type,bool status,int objId,GLuint textureList);//オブジェクトを位置情報付きで生成
void setObjectWithMessage(Object *obj,const char *message);//警告文の設定
void setSubObjectWithMessage(subObject *subObj,const char *message);
void initSubObjectWithType(subObject *subObj,subObjType type);//サブオブジェクトの初期化
subObject getSubObjectWithType(subObjType type);//サブオブジェクトの生成
void drawCircle(double x,double y);//デバッグ用
void buildTableObjectWithSubObject(Object *obj,int subObjNum);
void dumpObject(Object obj);//オブジェクトをダンプ
void dumpSubObject(Object obj);//サブオブジェクトをダンプ
void printObjectType(objType type,...);//オブジェクトのタイプを表示

#endif