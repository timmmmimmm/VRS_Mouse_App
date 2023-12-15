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

static int16_t checkForOverflow(float data);

MouseAxisInfo axisInfo = {0};

int16_t xVals[WINDOW_SIZE + 1],
	    yVals[WINDOW_SIZE + 1],
		zVals[WINDOW_SIZE + 1];


void initFilter(float(* xFn)(void),
			       float(* yFn)(void),
				   float(* zFn)(void),
				   void(* updateData)(void)){
	getX = xFn;
	getY = yFn;
	getZ = zFn;
	update = updateData;

	float temp;

	for (uint8_t i = 0; i < WINDOW_SIZE; ++i) {
		update();



		xVals[i] = checkForOverflow((float)round(getX()));
		yVals[i] = checkForOverflow((float)round(getY()));

	}
}

int16_t checkForOverflow(float data){
		data = data > (float)INT16_MAX ? (float)INT16_MAX : data;
		data = data < (float)INT16_MIN ? (float)INT16_MIN : data;

		return (int16_t) data;
}


