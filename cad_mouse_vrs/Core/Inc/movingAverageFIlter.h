/*
 * movingAverageFIlter.h
 *
 *  Created on: Dec 15, 2023
 *      Author: timip
 */

#ifndef INC_MOVINGAVERAGEFILTER_H_
#define INC_MOVINGAVERAGEFILTER_H_

#include "stdint.h"


typedef struct{
	int16_t x;
	int16_t y;
	int16_t z;
}MouseAxisInfo;

/**
 * Filter window init, loads callbacks and initializes values
 */
void initFilter(float(* xFn)(void),
	       float(* yFn)(void),
		   float(* zFn)(void),
		   void(* updateData)(void),
		   void(* delayFn)(uint32_t));
MouseAxisInfo* MA_filterData();

#endif /* INC_MOVINGAVERAGEFILTER_H_ */
