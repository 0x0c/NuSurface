/*
 *  viewManager.cpp
 *  opengl_test
 *
 *  Created by Akira on 10/05/30.
 *  Copyright 2010 東京工業大学付属科学技術高等学校. All rights reserved.
 *
 */

#include <GLUT/GLUT.h>
#include "viewManager.h"
#include "objectController.h"



//未完成(文字列の表示位置の調整処理が必要)
void drawStringWithOption(const char *string,void *font,double posx,double posy)
{
	//printf("drawStringWithOption:%s\n",string);
	glColor3d(0.0, 0.0, 0.0);
	glRasterPos2d(posx - .45, posy);
	while (*string) {
		glutBitmapCharacter(font, *string);
		++string;
	}
}

//完成
void initWithViewManager(viewManager *manager)
{
	//printf("initWithViewManager\n");
	int i;

	//nullオブジェクトの生成
	Object nullObj = getObjectWithType(0.0, 0.0, 0.0, 0.0, nullObject, false, -1, -1);
	
	for (i = 0; i < MAX_OBJECT; i++) {
		manager->objs[i] = nullObj;
	}
	
	manager->objMemberNum = 0;
}

//完成
void addObject(viewManager *manager,Object obj)
{
	//printf("addObject\n");
	//オブジェクトの追加
	if(manager->objMemberNum < MAX_OBJECT){
		manager->objs[manager->objMemberNum] = obj;
		manager->objMemberNum++;
	}
	else printf("Member limit.\n");
}

//完成
void removeObject(viewManager *manager,int objId)
{
	//printf("removeObject\n");
	int i,j = 0;
	Object nullObj;
	viewManager tmp;
	
	initWithViewManager(&tmp);
	
	if (objId > manager->objMemberNum + 1) {
		printf("Input error.\n");
		return;
	}
	
	//nullオブジェクトの生成
	if (manager->objMemberNum > 0) {
		initObjectWithType(0.0, 0.0, 0.0, 0.0, &nullObj, nullObject, -1, false, nullObject);
		manager->objs[objId] = nullObj;
	}
	else printf("No object.\n");
	
	//textureListのコピー
	for (i = 0; i < MAX_TEXTURE; i++) {
		tmp.textureInfo[i] = manager->textureInfo[i];
		tmp.textureList[i] = manager->textureList[i];
	}
	
	//ビューマネージャを再構成
	//printf("Rebuild view manager.\n");
	for (i = 0; i < manager->objMemberNum; i++) {
		if (manager->objs[i].objId != nullObject) {
			//オブジェクトの追加
			addObject(&tmp, getObjectWithType(manager->objs[i].sizex*2, manager->objs[i].sizey*2, manager->objs[i].posx, manager->objs[i].posy, manager->objs[i].type, manager->objs[i].drugMode, j, manager->textureList[manager->objs[i].resource.texture - 1]));
			
			//サブオブジェクトのコピー
			if (manager->objs[i].subObjMemberNum > 0) {
				//printf("Rebuild sub object.\n");
				for (int k = 0; k < manager->objs[i].subObjMemberNum; k++) {
					tmp.objs[j].subObjs[k] = manager->objs[i].subObjs[k];
					tmp.objs[j].subObjMemberNum++;
				}
			}
			
			if (manager->objs[i].resource.status == true) tmp.objs[i].resource.status = true;
			j++;
		}
	}
	
	manager->objMemberNum--;
	
	*manager = tmp;
}

//完成
void drawViewManager(viewManager *manager)
{
	//printf("drawViewManager\n");
	
	int i;
	
	//ビューマネージャの描画
	for (i = manager->objMemberNum - 1; i >= 0; i--) {
		if(manager->objs[i].t == true) setMatrixTranslarte(manager->objs[i].tposx, manager->objs[i].tposy,&manager->objs[i]);
		else setMatrixTranslarte(manager->objs[i].posx, manager->objs[i].posy,&manager->objs[i]);
		
		//オブジェクトの描画
		drawObject(&manager->objs[i]);
		glLoadIdentity();
	}
}

