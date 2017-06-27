/*
 * currentcontroller.c
 *
 *  Created on: 26 cze 2017
 *      Author: Luke
 */
#include "hal.h"

static void pwmpcb(PWMDriver* pwmp)
{
  (void) pwmp;
}

static void pwmc1cb(PWMDriver *pwmp)
{
  (void) pwmp;
}

void cc_init(void)
{
  PWMConfig pwmcfg = {
                        10000000,
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

  pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 1000));
}

void cc_setCurrent(float current)
{
  (void) current;
}

