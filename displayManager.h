/*
 *  displayManager.h
 *  opengl_test
 *
 *  Created by Akira on 10/10/01.
 *  Copyright 2010 東京工業大学付属科学技術高等学校. All rights reserved.
 *
 */

#ifndef DISPAY_MANAGER
#define DISPAY_MANAGER

#include "viewManager.h"

typedef struct{
	viewManager manager[MAX_VIEW_MANAGER];
	int diplayId;
	int VMNum;
}displayManager;

void initWithDisplayManager(displayManager *dmanager);

#endif DISPLAY_MANAGER