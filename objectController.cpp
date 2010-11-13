/*
 *  objectController.cpp
 *  opengl_test
 *
 *  Created by Akira on 10/05/30.
 *  Copyright 2010 東京工業大学付属科学技術高等学校. All rights reserved.
 *
 */

#include "objectController.h"

double sx,sy,sz;//クリックしたところのグローバル座標を格納
static double mx,my,mz;//グローバル座標格納

//完成
void setMatrixTranslarte(double x,double y,Object *obj)//オブジェクトの移動
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(x, y, 1.0);
	glRotated(obj->angle, 0.0, 0.0, 1.0);
	glScaled(obj->scale, obj->scale, 1.0);
}

void setMatrixTranslarte(double x,double y)//サブオブジェクトの移動
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(x, y, 0.0);
	glRotated(0.0, 0.0, 0.0, 0.0);
}

//完成
void objectWithTouchAction(int button,int state,int x,int y,viewManager *manager,touchManager *tmanager)
{
	double modelView[16];
	double projection[16];
	int viewport[4];
	
	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);
	
	//touch
	if(state == GLUT_DOWN){
		sx = sy = sz = 0;
		gluUnProject(x, y, 0, modelView, projection, viewport, &sx, &sy, &sz);
		
		addTouchObject(tmanager, getTouchObject(sx, sy));
		
		for (int i = 0; i < tmanager->touchCount; i++) {
			checkHitViewManager(&tmanager->touchTable[i],manager);
		}
	}
	
	//untouch
	else if(state == GLUT_UP) {
		removeTouchObject(tmanager, 1);
		
		//オブジェクトの後処理
		for (int i = 0; i < manager->objMemberNum; i++) {
			if (manager->objs[i].thit == true) {
				manager->objs[i].touchMode = true;
				manager->objs[i].t = false;
				manager->objs[i].thit = false;
				if (manager->objs[i].subObjMemberNum > 0) {
					for (int j = 0; j < manager->objs[i].subObjMemberNum; j++) {
						manager->objs[i].subObjs[j].touchMode = true;
					}
				}
			}
			
			manager->objs[i].posx = manager->objs[i].tposx;
			manager->objs[i].posy = manager->objs[i].tposy;
						
			//buttonObjの後処理
			if (manager->objs[i].type == buttonObj) manager->objs[i].resource.texture = manager->textureList[texButtonObj_unpushed];
			
			gluUnProject(x, y, 0, modelView, projection, viewport, &sx, &sy, &sz);
		}
	}
}//タッチによるオブジェクトに対する当たり判定

//完成
void objectWithMotionAction(int x,int y,viewManager *manager,touchManager *tmanager)
{
	double modelView[16];//モデルビュー行列格納
	double projection[16];//プロジェクション行列格納
	int viewport[4];//ビューポート行列格納
	
	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);
	
	for (int i = 0; i < tmanager->touchCount; i++) {
		gluUnProject(x, y, 0, modelView, projection, viewport, &mx, &my, &mz);
		setTouchPosition(&tmanager->touchTable[i],mx, my);
		
		if (tmanager->touchTable[i].objId > -1) {
			//tableObjでないならば
			if (manager->objs[tmanager->touchTable[i].objId].type != tableObj) {
				if(manager->objs[tmanager->touchTable[i].objId].thit == true){
					//オブジェクトの移動
					manager->objs[tmanager->touchTable[i].objId].tposx = manager->objs[tmanager->touchTable[i].objId].posx + tmanager->touchTable[i].movx;
					manager->objs[tmanager->touchTable[i].objId].tposy = manager->objs[tmanager->touchTable[i].objId].posy + tmanager->touchTable[i].movy;
					
					manager->objs[tmanager->touchTable[i].objId].accelerationx = (tmanager->touchTable[i].vposxNow - tmanager->touchTable[i].vposxPre) / REG;
					manager->objs[tmanager->touchTable[i].objId].accelerationy = -(tmanager->touchTable[i].vposyNow - tmanager->touchTable[i].vposyPre) / REG;
					
					manager->objs[tmanager->touchTable[i].objId].t = true;
					
					//サブオブジェクトの移動,加速度
					if (manager->objs[tmanager->touchTable[i].objId].subObjMemberNum > 0) {
						for (int j = 0; j < manager->objs[i].subObjMemberNum; j++) {
							manager->objs[tmanager->touchTable[i].objId].subObjs[j].accelerationx = tmanager->touchTable[i].movx / REG;
							manager->objs[tmanager->touchTable[i].objId].subObjs[j].accelerationy = tmanager->touchTable[i].movy / REG;
						}
					}
				}
			}
			//tableObjならば
			else if (manager->objs[tmanager->touchTable[i].objId].type == tableObj) {
				if(manager->objs[tmanager->touchTable[i].objId].thit == true){
					manager->objs[tmanager->touchTable[i].objId].tposy = manager->objs[tmanager->touchTable[i].objId].posy + tmanager->touchTable[i].movy;
					manager->objs[tmanager->touchTable[i].objId].accelerationy = -(tmanager->touchTable[i].vposyNow - tmanager->touchTable[i].vposyPre) / REG;
					
					manager->objs[tmanager->touchTable[i].objId].t = true;
					
					//サブオブジェクトの移動
					for (int j = 0; j < manager->objs[tmanager->touchTable[i].objId].subObjMemberNum; j++) {
						manager->objs[tmanager->touchTable[i].objId].subObjs[j].tposy = manager->objs[tmanager->touchTable[i].objId].subObjs[j].posy + tmanager->touchTable[i].movy;
					}
					//サブオブジェクトの加速度
					if (manager->objs[tmanager->touchTable[i].objId].subObjMemberNum > 0) {
						for (int j = 0; j < manager->objs[tmanager->touchTable[i].objId].subObjMemberNum; j++) {
							manager->objs[tmanager->touchTable[i].objId].subObjs[j].accelerationy = -(tmanager->touchTable[i].vposyNow - tmanager->touchTable[i].vposyPre) / REG;
						}
					}
				}
			}
		}
	}
}//オブジェクトの操作

