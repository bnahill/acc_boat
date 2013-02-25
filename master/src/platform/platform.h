/*!
 @file platform.h
 
 @brief Platform-specific details for most of the logging hardware
 
 This file should exist for each different supported platform. It should
 indicate what buses are used for each device and how they should be
 configured.
 
 Most of the wiring details are left to board.h where the I/O modes for port
 configurations are defined. SPI slaves are an exception.
 
 @author Ben Nahill <bnahill@gmail.com>
 */

#ifndef __IMU_PLATFORM_H_
#define __IMU_PLATFORM_H_

#include <hal.h>

#include "lib/imu.h"

#include <lib/spi.h>

#include <lib/ext.h>
#include <lib/button.h>
#include <lib/usbserial.h>
#include <lib/frdm_slave.h>

/*!
 @brief Platform-dependent component definitions
 
 This namespace contains all platform-dependent devices. This includes all
 external peripherals and their on-chip interfaces. All of these items shall
 be instantiantiated in platform.cpp.
 */
namespace Platform {
	extern SPI spi1;
	extern SPI spi2;
	
	typedef ExtCallback<EXTD1> Ext1Callback;
	typedef Button<EXTD1> button_t;
	extern button_t button;
	
	extern gpio_pin_t leds[4];
	
	extern const EXTConfig extcfg;
	
	typedef USBSerial<USBD1> usbserial1_t;
	extern usbserial1_t usbserial1;
	
	uint32_t constexpr num_slaves = 1;
	extern FRDMSlave slaves[num_slaves];
	extern gpio_pin_t sync_pin;
	
	//typedef RealTimeClock<RTCD1> rtc1;

	void early_init();
};

using namespace Platform;

#endif // __IMU_PLATFORM_H_
