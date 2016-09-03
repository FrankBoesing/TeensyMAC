/* TeensyMAC library code is placed under the MIT license
 * Copyright (c) 2016 Frank Bösing
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "TeensyMAC.h"
#include <Arduino.h>

#define MY_SYSREGISTERFILE	((uint8_t *)0x40041000) // System Register File


static uint32_t _getserialhw(void) {
	uint32_t num;
	__disable_irq();
#if defined(HAS_KINETIS_FLASH_FTFA) || defined(HAS_KINETIS_FLASH_FTFL)
	FTFL_FSTAT = FTFL_FSTAT_RDCOLERR | FTFL_FSTAT_ACCERR | FTFL_FSTAT_FPVIOL;
	FTFL_FCCOB0 = 0x41;
	FTFL_FCCOB1 = 15;
	FTFL_FSTAT = FTFL_FSTAT_CCIF;
	while (!(FTFL_FSTAT & FTFL_FSTAT_CCIF)) ; // wait
	num = *(uint32_t *)&FTFL_FCCOB7;
#elif defined(HAS_KINETIS_FLASH_FTFE)
	// Does not work in HSRUN mode :
	FTFL_FSTAT = FTFL_FSTAT_RDCOLERR | FTFL_FSTAT_ACCERR | FTFL_FSTAT_FPVIOL;
	*(uint32_t *)&FTFL_FCCOB3 = 0x41070000;
	FTFL_FSTAT = FTFL_FSTAT_CCIF;
	while (!(FTFL_FSTAT & FTFL_FSTAT_CCIF)) ; // wait
	num = *(uint32_t *)&FTFL_FCCOBB;
#endif
	__enable_irq();
	return num;
}


#if defined(HAS_KINETIS_FLASH_FTFE) && (F_CPU > 120000000)
extern "C" void startup_early_hook(void) {
#if defined(KINETISK)
  WDOG_STCTRLH = WDOG_STCTRLH_ALLOWUPDATE;
#elif defined(KINETISL)
  SIM_COPC = 0;  // disable the watchdog
#endif
 *(uint32_t*)(MY_SYSREGISTERFILE) = _getserialhw();
}

uint32_t teensySerial(void) { 
	uint32_t num;
	num = *(uint32_t*)(MY_SYSREGISTERFILE);
	if (num < 10000000) num = num * 10;		
	return num;
}
uint64_t teensyMAC(void) {
	return 0x04E9E5000000ULL | (*(uint32_t*)(MY_SYSREGISTERFILE));
}

#else

	uint32_t teensySerial(void) { 
	uint32_t num;	
	num = _getserialhw();
	if (num < 10000000) num = num * 10;	
	return num; 
}
uint64_t teensyMAC(void) {
	return 0x04E9E5000000ULL | _getserialhw();
}

#endif 



