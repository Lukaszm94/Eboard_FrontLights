/*
 * serialmanager.h
 *
 *  Created on: 31 gru 2016
 *      Author: Luke
 */

#ifndef SERIALMANAGER_H_
#define SERIALMANAGER_H_
#include "datatypes.h"

void sm_init(void);
//void sm_sendErrorPacket(ErrorPacket packet);
int sm_chprintf(const char *format, ...);

#endif /* SERIALMANAGER_H_ */
