/*
 * globals.h
 *
 *  Created on: 27 cze 2017
 *      Author: Luke
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "datatypes.h"
#include "ch.h"

//----- lights -----
extern LightsManagerData lm_data;
extern mutex_t lm_dataMutex;

//----- can -----
extern event_listener_t can_el;

#endif /* GLOBALS_H_ */
