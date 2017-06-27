#include "ch.h"
#include "hal.h"
#include "canmanager.h"
#include "datatypes.h"
#include "lightsmanager.h"


LightsManagerData lm_data;
mutex_t lm_dataMutex;

event_listener_t can_el;

// lights manager thread
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {
  (void)arg;
  chRegSetThreadName("lightsManager");
  while (true) {
    LightsManagerData data = lm_getData();
    lm_setBrightness(data.brightness);
    chThdSleepMilliseconds(data.timeOn);
    data = lm_getData();
    if(data.timeOff > 0) {
      lm_setBrightness(0);
      chThdSleepMilliseconds(data.timeOff);
    }
  }
}

/*
 * CAN receiver thread, waits for rxfull event, then canmanager processes new packets
 */
static THD_WORKING_AREA(waThread2, 512);
static THD_FUNCTION(Thread2, arg) {
  (void)arg;
  chRegSetThreadName("can_receiver");
  chEvtRegister(&CAND1.rxfull_event, &can_el, 0);
  while(true) {
    while(!chThdShouldTerminateX()) {
      if (chEvtWaitAnyTimeout(ALL_EVENTS, MS2ST(100)) == 0)
        continue;
      cm_run();
    }
    chEvtUnregister(&CAND1.rxfull_event, &can_el);
  }
}

/*
 * Application entry point.
 */
int main(void) {
  halInit();
  chSysInit();
  chMtxObjectInit(&lm_dataMutex);

  cm_init();
  lm_init();

  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
  chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, NULL);

  while (true) {
      palClearLine(LINE_ARD_A7);
      chThdSleepMilliseconds(500);
      palSetLine(LINE_ARD_A7);
      chThdSleepMilliseconds(500);
  }
}
