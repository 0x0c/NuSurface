/*
 *  multitouchLibrary.c
 *  opengl_test
 *
 *  Created by Akira on 10/07/31.
 *  Copyright 2010 東京工業大学付属科学技術高等学校. All rights reserved.
 *
 */

#include "multi-touchLib.h"

bool clicked = false;
CvPoint start, end;
IplImage *src = NULL, *dst = NULL;

void MTFinalaze(MTCaptureImg *cap)
{
	cvReleaseCapture(&cap->cap);
	cvDestroyAllWindows();
	cvReleaseMemStorage(&cap->contourStorage);
	cvReleaseImage(&cap->maskImg);
	cvReleaseImage(&cap->maskedImg);
	cvReleaseImage(&cap->hsvImg);
	cvReleaseImage(&cap->tmpImg);
	cvReleaseImage(&cap->resultImg);
	
	return;
}

void MTDisplay(MTCaptureImg *cap)
{
	char num[32] = "";
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	
	for(int i = 0; i < cap->tnum; i++) {
		sprintf(num, "%d", i + 1);
		cvPutText(cap->capImg, num, cvPoint(cap->touchNow[i].x, cap->touchNow[i].y), &cap->font, CV_RGB(0xff, 0, 0));
		sprintf(num, "%d/%d:%3d,%3d", i + 1, cap->tnum, cap->touchNow[i].x, cap->touchNow[i].y);
		cvPutText(cap->capImg, num, cvPoint(10, 30 * (i + 1)), &cap->font, CV_RGB(0xff,0,0));
	}
	
	glPixelZoom(1, -1);
	glRasterPos2d(-1.0, 1.0);
	cvCvtColor(cap->capImg, cap->capImg, CV_BGR2RGB);
	
	glDrawPixels(cap->capImg->width, cap->capImg->height, GL_RGB, GL_UNSIGNED_BYTE, cap->capImg->imageData);
	
	glutSwapBuffers();
}

