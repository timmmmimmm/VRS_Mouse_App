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

static MouseAxisInfo axisInfo = {0}, prevAxisInfo = {0};//, fadeAxisInfo = {0};

MouseAxisInfo *PP_ProcessData(MouseAxisInfo *mouseData)
{

	axisInfo.x = mouseData->x;
	axisInfo.y = mouseData->y;
	axisInfo.z = mouseData->z;
	// if(axisInfo.x ==0 )
	// 	fadeAxisInfo.x = 0;
	// if(axisInfo.y ==0 )
	// 	fadeAxisInfo.y = 0;
	// if(axisInfo.z ==0 )
	// 	fadeAxisInfo.z = 0;
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

	// fadeAxisInfo.x = fadeAxisInfo.x * 0.9 + axisInfo.x * 0.1;
	// fadeAxisInfo.y = fadeAxisInfo.y * 0.9 + axisInfo.y * 0.1;
	// fadeAxisInfo.z = fadeAxisInfo.z * 0.9 + axisInfo.z * 0.1;

	// return &fadeAxisInfo;
	return &axisInfo;	
}
