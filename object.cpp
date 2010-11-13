/*
 *  object.cpp
 *  opengl_test
 *
 *  Created by Akira on 10/05/30.
 *  Copyright 2010 東京工業大学付属科学技術高等学校. All rights reserved.
 *
 */

#include "object.h"

extern pngInfo info;

//完成
void setTextureList(GLuint *textures,pngInfo *textureInfo)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glAlphaFunc(GL_GREATER, 0.5);
	
	textures[texButtonObj_unpushed] = pngBind("/Users/akiramatsuda/Desktop/buttonObj_unpushed.png", PNG_BUILDMIPMAP, PNG_ALPHA, &textureInfo[texButtonObj_unpushed], GL_CLAMP, GL_NEAREST, GL_NEAREST);
	
	textures[texButtonObj_pushed] = pngBind("/Users/akiramatsuda/Desktop/buttonObj_pushed.png", PNG_BUILDMIPMAP, PNG_ALPHA, &textureInfo[texButtonObj_pushed], GL_CLAMP, GL_NEAREST, GL_NEAREST);
	
	textures[texSwitchObj_off] = pngBind("/Users/akiramatsuda/Desktop/switchObj_off.png", PNG_BUILDMIPMAP, PNG_ALPHA, &textureInfo[texSwitchObj_off], GL_CLAMP, GL_NEAREST, GL_NEAREST);
	
	textures[texSwitchObj_on] = pngBind("/Users/akiramatsuda/Desktop/switchObj_on.png", PNG_BUILDMIPMAP, PNG_ALPHA, &textureInfo[texSwitchObj_on], GL_CLAMP, GL_NEAREST, GL_NEAREST);
	
	textures[texRectObj] = pngBind("/Users/akiramatsuda/Desktop/rectObj.png", PNG_BUILDMIPMAP, PNG_ALPHA, &textureInfo[texRectObj], GL_CLAMP, GL_NEAREST, GL_NEAREST);
	
	textures[texAlartObj] = pngBind("/Users/akiramatsuda/Desktop/alartObj.png", PNG_BUILDMIPMAP, PNG_ALPHA, &textureInfo[texAlartObj], GL_CLAMP, GL_NEAREST, GL_NEAREST);
	
	textures[texTableObj] = pngBind("/Users/akiramatsuda/Desktop/tableObj.png", PNG_BUILDMIPMAP, PNG_ALPHA, &textureInfo[texTableObj], GL_CLAMP, GL_NEAREST, GL_NEAREST);
}

void setTexture(GLuint *textures,pngInfo *textureInfo,int texNum,char *path)
{
	textures[texNum] = pngBind(path, PNG_BUILDMIPMAP, PNG_ALPHA, &textureInfo[texNum], GL_CLAMP, GL_NEAREST, GL_NEAREST);
}

void addSubObject(Object *obj,subObject subObj)
{
	//サブオブジェクトの追加
	
	printf("add sub object\n");
	if(obj->subObjMemberNum < MAX_SUB_OBJECT){
		obj->subObjs[obj->subObjMemberNum] = subObj;
		obj->subObjMemberNum++;
	}
	else printf("Member limit!\n");
}

//完成
void drawDebugObjectOutline(Object obj)
{
	glBegin(GL_LINE_LOOP);
	glColor3d(1.0, 0.0, 0.0);//R
	glVertex2f(obj.posx + obj.sizex, obj.posy + obj.sizey);
	glColor3d(0.0, 1.0, 0.0);//G
	glVertex2f(obj.posx + obj.sizex, obj.posy - obj.sizey);
	glColor3d(0.0, 0.0, 1.0);//B
	glVertex2f(obj.posx - obj.sizex, obj.posy - obj.sizey);
	glColor3d(1.0, 1.0, 0.0);//RG
	glVertex2f(obj.posx - obj.sizex, obj.posy + obj.sizey);
	glEnd();
	
	glBegin(GL_POINTS);
	glColor3d(1.0, 0.0, 0.0);
	glVertex2f(obj.posx, obj.posy);
	glEnd();
}

//完成
void drawDebugAxis(void)
{
	glBegin(GL_LINES);
	glColor3d(1.0, 1.0, 0.0);
	glVertex3d(0.0, hy, 0.0);
	glVertex3d(0.0, -hy, 0.0);
	glVertex3d(hx, 0.0, 0.0);
	glVertex3d(-hx, 0.0, 0.0);
	glEnd();
}

