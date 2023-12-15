/*
 * movingAverageFIlter.h
 *
 *  Created on: Dec 15, 2023
 *      Author: timip
 */

#ifndef INC_MOVINGAVERAGEFILTER_H_
#define INC_MOVINGAVERAGEFILTER_H_

#include "stdint.h"
#include "string.h"


typedef struct{
	int16_t x;
	int16_t y;
	int16_t z;
}MouseAxisInfo;


void initFilter(float(* xFn)(void),
			       float(* yFn)(void),
				   float(* zFn)(void),
				   void(* updateData)(void));
MouseAxisInfo* MA_filterData();

#endif /* INC_MOVINGAVERAGEFILTER_H_ */