//未完成
void drawObject(Object *obj)
{
	//printf("drawObject\n");
	
	//オブジェクトの描画
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA , GL_ONE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glBindTexture(GL_TEXTURE_2D, obj->resource.texture);
	glColor4f(1.0, 1.0, 1.0, .5);
	glBegin(GL_QUADS);
	{
		glTexCoord2d(0.0, 0.0);
		glVertex2d(-obj->sizex,obj->sizey);
		glTexCoord2d(0.0, 1.0);
		glVertex2d(-obj->sizex,-obj->sizey);
		glTexCoord2d(1.0, 1.0);
		glVertex2d(obj->sizex,-obj->sizey);
		glTexCoord2d(1.0, 0.0);
		glVertex2d(obj->sizex,obj->sizey);
	}
	glEnd();
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
	
	if (obj->type == alartObj) drawStringWithOption(obj->resource.message, GLUT_BITMAP_HELVETICA_12, obj->posx, obj->posy);
	
	//慣性
	if (obj->touchMode == true) obj->touchMode = animateObject(obj, inertia);
	
	
	//tableObjの後処理
	if (obj->type == tableObj) {
		if (obj->touchMode == true) {
			if (obj->posy < -hy) {
				for (int j = 0; j < obj->subObjMemberNum; j++) {
					obj->subObjs[j].posy = (hy - .5) - (j * 1.0);
					obj->subObjs[j].tposy = obj->subObjs[j].posy;
					obj->subObjs[j].accelerationx = 0;
					obj->subObjs[j].accelerationy = 0;
				}
				obj->accelerationx = 0;
				obj->accelerationy = 0;
				obj->posy = -hy;
				obj->tposy = -hy;
			}
			else if	(obj->posy > hy) {
				int k = -1;
				for (int j = obj->subObjMemberNum; j >= 0; j--) {
					obj->subObjs[j].posy = -(hy - .5) + (k * 1.0);
					obj->subObjs[j].tposy = obj->subObjs[j].posy;
					obj->subObjs[j].accelerationx = 0;
					obj->subObjs[j].accelerationy = 0;
					k++;
				}
				obj->accelerationx = 0;
				obj->accelerationy = 0;
				obj->posy = hy;
				obj->tposy = hy;
			}
			else {
				for (int j = 0; j < obj->subObjMemberNum; j++) {
					obj->subObjs[j].posy = obj->subObjs[j].tposy;
				}
			}
		}
	}
	//glDisable(GL_BLEND);
}

//未完成
void drawSubObject(subObject subObj)
{	
	//printf("drawSubObject\n");
	
	setMatrixTranslarte(subObj.posx, subObj.tposy);
	if (subObj.subType == cellObj) {
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_QUADS);
		{
			glTexCoord2d(0.0, 0.0);
			glVertex2d(-subObj.sizex,subObj.sizey);
			
			glColor4f(1.0, 1.0, 0.0, 1.0);
			glTexCoord2d(0.0, 1.0);
			glVertex2d(-subObj.sizex,-subObj.sizey);
			
			glColor4f(1.0, 0.0, 1.0, 1.0);
			glTexCoord2d(1.0, 1.0);
			glVertex2d(subObj.sizex,-subObj.sizey);
			
			glColor4f(0.0, 1.0, 1.0, 1.0);
			glTexCoord2d(1.0, 0.0);
			glVertex2d(subObj.sizex,subObj.sizey);
		}
		glEnd();
	}
	else {
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_QUADS);
		{
			glTexCoord2d(0.0, 0.0);
			glVertex2d(-subObj.sizex,subObj.sizey);
			glTexCoord2d(0.0, 1.0);
			glVertex2d(-subObj.sizex,-subObj.sizey);
			glTexCoord2d(1.0, 1.0);
			glVertex2d(subObj.sizex,-subObj.sizey);
			glTexCoord2d(1.0, 0.0);
			glVertex2d(subObj.sizex,subObj.sizey);
		}
		glEnd();
	}
	
	drawStringWithOption(subObj.resource.message, GLUT_BITMAP_HELVETICA_12, 0,0);
}

