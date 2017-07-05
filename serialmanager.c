/*
 * serialmanager.c
 *
 *  Created on: 31 gru 2016
 *      Author: Luke
 */
#include "serialmanager.h"
#include "globals.h"
#include "chprintf.h"
#include "hal.h"
#include "buffer.h"
#include "canmanager.h"

#define CURRENT_PACKET_HEADER 'a'
#define TEMPERATURE_PACKET_HEADER 'b'
#define BATTERY_PACKET_HEADER 'c'
#define SPEED_PACKET_HEADER 'd'
#define ERROR_PACKET_HEADER 'e'

#define LIGHTS_PACKET_HEADER 'l'

#define PACKET_END_CHAR ';'

#define BUFFER_SIZE 16

void sm_init(void)
{
  sdStart(&SD1, NULL);
}

/*void sm_sendErrorPacket(ErrorPacket packet)
{
  //TODO
}*/

int sm_chprintf(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    if(chMtxTryLock(&serialMutex) == TRUE) {
      chvprintf(chp, format, args);
      chMtxUnlock(&serialMutex);
    }
    va_end(args);
    return 0;
}
