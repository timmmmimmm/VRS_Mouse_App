/*
 * mouseAxisPostProcessing.c
 *
 *  Created on: Dec 22, 2023
 *      Author: timip
 */

#include "mouseAxisPostProcessing.h"
#include <stdlib.h>

#define NOISE_FILTER_MARGIN 3
#define NOISE_FILTER_DIFF_MARGIN 5
#define Z_AXIS_POST_MULT 10

static MouseAxisInfo axisInfo = {0}, prevAxisInfo = {0};
int16_t MAP(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
MouseAxisInfo *PP_ProcessData(MouseAxisInfo *mouseData, uint16_t DPI)
{

	axisInfo.x = mouseData->x;
	axisInfo.y = mouseData->y;
	axisInfo.z = mouseData->z;

	if(abs(prevAxisInfo.x) < 5 && abs(mouseData->x) < NOISE_FILTER_DIFF_MARGIN)
	{
	  if(abs(mouseData->x - prevAxisInfo.x) <= NOISE_FILTER_MARGIN)
		  axisInfo.x = 0;
	}

	if(abs(prevAxisInfo.y) < 5 && abs(mouseData->y) < NOISE_FILTER_DIFF_MARGIN)
	{
	  if(abs(mouseData->y - prevAxisInfo.y) <= NOISE_FILTER_MARGIN)
		  axisInfo.y = 0;
	}

	if(abs(prevAxisInfo.z) < 5 && abs(mouseData->z) < NOISE_FILTER_DIFF_MARGIN)
	{
	  if(abs(mouseData->z - prevAxisInfo.z) <= NOISE_FILTER_MARGIN)
		  axisInfo.z = 0;
	}


	if(axisInfo.x != 0)
	{
		axisInfo.x = axisInfo.x > 0 ?
						axisInfo.x - NOISE_FILTER_MARGIN :
						axisInfo.x + NOISE_FILTER_MARGIN;
	}

	if(axisInfo.y != 0)
		{
			axisInfo.y = axisInfo.y > 0 ?
							axisInfo.y - NOISE_FILTER_MARGIN :
							axisInfo.y + NOISE_FILTER_MARGIN;
		}

	if(axisInfo.z != 0)
		{
			axisInfo.z = axisInfo.z > 0 ?
							axisInfo.z - NOISE_FILTER_MARGIN :
							axisInfo.z + NOISE_FILTER_MARGIN;

			axisInfo.z *= Z_AXIS_POST_MULT;
		}


	prevAxisInfo = *mouseData;

	axisInfo.x *= DPI;
	axisInfo.y *= DPI;
	if(axisInfo.z > 50)
		axisInfo.z = 50;
	else if(axisInfo.z < -50)
		axisInfo.z = -50;
	axisInfo.z = MAP(axisInfo.z, -50, 50, -15, 15);

	return &axisInfo;	
}
