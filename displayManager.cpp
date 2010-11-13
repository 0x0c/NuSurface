/*
 *  displayManager.cpp
 *  opengl_test
 *
 *  Created by Akira on 10/10/01.
 *  Copyright 2010 東京工業大学付属科学技術高等学校. All rights reserved.
 *
 */

#include "displayManager.h"

void initWithDisplayManager(displayManager *dmanager)
{
	for (int i = 0; i < MAX_VIEW_MANAGER; i++) initWithViewManager(&dmanager->manager[i]);
	dmanager->VMNum = 0;
	dmanager->diplayId = 0;
	
	return;
}