bool animateObject(Object *obj,animationType type)
{
	//printf("animateObject\n");
	switch (type) {
		case inertia:
			if (obj->accelerationx == 0 && obj->accelerationy == 0) {
				//printf("false:0\n");
				return false;
			}
			
			//accx
			else if (obj->accelerationx == 0){
				//accy
				if (obj->accelerationy > .005) {
					obj->tposy += obj->accelerationy;
					obj->accelerationy -= obj->accelerationy * FRICTION;
					if (obj->accelerationy <= 0) {
						obj->accelerationy = 0;
						//printf("false\n");
						return false;
					}
				}
				else {
					obj->tposy += obj->accelerationy;
					obj->accelerationy -= obj->accelerationy *FRICTION;
					if (obj->accelerationy >= 0) {
						obj->accelerationy = 0;
						//printf("false\n");
						return false;
					}
				}
			}
			
			//accy
			else if (obj->accelerationy == 0){
				if (obj->accelerationx > .005) {
					obj->tposx += obj->accelerationx;
					obj->accelerationx -= obj->accelerationx * FRICTION;
					if (obj->accelerationx <= .005) {
						obj->accelerationx = 0;
						//printf("false\n");
						return false;
					}
				}
				else {
					obj->tposx += obj->accelerationx;
					obj->accelerationx -= obj->accelerationx * FRICTION;
					if (obj->accelerationx >= .005) {
						obj->accelerationx = 0;
						//printf("false\n");
						return false;
					}
				}
			}
			
			//accx
			else if (obj->accelerationx > .005) {
				obj->tposx += obj->accelerationx;
				obj->accelerationx -= obj->accelerationx * FRICTION;
				if (obj->accelerationx <= .005) {
					obj->accelerationx = 0;
					//printf("false\n");
					return false;
				}
				//accy
				if (obj->accelerationy > .005) {
					obj->tposy += obj->accelerationy;
					obj->accelerationy -= obj->accelerationy * FRICTION;
					if (obj->accelerationy <= .005) {
						obj->accelerationy = 0;
						//printf("false\n");
						return false;
					}
				}
				else {
					obj->tposy += obj->accelerationy;
					obj->accelerationy -= obj->accelerationy *FRICTION;
					if (obj->accelerationy >= .005) {
						obj->accelerationy = 0;
						//printf("false\n");
						return false;
					}
				}
			}
			
			//accx
			else if (obj->accelerationx < .005) {
				obj->tposx += obj->accelerationx;
				obj->accelerationx -= obj->accelerationx * FRICTION;
				if (obj->accelerationx >= .005) {
					obj->accelerationx = 0;
					//printf("false\n");
					return false;
				}
				
				//accy
				if (obj->accelerationy > .005) {
					obj->tposy += obj->accelerationy;
					obj->accelerationy -= obj->accelerationy * FRICTION;
					if (obj->accelerationy <= .005) {
						obj->accelerationy = 0;
						//printf("false\n");
						return false;
					}
				}
				else {
					obj->tposy += obj->accelerationy;
					obj->accelerationy -= obj->accelerationy * FRICTION;
					if (obj->accelerationy >= .005) {
						obj->accelerationy = 0;
						//printf("false\n");
						return false;
					}
				}
			}
			obj->posx = obj->tposx;
			obj->posy = obj->tposy;
			
			if (obj->posx > hx || obj->posx < -hx) {
				obj->accelerationx = -obj->accelerationx;
			}
			if (obj->posy > hy || obj->posy < -hy) {
				obj->accelerationy = -obj->accelerationy;
			}
			
			//printf("true\n");
			return true;
					
		case fade_in:
			
			break;
		
		case fade_out:
			
			break;
			
		default:
			break;
	}
	
	return false;
}