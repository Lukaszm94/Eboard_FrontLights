/*
 * lightsmanager.c
 *
 *  Created on: 27 cze 2017
 *      Author: Luke
 */
#include "lightsmanager.h"
#include "config.h"
#include "globals.h"
#include "hal.h"

#define BLINKING_PATTERNS_COUNT 3

typedef struct BlinkingPattern {
  uint16_t timeOn; // in ms
  uint16_t timeOff; // in ms
} BlinkingPattern;

BlinkingPattern blinkingPatterns[BLINKING_PATTERNS_COUNT];

static void pwmpcb(PWMDriver* pwmp)
{
  (void) pwmp;
}

static void pwmc1cb(PWMDriver *pwmp)
{
  (void) pwmp;
}

void lm_init(void)
{
  PWMConfig pwmcfg = {
                        2000000,
                        100,
                        pwmpcb,
                        {
                         {PWM_OUTPUT_ACTIVE_HIGH, pwmc1cb},
                         {PWM_OUTPUT_DISABLED, NULL},
                         {PWM_OUTPUT_DISABLED, NULL},
                         {PWM_OUTPUT_DISABLED, NULL}
                        },
                        0,
                        0
  };
  pwmStart(&PWMD1, &pwmcfg);
  palSetPadMode(GPIOA, 8, PAL_MODE_ALTERNATE(2));
  lm_setBrightness(0);

  blinkingPatterns[0].timeOff = 0;
  blinkingPatterns[0].timeOn = 1000;
  blinkingPatterns[1].timeOff = 500;
  blinkingPatterns[1].timeOn = 500;
  blinkingPatterns[2].timeOff = 800;
  blinkingPatterns[2].timeOn = 200;

  CANLightsPacket packet;
  packet.blinkingMode = LIGHTS_DEFAULT_MODE;
  packet.brightness= LIGHTS_DEFAULT_BRIGHTNESS;
  lm_newLightsPacket(packet);
}

void lm_newLightsPacket(CANLightsPacket packet)
{
  if(chMtxTryLock(&lm_dataMutex)) {
    lm_data.blinkingMode = packet.blinkingMode;
    lm_data.brightness = packet.brightness;
    lm_setBrightness(lm_data.brightness);
    if(lm_data.blinkingMode < BLINKING_PATTERNS_COUNT) {
      uint8_t mode = lm_data.blinkingMode;
      lm_data.timeOn = blinkingPatterns[mode].timeOn;
      lm_data.timeOff = blinkingPatterns[mode].timeOff;
    }
    chMtxUnlock(&lm_dataMutex);
  }
}

LightsManagerData lm_getData(void)
{
  LightsManagerData data;
  data.blinkingMode = LIGHTS_DEFAULT_MODE;
  data.brightness = LIGHTS_DEFAULT_BRIGHTNESS;
  int i = 0;
  for(i = 0; i < 5; i++) {
    if(chMtxTryLock(&lm_dataMutex)) {
      data = lm_data;
      chMtxUnlock(&lm_dataMutex);
      return data;
    }
    chThdSleepMilliseconds(20);
  }
  return data;
}

void lm_setBrightness(uint8_t brightnessPercent)
{
  if(brightnessPercent == 0) {
    pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 100 * 100));
    return;
  }
  float x = brightnessPercent;
  float y = -0.3 * x + 100;
  if(y < 0.0)
    y = 0.0;
  if(y > 100.0)
    y = 100;
  uint16_t percentage = (uint16_t) y;
  pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, percentage * 100)); // percentage in this function needs to be from 0 to 10000, LOL
}
