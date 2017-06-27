/*
 * datatypes.h
 *
 *  Created on: 27 cze 2017
 *      Author: Luke
 */

#ifndef DATATYPES_H_
#define DATATYPES_H_

#include <stdint.h>

typedef struct CANLightsPacket {
  uint8_t brightness; // in percent, *1
  uint8_t blinkingMode;
} CANLightsPacket;

typedef struct LightsManagerData {
  uint8_t brightness; // in percent, *1
  uint8_t blinkingMode;
  uint16_t timeOn; // in ms
  uint16_t timeOff; // in ms
} LightsManagerData;

#endif /* DATATYPES_H_ */
