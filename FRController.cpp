/*
 *  FRConroller.cpp
 *  opengl_test
 *
 *  Created by Akira on 10/09/12.
 *  Copyright 2010 東京工業大学付属科学技術高等学校. All rights reserved.
 *
 */

#include "FRController.h"

int InitializeHandle(struct termios *oldtio)
{
	int fd;
	struct termios newtio;
	
	if ((fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY)) == -1) {
		fprintf(stderr, "FR:failed to create file descripter!\n");
		exit(-1);
	}
	
	tcgetattr(fd, oldtio);
	bzero(&newtio, sizeof(newtio));
	newtio.c_cflag = (BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD);
	newtio.c_iflag = (IGNPAR | ICRNL);
	newtio.c_oflag = 0;
	newtio.c_lflag = ICANON;
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &newtio);
	
	return fd;
}

void ReadLine(int fd, char *result, int length)
{
	unsigned long readBytes;
	char buffer[1];
	int i = 0;
	
	while(1) {
		readBytes = read(fd, buffer, 1);
		if (readBytes == 1) {
			if (*buffer == 0x0a || *buffer == 0x0d) {
				if (i != 0) {
					result[i] = '\0';
					i = 0;
					break;
				}
			} else if (i < length - 1) { 
				result[i++] = *buffer;
			}
		}
	}
}

void ReadXYZValue(int fd, int *x, int *y, int *z)
{
	char str[CHAR_MAX_LENGTH];
	char *xStr, *yStr, *zStr;
	
	ReadLine(fd, str, CHAR_MAX_LENGTH);
	
	xStr = strtok(str, ",");
	yStr = strtok(NULL, ",");
	zStr = strtok(NULL, ",");
	
	if (xStr == NULL || yStr == NULL || zStr == NULL) {
		fprintf(stderr, "FR:failed to read xyz value!\n");
		exit(-1);
	} else {
		*x = atoi(xStr);
		*y = atoi(yStr);
		*z = atoi(zStr);
	}
}

void ReleaseHandle(int fd, struct termios *oldtio)
{
	tcsetattr(fd, TCSANOW, oldtio);
	close(fd);
}

void initWithFRController(FRController *frc)
{
	frc->fd = InitializeHandle(&frc->oldtio);
	frc->resource.xNow = 0;
	frc->resource.yNow = 0;
	frc->resource.zNow = 0;
	frc->resource.xPre = 0;
	frc->resource.yPre = 0;
	frc->resource.zPre = 0;
	frc->resource.mode = acceleratorNone;
	frc->action = FRAction;
}

void FRTimer(FRController *frc,displayManager *dmanager)
{
	int x,y,z;
	ReadXYZValue(frc->fd, &x, &y, &z);
	x -= 510;
	y -= 560;
	z -= 290;
	setAccelerationWithFRController(frc, x, y, z);
	frc->action(&frc->resource,dmanager);
	//printf("%d,%d,%d\n",frc->resource.xNow,frc->resource.yNow,frc->resource.zNow);
	//printf("%d,%d,%d\n",x,y,z);
}

void callFuncFRAcceleration(FRController frc,viewManager *manager)
{
	static bool check = false;
	//printf("%4d,%4d,%4d\n",frc.xNow,frc.xPre,abs(frc.xNow - frc.xPre));
	if (abs(frc.resource.xNow - frc.resource.xPre) > 250 && check) {
		printf("shock!:%5d\n",abs(frc.resource.xNow - frc.resource.xPre));
		check = false;
		return;
	}
	check = true;
	
	return;
}

void setAccelerationWithFRController(FRController *frc, int x, int y, int z)
{
	frc->resource.xPre = frc->resource.xNow;
	frc->resource.yPre = frc->resource.yNow;
	frc->resource.zPre = frc->resource.zNow;
	frc->resource.xNow = x;
	frc->resource.yNow = y;
	frc->resource.zNow = z;
	
	//insert judgement code
	
}

void FRAction(FRControllerResource *frcRes,displayManager *dmanager,...)
{
	static bool check = false;
	//printf("%4d,%4d,%4d\n",frc.xNow,frc.xPre,abs(frc.xNow - frc.xPre));
	if (frcRes->xNow - frcRes->xPre > 500 && check) {
		printf("shock!:%5d\n",abs(frcRes->xNow - frcRes->xPre));
		if (dmanager->diplayId < MAX_VIEW_MANAGER) {
			printf("in++ %d\n",dmanager->diplayId);
			dmanager->diplayId++;
		}
		check = false;
		return;
	}
	else if	(frcRes->xNow - frcRes->xPre < 500 && check){
		if (dmanager->diplayId > 0) {
			printf("in-- %d\n",dmanager->diplayId);
			dmanager->diplayId--;
		}
		check = false;
		return;
	}
	
	/*
	//printf("%d,%d,%d\n",frcRes->xNow,frcRes->yNow,frcRes->zNow);
	for (int i = 0; i < dmanager->manager.objMemberNum; i++) {
		if (dmanager->manager.objs[i].thit == false) {
			dmanager->manager.objs[i].accelerationx += (double)((frcRes->xNow - 242) * .00001);
			dmanager->manager.objs[i].accelerationy += (double)((frcRes->yNow + 35) * .00001);
		}
	}
	 */
	
	check = true;
	
	return;
}