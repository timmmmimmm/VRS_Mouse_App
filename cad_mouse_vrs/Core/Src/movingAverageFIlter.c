/*
 * movingAverageFIlter.c
 *
 *  Created on: Dec 15, 2023
 *      Author: timip
 */

#include "movingAverageFIlter.h"
#include "math.h"

#define WINDOW_SIZE 10

static float(* getX)(void) = 0;
static float(* getY)(void) = 0;
static float(* getZ)(void) = 0;
static void(* update)(void) = 0;
static void(* delay)(uint32_t) = 0;

static int16_t checkForOverflow(float data);
static void updateWindowValues(int16_t nX, int16_t nY, int16_t nZ);

MouseAxisInfo axisInfo = {0};

int16_t xVals[WINDOW_SIZE],
	    yVals[WINDOW_SIZE],
		zVals[WINDOW_SIZE];


void initFilter(float(* xFn)(void),
			       float(* yFn)(void),
				   float(* zFn)(void),
				   void(* updateData)(void),
				   void(* delayFn)(uint32_t)){
	getX = xFn;
	getY = yFn;
	getZ = zFn;
	update = updateData;
	delay = delayFn;

	for (uint8_t i = 0; i < WINDOW_SIZE; ++i) {
		update();

		xVals[i] = checkForOverflow((float)round(getX()));
		yVals[i] = checkForOverflow((float)round(getY()));
		zVals[i] = checkForOverflow((float)round(getZ()));


		delay(50);

	}
}

MouseAxisInfo* MA_filterData(){
	update();
	updateWindowValues(checkForOverflow((float)round(getX())),
				  	   checkForOverflow((float)round(getY())),
					   checkForOverflow((float)round(getZ())));

	int16_t xSum = 0, ySum = 0, zSum = 0;

	for(uint8_t i = 0; i < WINDOW_SIZE; ++i){
		xSum += xVals[i];
		ySum += yVals[i];
		zSum += zVals[i];
	}

	axisInfo.x = (int16_t)round((float)xSum / (float)WINDOW_SIZE);
	axisInfo.y = (int16_t)round((float)ySum / (float)WINDOW_SIZE);
	axisInfo.z = (int16_t)round((float)zSum / (float)WINDOW_SIZE);

	return &axisInfo;
}

int16_t checkForOverflow(float data){
		data = data > (float)INT16_MAX ? (float)INT16_MAX : data;
		data = data < (float)INT16_MIN ? (float)INT16_MIN : data;

		return (int16_t) data;
}

void updateWindowValues(int16_t nX, int16_t nY, int16_t nZ){
	for(uint8_t i = 1; i < WINDOW_SIZE; ++i){
		xVals[i - 1] = xVals[i];
		yVals[i - 1] = yVals[i];
		zVals[i - 1] = zVals[i];
	}

	xVals[WINDOW_SIZE - 1] = nX;
	yVals[WINDOW_SIZE - 1] = nY;
	zVals[WINDOW_SIZE - 1] = nZ;
}


