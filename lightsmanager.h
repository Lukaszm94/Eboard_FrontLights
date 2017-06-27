/*
 * lightsmanager.h
 *
 *  Created on: 27 cze 2017
 *      Author: Luke
 */

#ifndef LIGHTSMANAGER_H_
#define LIGHTSMANAGER_H_

#include "datatypes.h"

void lm_init(void);
void lm_newLightsPacket(CANLightsPacket packet);
LightsManagerData lm_getData(void);

// private
void lm_setBrightness(uint8_t brightnessPercent);

#endif /* LIGHTSMANAGER_H_ */
