/*
 * canmanager.h
 *
 *  Created on: 28 gru 2016
 *      Author: Luke
 */

#ifndef CANMANAGER_H_
#define CANMANAGER_H_
#include "hal.h"
#include "datatypes.h"

void cm_init(void);
void cm_run(void);

// private
CANLightsPacket cm_unpackFrontLightsPacket(CANRxFrame frame);

uint32_t cm_getEID(uint8_t deviceId, uint8_t messageId);

#endif /* CANMANAGER_H_ */