void MTTimer(MTCaptureImg *cap,viewManager *manager,touchManager *tmanager)
{	
	int cnt = 0;
	CvPoint center = cvPoint(0,0);
	
	cap->capImg = cvQueryFrame(cap->cap);
	if (cap->calibrated == true) {
		getCameraImageWithCalibrate(cap);
	}
	
	cvCvtColor(cap->capImg, cap->hsvImg, CV_BGR2HSV);
	cvCopy(cap->hsvImg, cap->tmpImg);
	cvThreshold(cap->tmpImg, cap->maskImg, (double)cap->trackbarValue01 / 2, 255, CV_THRESH_BINARY);
	
	cvCopy(cap->maskImg, cap->tmpImg);
	cvFindContours(cap->tmpImg, cap->contourStorage, &cap->contours, sizeof(CvContour), CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	
	cvDistTransform(cap->maskImg, cap->distImg, CV_DIST_L2, 3, NULL, NULL);
	cvNormalize(cap->distImg, cap->distNormImg, 0.0, 1.0, CV_MINMAX, NULL);
	
	//タッチの格納
	
	if(cap->contours != NULL) {
		cvInitTreeNodeIterator (&cap->it, cap->contours, 1);
		while ((cap->contours = (CvSeq *) cvNextTreeNode (&cap->it)) != NULL) {
			if(cnt < MAX_TOUCHES){
				cvSetZero(cap->maskImg);
				cvDrawContours(cap->maskImg, cap->contours, cvScalarAll(255), cvScalarAll(255), 1, -1);
				cvMinMaxLoc(cap->distNormImg, 0, 0, 0, &center, cap->maskImg);
				if ((cap->touchNow[cnt-1].x == center.x)&(cap->touchNow[cnt-1].y == center.y))
					continue;
				
				cap->touchPre[cnt].x = cap->touchNow[cnt].x;
				cap->touchPre[cnt].y = cap->touchNow[cnt].y;
				cap->touchNow[cnt].x = center.x;
				cap->touchNow[cnt].y = center.y;
				
				cnt++;
				
				cvCircle(cap->capImg, center, 3, CV_RGB(0xff, 0, 0), -1);
			}
		}
	}
	
	for (int i = cnt; i < MAX_TOUCHES; i++) {
		cap->touchPre[i].x = -1;
		cap->touchPre[i].y = -1;
		cap->touchNow[i].x = -1;
		cap->touchNow[i].y = -1;
	}
	cap->tnum = cnt;
	
	setTouches(cap, tmanager,manager);
	
	return;
}

void MTKeyboard(MTCaptureImg *cap,unsigned char key, int x, int y)
{	
	switch(key) {
		case 'q':
		case 'Q':
			MTFinalaze(cap);
			exit(0);
			break;
			
			/*
			 case 'f':
			 case 'F':
			 if(isFullscreen) {
			 glutReshapeWindow(cap->capImg->width, cap->capImg->height);
			 glutPositionWindow(100, 100);
			 isFullscreen = false;
			 }
			 else {
			 glutFullScreen();
			 isFullscreen = true;
			 }
			 break;
			 */	
		default:
			break;
	}
}

void initWithMTCaptureImg(MTCaptureImg *cap)
{
	cap->trackbarValue01 = 127 * 3;
	cap->trackbarValue02 = 0;
	cap->trackbarValue03 = 0;
	
	cap->capImg = 0;
	cap->maskImg = 0;
	cap->hsvImg = 0;
	cap->maskedImg = 0;
	cap->distImg = 0;
	cap->distNormImg = 0;
	cap->tmpImg = 0;
	cap->resultImg = 0;
	
	cap->contourStorage = 0;
	cap->contours = 0;
	cap->point = 0;
	cap->calibrated = false;
}

void MTInit(MTCaptureImg *cap,int argc,char **argv)
{
	if((cap->cap = cvCaptureFromCAM(1)) == NULL ) {
		puts("could not open Capture Device.");
		puts("press enter key to exit...");
		getc(stdin);
		return;
	}
	
	initWithMTCaptureImg(cap);
	
	// webcamから画像を取ってくる
	cap->capImg = cvQueryFrame(cap->cap);
	
	// いろいろ使うバッファの確保
	cap->tmpImg = cvCreateImage(cvGetSize(cap->capImg), IPL_DEPTH_8U, 1);
	cap->maskImg = cvCreateImage(cvGetSize(cap->capImg), IPL_DEPTH_8U, 1);
	cap->distImg = cvCreateImage(cvGetSize(cap->capImg), IPL_DEPTH_32F, 1);
	cap->distNormImg = cvCreateImage(cvGetSize(cap->capImg), IPL_DEPTH_32F, 1);
	cap->resultImg = cvCreateImage(cvGetSize(cap->capImg), IPL_DEPTH_32F, 3);
	cap->contourStorage = cvCreateMemStorage(0);
	
	cap->touchNow[0].x = -1;
	cap->touchNow[0].y = -1;
	cap->touchPre[0].x = -1;
	cap->touchPre[0].y = -1;
	
	// いろいろ使うのでコピー
	cap->maskedImg = cvCloneImage(cap->capImg);
	cap->hsvImg = cvCloneImage(cap->capImg);
	
	// hsv色空間に変換した画像の3ch目だけ使う(H:色相,S:彩度,V:明度)
	cvSetImageCOI(cap->hsvImg, 3);
	
	// フォントの初期化
	cvInitFont(&cap->font, CV_FONT_HERSHEY_PLAIN, 1.0, 1.0);
	
	//閾値の設定
	cvNamedWindow("cvWindow", CV_WINDOW_AUTOSIZE);
	cvCreateTrackbar("trackbar01", "cvWindow", &cap->trackbarValue01, 255*2, NULL);
	cvSetTrackbarPos("trackbar01", "cvWindow", 150);
	//cvCreateTrackbar("trackbar02", "cvWindow", &cap->trackbarValue02, 10, NULL);
	//cvSetTrackbarPos("trackbar02", "cvWindow", 1);
	
	return;
}

void MTCalibration(tmpPoint *tmp,double x,double y)
{
	tmp->xPre = tmp->xNow;
	tmp->yPre = tmp->yNow;
	//中心より右
	if (x > ISIGHT_RES_WIDTH_HALF) {
		//中心より下
		if (y > ISIGHT_RES_HEIGHT_HALF) {
			//printf("B\n");
			tmp->xNow = (hx - ((x / ISIGHT_RES_WIDTH_HALF) * hx));//x
			tmp->yNow = (hy - ((y / ISIGHT_RES_HEIGHT_HALF) * hy));//y
		}
		//中心より上
		else {
			//printf("C\n");
			tmp->xNow = (hx - ((x / ISIGHT_RES_WIDTH_HALF) * hx));//x
			tmp->yNow = -((y - ISIGHT_RES_HEIGHT_HALF) / ISIGHT_RES_HEIGHT_HALF) * hy;//y
		}
	}
	//中心より左
	else {
		//中心より下
		if (y > ISIGHT_RES_HEIGHT_HALF) {
			//printf("A\n");
			tmp->xNow = -((x - ISIGHT_RES_WIDTH_HALF) / ISIGHT_RES_WIDTH_HALF) * hx;//x
			tmp->yNow = (hy - ((y / ISIGHT_RES_HEIGHT_HALF) * hy));//y
		}
		//中心より上
		else {
			//printf("D\n");
			tmp->xNow = -((x - ISIGHT_RES_WIDTH_HALF) / ISIGHT_RES_WIDTH_HALF) * hx;//x
			tmp->yNow = -((y - ISIGHT_RES_HEIGHT_HALF) / ISIGHT_RES_HEIGHT_HALF) * hy;//y
		}		
	}
}

void setTouches(MTCaptureImg *cap,touchManager *tmanager,viewManager *manager)
{
	tmpPoint tmpP;
	bool vaildTouch[MAX_TOUCHES];
	int i,j;
	int cnt = 0,nw = 0,rl = 0;
	
	//init vaild touch
	for (i = 0; i < MAX_TOUCHES; i++) vaildTouch[i] = false;
	
	if (cap->tnum > 0) {
		for (i = 0; i < MAX_TOUCHES; i++) {//now
			for (j = i; j < MAX_TOUCHES; j++) {//pre
				
				if (cap->touchNow[j].x == -1 && cap->touchNow[j].y == -1) break;
				
				if (vaildTouch[j] == false) {
					if (abs(cap->touchNow[j].x - cap->touchPre[j].x) < TOUCH_DIST && abs(cap->touchNow[j].y - cap->touchPre[j].y) < TOUCH_DIST) {//同一と判断
						
						//引継ぎ
						//タッチのキャリブレーション
						MTCalibration(&tmpP,cap->touchNow[j].x,cap->touchNow[j].y);//タッチのキャリブレーション
						if (abs(cap->touchNow[i].x - cap->touchPre[i].x) > TOUCH_C_DIST && abs(cap->touchNow[i].y - cap->touchPre[i].y) > TOUCH_C_DIST) {
							setTouchPosition(&tmanager->touchTable[j],tmpP.xNow,-tmpP.yNow );//タッチに新しい座標を代入
						}
						
						vaildTouch[j] = true;//有効なタッチの記憶
						cnt++;
					}
					else {
						MTCalibration(&tmpP,cap->touchNow[j].x,cap->touchNow[j].y);//タッチのキャリブレーション
						addTouchObject(tmanager, getTouchObject(tmpP.xNow, tmpP.yNow));//追加
						vaildTouch[j] = true;//有効なタッチの記憶
						nw++;
					}
				}
			}
		}
	}
	
	//解放
	//printf("v:");
	for (i = 0;	i < MAX_TOUCHES; i++) {
		//printf("%d,",vaildTouch[i]);
		if (vaildTouch[i] == false) {
			
			rl++;
			
			if (manager->objs[tmanager->touchTable[i].objId].thit == true) {
				manager->objs[tmanager->touchTable[i].objId].touchMode = true;
				manager->objs[tmanager->touchTable[i].objId].t = false;
				manager->objs[tmanager->touchTable[i].objId].thit = false;
				if (manager->objs[tmanager->touchTable[i].objId].subObjMemberNum > 0) {
					for (int j = 0; j < manager->objs[tmanager->touchTable[i].objId].subObjMemberNum; j++) {
						manager->objs[tmanager->touchTable[i].objId].subObjs[j].touchMode = true;
					}
				}
			}
			
			if (tmanager->touchTable[i].touchId == manager->objs[tmanager->touchTable[i].objId].touchIds[0]) {
				printf("rm:tId[0]\n");
				manager->objs[tmanager->touchTable[i].objId].touchIds[0] = -1;
				manager->objs[tmanager->touchTable[i].objId].touchNum--;
				continue;
			}
			if (tmanager->touchTable[i].touchId == manager->objs[tmanager->touchTable[i].objId].touchIds[1]) {
				printf("rm:tId[1]\n");
				manager->objs[tmanager->touchTable[i].objId].touchIds[1] = -1;
				manager->objs[tmanager->touchTable[i].objId].touchNum--;
			}
			
			manager->objs[tmanager->touchTable[i].objId].posx = manager->objs[tmanager->touchTable[i].objId].tposx;
			manager->objs[tmanager->touchTable[i].objId].posy = manager->objs[tmanager->touchTable[i].objId].tposy;
			
			//buttonObjの後処理
			if (manager->objs[tmanager->touchTable[i].objId].type == buttonObj) manager->objs[tmanager->touchTable[i].objId].resource.texture = manager->textureList[texButtonObj_unpushed];
			removeTouchObject(tmanager, i);
		}
	}
	
	//printf("\n.");
	//printf("cnt:%3d nw:%3d rl:%3d\n",cnt,nw,rl);
	
	//タッチとドラッグの動作
	//当たり判定
	MTDrug(tmanager, manager);
}

void MTDrug(touchManager *tmanager,viewManager *manager)
{
	//printf("%d\n", manager->objs[0].touchIds[0]);
	//printf("%d\n", manager->objs[0].touchIds[1]);
	
	for (int i = 0; i < MAX_TOUCHES; i++) {
		if (tmanager->touchTable[i].touchId != -1) {
			for (int j = 0; j < manager->objMemberNum; j++) {
				//ブジェクトの種類ごとの条件分岐
				switch (manager->objs[j].type) {
					case buttonObj:
						if(hitCondition){
							if (manager->objs[j].thit == false) {
								manager->objs[j].thit = true;
								manager->objs[j].touchMode = false;
								manager->objs[j].accMode = true;
								manager->objs[j].resource.texture = manager->textureList[texButtonObj_pushed];
								
								tmanager->touchTable[i].objId = manager->objs[j].objId;
								if (manager->objs[j].drugMode == true) {
									manager->objs[j].resource.texture = manager->textureList[texButtonObj_pushed];
									
									manager->objs[j].posx = manager->objs[j].tposx;
									manager->objs[j].posy = manager->objs[j].tposy;
									manager->objs[j].accelerationx = 0;
									manager->objs[j].accelerationy = 0;
								}
								manager->objs[j].action(manager->objs[j].type);
							}
							else {
								manager->objs[tmanager->touchTable[i].objId].t = true;
								manager->objs[j].accMode = false;
								if (manager->objs[j].drugMode == true) {
									manager->objs[tmanager->touchTable[i].objId].tposx = manager->objs[tmanager->touchTable[i].objId].posx + tmanager->touchTable[i].movx;
									manager->objs[tmanager->touchTable[i].objId].tposy = manager->objs[tmanager->touchTable[i].objId].posy + tmanager->touchTable[i].movy;
									
									manager->objs[tmanager->touchTable[i].objId].accelerationx = (tmanager->touchTable[i].vposxNow - tmanager->touchTable[i].vposxPre) / REG;
									manager->objs[tmanager->touchTable[i].objId].accelerationy = -(tmanager->touchTable[i].vposyNow - tmanager->touchTable[i].vposyPre) / REG;
								}
							}
						}
						break;
						
					case switchObj:
						if(hitCondition){
							if (manager->objs[j].thit == false) {
								manager->objs[j].thit = true;
								if (manager->objs[j].resource.status == false){
									manager->objs[j].resource.status = true;
									manager->objs[j].resource.texture = manager->textureList[texSwitchObj_on];
								}
								else {
									manager->objs[j].resource.status = false;
									manager->objs[j].resource.texture = manager->textureList[texSwitchObj_off];
								}
								
								tmanager->touchTable[i].objId = manager->objs[j].objId;
								
								manager->objs[j].posx = manager->objs[j].tposx;
								manager->objs[j].posy = manager->objs[j].tposy;
								manager->objs[j].accelerationx = 0;
								manager->objs[j].accelerationy = 0;
								manager->objs[j].touchMode = false;
								
								manager->objs[j].action(manager->objs[j].type);
							}
						}
						break;
						
					case imageObj:
						if(hitCondition){
							if (manager->objs[j].thit == false) {
								manager->objs[j].thit = true;
								
								tmanager->touchTable[i].objId = manager->objs[j].objId;
								
								//
								manager->objs[j].posx = manager->objs[j].tposx;
								manager->objs[j].posy = manager->objs[j].tposy;
								manager->objs[j].accelerationx = 0;
								manager->objs[j].accelerationy = 0;
								manager->objs[j].touchMode = false;
								//
								
								manager->objs[j].action(manager->objs[j].type);
							}
						}
						break;
						
					case planeObj:
						if(hitCondition){
							if (manager->objs[j].thit == false) {
								tmanager->touchTable[i].objId = manager->objs[j].objId;
								
								manager->objs[j].action(manager->objs[j].type);
							}
						}
						break;
						
					case menuObj:
						if(hitCondition){
							if (manager->objs[j].thit == false) {
								tmanager->touchTable[i].objId = manager->objs[j].objId;
								
								manager->objs[j].action(manager->objs[j].type);
							}
						}
						break;
						
					case tableObj:
						if(hitCondition){
							if (manager->objs[j].thit == false) {
								manager->objs[j].thit = true;
								
								tmanager->touchTable[i].objId = manager->objs[j].objId;
								
								//
								manager->objs[j].posx = manager->objs[j].tposx;
								manager->objs[j].posy = manager->objs[j].tposy;
								manager->objs[j].accelerationx = 0;
								manager->objs[j].accelerationy = 0;
								manager->objs[j].touchMode = false;
								//
								
								if (manager->objs[j].subObjMemberNum > 0) {
									for (int j = 0; j < manager->objs[j].subObjMemberNum; j++) {
										
										//
										manager->objs[j].subObjs[j].accelerationx = 0;
										manager->objs[j].subObjs[j].accelerationy = 0;
										manager->objs[j].subObjs[j].touchMode = false;
										//
										
									}
								}
								
								manager->objs[j].action(manager->objs[j].type);
							}
						}
						break;
						
					case alartObj:
						if(hitCondition){
							if (manager->objs[j].thit == false) {
								tmanager->touchTable[i].objId = manager->objs[j].objId;
								manager->objs[j].action(manager->objs[j].type);
								removeObject(manager,j);
							}
						}
						break;
						
					case rectObj:
						if(hitCondition){
							if (manager->objs[j].thit == false) {//はなされた
								manager->objs[j].thit = true;
								manager->objs[j].touchMode = false;
								manager->objs[j].accMode = true;
								
								if (manager->objs[j].drugMode == true) {	
									manager->objs[j].posx = manager->objs[j].tposx;
									manager->objs[j].posy = manager->objs[j].tposy;
									manager->objs[j].accelerationx = 0;
									manager->objs[j].accelerationy = 0;
								}
								manager->objs[j].action(manager->objs[j].type);
							}
							else {//おされた
								tmanager->touchTable[i].objId = manager->objs[j].objId;
								
								//タッチの登録
								if (manager->objs[j].touchIds[0] == -1) {
									printf("ad:tId[0]<<%d\n",tmanager->touchTable[i].touchId);
									manager->objs[j].touchIds[0] = tmanager->touchTable[i].touchId;
									manager->objs[j].touchNum++;
								}
								else if (manager->objs[j].touchIds[1] == -1 && tmanager->touchTable[i].touchId != manager->objs[j].touchIds[0]){
									printf("ad:tId[1]<<%d\n",tmanager->touchTable[i].touchId);
									manager->objs[j].touchIds[1] = tmanager->touchTable[i].touchId;
									manager->objs[j].touchNum++;
								}
								
								manager->objs[tmanager->touchTable[i].objId].t = true;
								manager->objs[j].accMode = false;
								
								//ドラッグの処理
								if (manager->objs[j].drugMode == true) {
									manager->objs[tmanager->touchTable[i].objId].tposx = manager->objs[tmanager->touchTable[i].objId].posx + tmanager->touchTable[i].movx;
									manager->objs[tmanager->touchTable[i].objId].tposy = manager->objs[tmanager->touchTable[i].objId].posy + tmanager->touchTable[i].movy;
									
									//加速度の設定
									manager->objs[tmanager->touchTable[i].objId].accelerationx = (tmanager->touchTable[i].vposxNow - tmanager->touchTable[i].vposxPre) / REG;
									manager->objs[tmanager->touchTable[i].objId].accelerationy = -(tmanager->touchTable[i].vposyNow - tmanager->touchTable[i].vposyPre) / REG;
								}
							}
						}
						break;
				}
			}
		}
	}
}

void setCameraImageWithCalibrate(MTCaptureImg *cap)
{			
	cvNamedWindow ("src", CV_WINDOW_AUTOSIZE);
	cvSetMouseCallback("src", mouseClickWithCalibration);
	
	src = cvQueryFrame(cap->cap);
	
	end.x = src->width;
	end.y = src->height;
	
	while (cvWaitKey(33) != 'c') {
		src = cvQueryFrame(cap->cap);
		dst = cvCloneImage(src);
		if(clicked) {
			cvRectangle(src, start, end, cvScalar(0, 0, 255));
		} 
		else {
			cvRectangle(src, start, end, cvScalar(255, 0, 0)); 
		}
		if(((end.x-start.x)>0)&((end.y-start.y)>0))
			cvSetImageROI(dst, cvRect(start.x, start.y, end.x-start.x, end.y-start.y));
		cvShowImage ("src", src);
		cvResize(dst, cap->capImg, CV_INTER_CUBIC);
		cvReleaseImage(&dst);
	}
	if(((end.x-start.x)>0)&((end.y-start.y)>0))
		cap->roi = cvRect(start.x, start.y, end.x-start.x, end.y-start.y);
	cvSetMouseCallback("src", NULL);
	cvDestroyWindow("src");
	cap->calibrated = true;
	return;
}

void getCameraImageWithCalibrate(MTCaptureImg *cap)
{
	src = cvQueryFrame(cap->cap);
	dst = cvCloneImage(src);
	cvSetImageROI(dst, cap->roi);
	cvResize(dst, cap->capImg, CV_INTER_CUBIC);
	cvReleaseImage(&dst);
	return;
}

void mouseClickWithCalibration(int event, int x, int y, int flags, void *pram)
{
	switch(event) {
		case CV_EVENT_LBUTTONDOWN:
			clicked = true;
			start.x = x;
			start.y = y;
			break;
		case CV_EVENT_LBUTTONUP:
			clicked = false;
			end.x = x;
			end.y = y;
			break;
		case CV_EVENT_MOUSEMOVE:
			if(!clicked) break;
			end.x = x;
			end.y = y;
			break;
	}
	
	return;
}