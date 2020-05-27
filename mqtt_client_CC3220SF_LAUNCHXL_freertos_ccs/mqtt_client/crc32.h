/*
 * crc32.h
 *
 *  Created on: Apr 1, 2020
 *      Author: hungnguyen
 */
// http://www.ti.com/lit/an/slaa221a/slaa221a.pdf?ts=1587880444170
// https://e2e.ti.com/support/wireless-connectivity/wifi/f/968/p/755538/2791091?tisearch=e2e-sitesearch&keymatch=cc3200%20crc#2791091

#ifndef MQTT_CLIENT_CRC32_H_
#define MQTT_CLIENT_CRC32_H_

#include <stdint.h>

#include "ti/devices/cc32xx/driverlib/crc.h"
#include "ti/devices/cc32xx/driverlib/rom.h"
#include "ti/devices/cc32xx/driverlib/rom_map.h"
#include "ti/devices/cc32xx/inc/hw_memmap.h"

#define FIRST_2BYTES  0xFF000000
#define SECOND_2BYTES 0xFF0000
#define THIRD_2BYTES  0xFF00
#define FOURTH_2BYTES 0xFF
#define ONE_BYTE      8
#define THREE_BYTES   24

uint32_t calculate_CRC32(unsigned char *ptr, int count);

#endif /* MQTT_CLIENT_CRC32_H_ */
