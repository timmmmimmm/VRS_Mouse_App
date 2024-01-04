/*
 * mouseAxisPostProcessing.h
 *
 *  Created on: Dec 22, 2023
 *      Author: timip
 */

#ifndef INC_MOUSEAXISPOSTPROCESSING_H_
#define INC_MOUSEAXISPOSTPROCESSING_H_

#include "movingAverageFIlter.h"

MouseAxisInfo *PP_ProcessData(MouseAxisInfo *mouseData, uint16_t DPI);

#endif /* INC_MOUSEAXISPOSTPROCESSING_H_ */
