/*
 * debug.c
 *
 *  Created on: Feb 5, 2020
 *      Author: hungn
 */

#include "debug.h"

void debug_code_location(unsigned int location) {
  // Update the static debug value
  debug_location = location;

  if (location > MAX_DEBUG_LOCATIONS) {
    stop_everything();
  }  // else output to GPIO pins 1-5 and toggle pin 6

  GPIO_write(CONFIG_GPIO_0, location & (1 << BIT_0));
  GPIO_write(CONFIG_GPIO_1, location & (1 << BIT_1));
  GPIO_write(CONFIG_GPIO_2, location & (1 << BIT_2));
  GPIO_write(CONFIG_GPIO_3, location & (1 << BIT_3));
  GPIO_write(CONFIG_GPIO_4, location & (1 << BIT_4));
  GPIO_write(CONFIG_GPIO_5, location & (1 << BIT_5));

  //GPIO_toggle(CONFIG_GPIO_6);
}

void stop_everything() {
  // Output to pins
  debug_code_location(debug_location);

  // Disable all interrupts
  HwiP_disable();

  // Disable all threads
  vTaskSuspendAll();
  int retc = 0;
  pthread_exit((void *)retc);
}