//完成
void initObjectWithType(double x,double y,double posx,double posy,Object *obj,objType type,int objId, bool status, const GLuint textureList)
{
	obj->objId = objId;
	obj->sizex = x/2.0;
	obj->sizey = y/2.0;
	obj->scale = 1.0;
	obj->angle = 0.0;
	obj->alpha = 1.0;
	obj->posx = posx;
	obj->posy = posy;
	obj->tposx = posx;
	obj->tposy = posy;
	obj->accelerationx = 0;
	obj->accelerationy = 0;
	obj->touchIds[0] = -1;
	obj->touchIds[1] = -1;
	obj->touchNum = 0;
	obj->type = type;
	obj->subObjMemberNum = 0;
	obj->resource.status = 0;
	obj->resource.texture = textureList;
	obj->thit = false;
	obj->t = false;
	obj->accMode = true;
	obj->drugMode = status;
	obj->action = printObjectType;//insert your original function
}

Object getObjectWithType(double x,double y,double posx,double posy,objType type,bool status,int objId,GLuint textureList)
{	
	Object obj;
	obj.objId = objId;
	obj.sizex = x/2.0;
	obj.sizey = y/2.0;
	obj.scale = 1.0;
	obj.angle = 0.0;
	obj.alpha = .5;
	obj.posx = posx;
	obj.posy = posy;
	obj.tposx = posx;
	obj.tposy = posy;
	obj.accelerationx = 0;
	obj.accelerationy = 0;
	obj.touchIds[0] = -1;
	obj.touchIds[1] = -1;
	obj.touchNum = 0;
	obj.type = type;
	obj.subObjMemberNum = 0;
	obj.resource.status = 0;
	obj.resource.texture = textureList;
	strcpy(obj.resource.message,"alart message");
	obj.thit = false;
	obj.t = false;
	obj.accMode = true;
	obj.drugMode = status;
	obj.action = printObjectType;//insert your original function
	
	return obj;
}

//未確認
void setObjectWithMessage(Object *obj,const char *message)
{
	//printf("setAlartMessage\n");
	strcpy(obj->resource.message, message);
	//*obj->resource.message = *message;
}

void setSubObjectWithMessage(subObject *subObj,const char *message)
{
	//printf("setAlartMessage\n");
	strcpy(subObj->resource.message, message);
	//*obj->resource.message = *message;
}

//未確認
void initSubObjectWithType(subObject *subObj,subObjType type)
{
	switch (type) {
		case cellObj:
			subObj->sizex = 0.5;
			subObj->sizey = 0.5;
			break;
			
		case indexObj:
			subObj->sizex = 0.05;
			subObj->sizey = 1.0;
			break;
			
		case scrollObj:
			subObj->sizex = 0.05;
			subObj->sizey = 0.5;
			break;
			
		default:
			printf("Object type error.\n");
			break;
	}
	
	subObj->subType = type;
	subObj->touchMode = false;
}

//完成
subObject getSubObjectWithType(subObjType type)
{
	subObject subObj;
	
	switch (type) {
		case cellObj:
			subObj.sizex = 0.5;
			subObj.sizey = 0.5;
			break;
			
		case indexObj:
			subObj.sizex = 0.05;
			subObj.sizey = 1.0;
			break;
			
		case scrollObj:
			subObj.sizex = 0.05;
			subObj.sizey = 0.5;
			break;
			
		default:
			printf("Object type error.\n");
			break;
	}
	
	subObj.subType = type;
	subObj.touchMode = false;
	
	return subObj;
}

//完成
void drawCircle(double x,double y)
{
	//printf("drawCicle\n");
	//円の描画
	static const float PI_OVER_180 = 0.0174532925; //定数
	static float angle,step,StartAngle;
	step = 360.0 / 10; //分割数からステップ数を計算
	
	glBegin(GL_POLYGON);
	for(angle = 0; angle < 360.0; angle += step){
		StartAngle = angle * PI_OVER_180;
		glColor3d(1.0, 1.0, 1.0);
		glVertex2f(0.02 * cos(StartAngle) + x,  0.02 * sin(StartAngle) + y);
	}
	glEnd();
}

//未完成
void buildTableObjectWithSubObject(Object *obj,int subObjNum)
{
	for (int i = 0; i < subObjNum; i++) {
		addSubObject(obj, getSubObjectWithType(cellObj));
		obj->subObjs[i].posx = -(hx - .5);
		obj->subObjs[i].posy = (hy - .5) - (i * 1.0);
		obj->subObjs[i].tposy = obj->subObjs[i].posy;
		setSubObjectWithMessage(&obj->subObjs[i], "cell");
	}
}

