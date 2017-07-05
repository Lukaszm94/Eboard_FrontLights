/*
 * canamanger.c
 *
 *  Created on: 28 gru 2016
 *      Author: Luke
 */
#include "canmanager.h"
#include "ch.h"
#include "hal.h"
#include "globals.h"
#include "serialmanager.h"
#include "inttypes.h"
#include "buffer.h"
#include "lightsmanager.h"

#define CAN_SERIAL_DEBUG 0

// rpm, total_current, duty_cycle
#define CAN_PACKET_STATUS 9
#define CAN_PACKET_STATUS_SIZE 8
// total_current_filtered, supply_voltage, amp_hours_drawn, amp_hours_charged
#define CAN_PACKET_STATUS_2 10
#define CAN_PACKET_STATUS_2_SIZE 8
// temperature
#define CAN_PACKET_STATUS_3 11
#define CAN_PACKET_STATUS_3_SIZE 2

// front lights
#define CAN_FRONT_LIGHTS_PACKET 16
#define CAN_FRONT_LIGHTS_PACKET_SIZE 2


CANRxFrame rxMsg;

/*
 * 500KBaud, automatic wakeup, automatic recover
 * from abort mode.
 */
static const CANConfig cancfg = {
  CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
  CAN_BTR_SJW(1) | CAN_BTR_TS2(7) |
  CAN_BTR_TS1(6) | CAN_BTR_BRP(5)
};

void cm_init(void)
{
  canStart(&CAND1, &cancfg);
}

void cm_run(void)
{
  while (canReceive(&CAND1, CAN_ANY_MAILBOX, &rxMsg, TIME_IMMEDIATE) == MSG_OK) {
    //palToggleLine(LINE_LED_GREEN);
    uint32_t eid = rxMsg.EID;
    uint8_t packetId = (eid & 0x0000FF00) >> 8;
    uint8_t deviceId = eid & 0x000000FF;
#if CAN_SERIAL_DEBUG
    sm_chprintf("deviceId= %d, packetId= %d\n\r", deviceId, packetId);
#endif
    if(packetId == CAN_FRONT_LIGHTS_PACKET) {
      CANLightsPacket packet = cm_unpackFrontLightsPacket(rxMsg);
      lm_newLightsPacket(packet);
    } else {
      // unknown packet
    }
  }
}

CANLightsPacket cm_unpackFrontLightsPacket(CANRxFrame frame)
{
  CANLightsPacket packet;
  packet.brightness = 0;
#if CAN_SERIAL_DEBUG
  sm_chprintf("%d, CANLightsPacket\n\r", ST2MS(chVTGetSystemTime()));
#endif
  if(frame.DLC != CAN_FRONT_LIGHTS_PACKET_SIZE) {
#if CAN_SERIAL_DEBUG
    sm_chprintf("INCORRECT DATA LEN: %d\n\r", frame.DLC);
#endif
    return packet;
  }
  int32_t index = 0;
  packet.brightness = bufferGetUInt8(frame.data8, &index);
  packet.blinkingMode = bufferGetUInt8(frame.data8, &index);
#if CAN_SERIAL_DEBUG
  sm_chprintf("brightness=%d, mode=%d\n\r", packet.brightness, packet.blinkingMode);
#endif
  return packet;

}

uint32_t cm_getEID(uint8_t deviceId, uint8_t messageId)
{
  uint32_t eid = (((uint32_t)messageId) << 8) | deviceId;
  return eid;
}
