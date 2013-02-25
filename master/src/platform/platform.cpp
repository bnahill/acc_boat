/*!
 @file platform.cpp
 
 @brief Platform-specific instantiations
 
 @author Ben Nahill <bnahill@gmail.com>
 */
#define __PLATFORM_CPP_

#include "platform.h"
#include "platform_usbserial.h"

namespace Platform {

///////////////////////////////////////////
// SPI Platform
///////////////////////////////////////////

SPI spi1(SPID1);

SPI spi2(SPID2);


///////////////////////////////////////////
// FRDM Slaves
///////////////////////////////////////////

FRDMSlave slaves[num_slaves] = {
	{spi1, GPIOE, 5},
	//{spi1, GPIOE, 6},
	//{spi1, GPIOE, 7},
	//{spi1, GPIOE, 8},
	//{spi1, GPIOE, 9},
};

gpio_pin_t sync_pin(GPIOE, 4);

///////////////////////////////////////////
// Button platform configuration
///////////////////////////////////////////

button_t button = {GPIOA, 0, button_t::ACTIVE_HIGH};

///////////////////////////////////////////
// Edge-trigger interrupt configuration
///////////////////////////////////////////

const EXTConfig extcfg = {
	{
		{EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOA,
			Ext1Callback::callback}, // 0
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL}, // 5
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL}, // 10
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL}, // 15
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL}, // 20
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL}
	}
};

///////////////////////////////////////////
// LED platform configuration
///////////////////////////////////////////

gpio_pin_t leds[4] = {	
	{GPIOD, 12},
	{GPIOD, 13},
	{GPIOD, 14},
	{GPIOD, 15},
};

usbserial1_t usbserial1({GPIOA, 9});

//////////////////////////////////////////////////////////
// Platform initialization
//////////////////////////////////////////////////////////

void early_init(){
	extStart(&EXTD1, &extcfg);
}

}


extern "C" {
void NMIVector(void){while(1);}
void HardFaultVector(void){while(1);}
void MemManageVector(void){while(1);}
void BusFaultVector(void){while(1);}
void UsageFaultVector(void){while(1);}
}