//完成
void dumpObject(Object obj)
{
	printf("-------dumpObject--------\n");
	printf("No:%d\n",obj.objId);
	printf("sizex:%lf\n",obj.sizex);
	printf("sizey:%lf\n",obj.sizey);
	printf("posx:%lf\n",obj.posx);
	printf("posy:%lf\n",obj.posy);
	printf("tposx:%lf\n",obj.tposx);
	printf("tposy:%lf\n",obj.tposy);
	printf("accx:%lf\n",obj.accelerationx);
	printf("accy:%lf\n",obj.accelerationy);
	printf("scale:%lf\n",obj.scale);
	printf("thit:%d\n",obj.thit);
	printf("t:%d\n",obj.t);
	printf("d:%d\n",obj.drugMode);
	printf("tn:%d\n",obj.touchNum);
	printf("tId:%d,%d\n",obj.touchIds[0],obj.touchIds[1]);
	
	switch (obj.type) {
		case buttonObj:
			printf("type:buttonObj\n");
			break;
			
		case switchObj:
			printf("type:swichObj:");
			printf("status:%d\n",obj.resource.status);
			break;
			
		case tableObj:
			printf("type:tableObj\n");
			break;
			
		case rectObj:
			printf("type:rectObj\n");
			break;
			
		case alartObj:
			printf("type:alartObj\n");
			printf("message:%s\n",obj.resource.message);
			break;
			
		case imageObj:
			printf("type:imageObj\n");
			break;
			
		case planeObj:
			printf("type:planeObj\n");
			break;
			
		case menuObj:
			printf("type:tableObj\n");
			break;
			
		case nullObject:
			printf("type:nullObj\n");
			break;
			
		default:
			break;
	}
	
	switch (obj.resource.texture) {
		case texButtonObj_pushed:
			printf("texture:buttonObj_pushed %d\n",obj.resource.texture);
			break;
		case texButtonObj_unpushed:
			printf("texture:buttonObj_unpushed %d\n",obj.resource.texture);
			break;
		case texSwitchObj_on:
			printf("texture:switchObj_on %d\n",obj.resource.texture);
			break;
		case texSwitchObj_off:
			printf("texture:switchObj_off %d\n",obj.resource.texture);
			break;
		case texRectObj:
			printf("texture:rectObj %d\n",obj.resource.texture);
			break;
		case texAlartObj:
			printf("texture:alartObj %d\n",obj.resource.texture);
			break;
		case texTableObj:
			printf("texture:tableObj %d\n",obj.resource.texture);
			break;
		default:
			break;
	}
	printf("subO:%d\n",obj.subObjMemberNum);
}

void dumpSubObject(Object obj)
{
	if (obj.subObjMemberNum > 0) {
		printf("subObj:%d\n",obj.subObjMemberNum);
		switch (obj.type) {
			case buttonObj:
				printf("type:buttonObj\n");
				break;
				
			case switchObj:
				printf("type:swichObj:");
				printf("status:%d\n",obj.resource.status);
				break;
				
			case tableObj:
				printf("type:tableObj\n");
				break;
				
			case rectObj:
				printf("type:rectObj\n");
				break;
				
			case alartObj:
				printf("type:alartObj\n");
				break;
				
			case imageObj:
				printf("type:imageObj\n");
				break;
				
			case planeObj:
				printf("type:planeObj\n");
				break;
				
			case menuObj:
				printf("type:tableObj\n");
				break;
				
			case nullObject:
				printf("type:nullObj\n");
				break;
				
			default:
				break;
		}
		
		printf(">subObj\n");
		for (int i = 0; i < obj.subObjMemberNum; i++) {
			printf("\n");
			switch (obj.subObjs[i].subType)
			{
				case cellObj:
					printf(">type:cellObj:%d\n",i);
					break;
					
				case indexObj:
					printf(">type:indexObj\n");
					break;
					
				case scrollObj:
					printf(">type:scrollObj\n");
					break;
					
				default:
					break;
			}
			
			printf(">sizex:%lf\n",obj.subObjs[i].sizex);
			printf(">sizey:%lf\n",obj.subObjs[i].sizey);
			printf(">posx:%lf\n",obj.subObjs[i].posx);
			printf(">posy:%lf\n",obj.subObjs[i].posy);
			printf(">accx:%lf\n",obj.subObjs[i].accelerationx);
			printf(">accy:%lf\n",obj.subObjs[i].accelerationy);
		}
	}
	else printf("No sub object.\n");
}

void printObjectType(objType type,...)
{
	va_list list;
	va_start(list,type);
	
	switch (type) {
		case buttonObj:
			printf("type:buttonObj\n");
			break;
			
		case switchObj:
			printf("type:swichObj:");
			break;
			
		case tableObj:
			printf("type:tableObj\n");
			break;
			
		case rectObj:
			printf("type:rectObj\n");
			break;
			
		case alartObj:
			printf("type:alartObj\n");
			break;
			
		case imageObj:
			printf("type:imageObj\n");
			break;
			
		case planeObj:
			printf("type:planeObj\n");
			break;
			
		case menuObj:
			printf("type:tableObj\n");
			break;
			
		case nullObject:
			printf("type:nullObj\n");
			break;
			
		default:
			break;
	}	
}