//完成
void checkHitViewManager(touchObject *tobj,viewManager *manager)//総当たり判定
{
	//printf("checkHitViewManager\n");
	int i;
	
	tobj->posyNow = -tobj->posyNow;
	
	//当たり判定チェック開始
	for (i = 0; i < manager->objMemberNum ; i++) {
		//オブジェクトの種類ごとの条件分岐
		switch (manager->objs[i].type) {
			case buttonObj:
				if(hitConditionV){
					manager->objs[i].thit = true;
					manager->objs[i].resource.texture = manager->textureList[texButtonObj_pushed];
					
					tobj->objId = manager->objs[i].objId;
					
					//
					manager->objs[i].posx = manager->objs[i].tposx;
					manager->objs[i].posy = manager->objs[i].tposy;
					manager->objs[i].accelerationx = 0;
					manager->objs[i].accelerationy = 0;
					manager->objs[i].touchMode = false;
					//
					
					manager->objs[i].action(manager->objs[i].type);
					
					return;
				}
				break;
				
			case switchObj:
				if(hitConditionV){
					manager->objs[i].thit = true;
					if (manager->objs[i].resource.status == false){
						manager->objs[i].resource.status = true;
						manager->objs[i].resource.texture = manager->textureList[texSwitchObj_on];
					}
					else {
						manager->objs[i].resource.status = false;
						manager->objs[i].resource.texture = manager->textureList[texSwitchObj_off];
					}
					
					tobj->objId = manager->objs[i].objId;
					
					//
					manager->objs[i].posx = manager->objs[i].tposx;
					manager->objs[i].posy = manager->objs[i].tposy;
					manager->objs[i].accelerationx = 0;
					manager->objs[i].accelerationy = 0;
					manager->objs[i].touchMode = false;
					//
					
					manager->objs[i].action(manager->objs[i].type);
					
					return;
				}
				break;
				
			case imageObj:
				if(hitConditionV){
					manager->objs[i].thit = true;
					
					tobj->objId = manager->objs[i].objId;
					
					//
					manager->objs[i].posx = manager->objs[i].tposx;
					manager->objs[i].posy = manager->objs[i].tposy;
					manager->objs[i].accelerationx = 0;
					manager->objs[i].accelerationy = 0;
					manager->objs[i].touchMode = false;
					//
					
					manager->objs[i].action(manager->objs[i].type);
					
					return;
				}
				break;
				
			case planeObj:
				if(hitConditionV){
					
					tobj->objId = manager->objs[i].objId;
					
					manager->objs[i].action(manager->objs[i].type);
					return;
				}
				break;
				
			case menuObj:
				if(hitConditionV){
					
					tobj->objId = manager->objs[i].objId;
					
					manager->objs[i].action(manager->objs[i].type);
					
					
					return;
				}
				break;
				
			case tableObj:
				if(hitConditionV){
					manager->objs[i].thit = true;
					
					tobj->objId = manager->objs[i].objId;
					
					//
					manager->objs[i].posx = manager->objs[i].tposx;
					manager->objs[i].posy = manager->objs[i].tposy;
					manager->objs[i].accelerationx = 0;
					manager->objs[i].accelerationy = 0;
					manager->objs[i].touchMode = false;
					//
					
					if (manager->objs[i].subObjMemberNum > 0) {
						for (int j = 0; j < manager->objs[i].subObjMemberNum; j++) {
							
							//
							manager->objs[i].subObjs[j].accelerationx = 0;
							manager->objs[i].subObjs[j].accelerationy = 0;
							manager->objs[i].subObjs[j].touchMode = false;
							//
							
						}
					}
					
					manager->objs[i].action(manager->objs[i].type);
					
					return;
				}
				break;
				
			case alartObj:
				if(hitConditionV){
					tobj->objId = manager->objs[i].objId;
					removeObject(manager,i);
					
					manager->objs[i].action(manager->objs[i].type);
					
					return;
				}
				break;
				
			case rectObj:
				if(hitConditionV){
					manager->objs[i].thit = true;
					
					//
					manager->objs[i].posx = manager->objs[i].tposx;
					manager->objs[i].posy = manager->objs[i].tposy;
					manager->objs[i].accelerationx = 0;
					manager->objs[i].accelerationy = 0;
					manager->objs[i].touchMode = false;
					//
					
					tobj->objId = manager->objs[i].objId;
					
					manager->objs[i].action(manager->objs[i].type);
					
					return;
				}
				break;
				
			default:
				
				break;
		}
	}
	
	printf("No objevt hit\n");
	
	return;
}

//完成
void dumpViewManager(viewManager manager)
{
	int i;
	
	printf("-----ViewManager-----\n");
	printf("Member:%d\n",manager.objMemberNum);

	for (i = 0; i < manager.objMemberNum; i++) dumpObject(manager.objs[i]);
}
