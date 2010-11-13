/*
 *  viewManager.h
 *  opengl_test
 *
 *  Created by Akira on 10/05/30.
 *  Copyright 2010 東京工業大学付属科学技術高等学校. All rights reserved.
 *
 */

#ifndef VIEW_MANAGER
#define VIEW_MANAGER

#include "object.h"
#include "touchManager.h"
#include "parameter.h"

typedef struct{
	Object objs[MAX_OBJECT];//オブジェクト
	int objMemberNum;//オブジェクトの数
	GLuint textureList[MAX_TEXTURE];//テクスチャリスト
	pngInfo textureInfo[MAX_TEXTURE];//テクスチャインフォ
}viewManager;

void drawStringWithOption(const char *string,void *font,double posx,double posy);//文字列表示
void initWithViewManager(viewManager *manager);//初期化
void addObject(viewManager *manager,Object obj);//オブジェクトの追加
void removeObject(viewManager *manager,int objId);//オブジェクトの削除
void drawViewManager(viewManager *manager);//ビューマネージャの描画
void drawObject(Object *obj);//オブジェクトの描画
void drawSubObject(subObject subObj);//サブオブジェクトの描画
void checkHitViewManager(touchObject *tobj,viewManager *manager);//総当たり判定
void dumpViewManager(viewManager manager);//ビューマネージャをダンプ

#endif