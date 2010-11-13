/*
 *  FRConroller.h
 *  opengl_test
 *
 *  Created by Akira on 10/09/12.
 *  Copyright 2010 東京工業大学付属科学技術高等学校. All rights reserved.
 *
 */

#ifndef FR_CONTROLLER
#define FR_CONTROLLER

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <strings.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "object.h"
#include "viewManager.h"
#include "parameter.h"
#include "displayManager.h"

typedef enum {
	acceleratorUp,
	acceleratorDown,
	acceleratorRight,
	acceleratorLeft,
	acceleratorNone
}acceleratorMode;

typedef struct{
	int xNow,yNow,zNow;
	int xPre,yPre,zPre;
	acceleratorMode mode;
}FRControllerResource;

typedef struct{
	int fd;
	struct termios oldtio;
	FRControllerResource resource;
	void (*action)(FRControllerResource *frcRes,displayManager *dmanager,...);//オブジェクトのアクション
}FRController;

int InitializeHandle(struct termios *oldtio);
void ReadLine(int fd, char *result, int length);
void ReadXYZValue(int fd, int *x, int *y, int *z);
void ReleaseHandle(int fd, struct termios *oldtio);

void initWithFRController(FRController *frc);
void FRTimer(FRController *frc,displayManager *dmanager);
void callFuncFRAcceleration(FRController frc,viewManager *manager);
void setAccelerationWithFRController(FRController *frc, int x, int y, int z);
void FRAction(FRControllerResource *frcRes,displayManager *dmanager,...);

#endif