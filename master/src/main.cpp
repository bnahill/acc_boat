#include "ch.h"
#include "hal.h"

#include "platform.h"
#include "ui/usbterm.h"

extern "C"{
	int main(void);
}

static Thread * main_thread;

//static FRDMSlave::data_t acc_buffer1[num_slaves * FRDMSlave::samples_per_transfer * 327];
//__attribute__((section(".ccm")))
//static FRDMSlave::data_t acc_buffer2[num_slaves * FRDMSlave::samples_per_transfer  * 204];

FRDMSlave::data_t acc_buffer1[num_slaves * FRDMSlave::samples_per_transfer * 500];
__attribute__((section(".ccm")))
FRDMSlave::data_t acc_buffer2[num_slaves * FRDMSlave::samples_per_transfer  * 300];
uint32_t sample_count;


typedef enum {
	MAIN_SIG_BUTTON = BIT(0),
	MAIN_SIG_TICK = BIT(1)
} main_sig_t;

static void button_cb(void *){
	chEvtSignal(main_thread, MAIN_SIG_BUTTON);
}

static void timer_cb(void *){
	chEvtSignal(main_thread, MAIN_SIG_TICK);
}

static GPTDriver * const timer_driver = &GPTD8;

static void timer_start(){
	gptStartContinuous(timer_driver, 1000);
}

static void timer_config(){
	static GPTConfig const timer_config = {
		(FRDMSlave::sample_rate * 1000) / FRDMSlave::samples_per_transfer,
		(gptcallback_t) timer_cb
	};
	
	gptObjectInit(timer_driver);
	gptStart(timer_driver, &timer_config);
}

static void timer_stop(){
	gptStopTimer(timer_driver);
}

static USBTerm term;

int main(void) {
	uint32_t i;
	FRDMSlave * slave;
	FRDMSlave::data_t * data_iter;
	FRDMSlave::data_t * buffer_end;
	
	halInit();
	chSysInit();
	
	main_thread = chThdSelf();
	
	Platform::early_init();
	
	for(slave = slaves; slave < slaves + num_slaves; slave++){
		slave->init();
	}
	
	timer_config();
	button.set_press_handler((button_t::button_handler)button_cb, nullptr);
	term.launch();
	
	while(true){
		// Wait for a button press
		chEvtGetAndClearEvents(ALL_EVENTS);
		chEvtWaitOne(MAIN_SIG_BUTTON);
		
		/*
		for(slave = slaves; slave < slaves + num_slaves; slave++){
			if(!slave->ping())
				break;
		} if(slave != slaves + num_slaves){
			// One of the slaves didn't respond...
			for(i = 0; i < 10; i++){
				chThdSleep(MS2ST(100));
				leds[0].toggle();
				leds[1].toggle();
				leds[2].toggle();
				leds[3].toggle();
			}
		}
		*/
		
		// Flash lights 3 times
		for(i = 0; i < 5; i++){
			chThdSleep(MS2ST(500));
			leds[0].toggle();
			leds[1].toggle();
			leds[2].toggle();
			leds[3].toggle();
		}
		
		// Let the last one go for a whole second
		chThdSleep(MS2ST(500));
		leds[0].clear();
		leds[1].clear();
		leds[2].clear();
		leds[3].clear();
		
		// Trigger first sample
		sync_pin.toggle();
		timer_start();

		chEvtGetAndClearEvents(MAIN_SIG_BUTTON);
		sample_count = 0;
		data_iter = acc_buffer1;
		buffer_end = acc_buffer1 + sizeof(acc_buffer1)/sizeof(*acc_buffer1);
		while(!chEvtWaitOneTimeout(MAIN_SIG_BUTTON, TIME_IMMEDIATE)){
			chEvtWaitOne(MAIN_SIG_TICK);
			
			for(slave = slaves; slave < slaves + num_slaves; slave++){
				slave->start_read();
			}
			for(slave = slaves; slave < slaves + num_slaves; slave++){
				slave->finish_read(data_iter);
				data_iter += FRDMSlave::samples_per_transfer;
				sample_count += FRDMSlave::samples_per_transfer;
				if(data_iter == acc_buffer1 + (sizeof(acc_buffer1)/sizeof(*acc_buffer1))){
					data_iter = acc_buffer2;
				} else if(data_iter == acc_buffer2 + (sizeof(acc_buffer2)/sizeof(*acc_buffer2))){
					break;
				}
			} if(slave != slaves + num_slaves){
				break;
			}
			
			// Trigger next sample
			sync_pin.toggle();
#if 0
			for(slave = slaves; slave < slaves + num_slaves; slave++){
				slave->finish_read(data_iter);
				data_iter += FRDMSlave::samples_per_transfer;
				if((uint32_t)(buffer_end - data_iter) < FRDMSlave::samples_per_transfer * sizeof(FRDMSlave::data_t)){
					if(data_iter < acc_buffer1){
						// Buffer is full!
						break;
					}
					data_iter = acc_buffer2;
					buffer_end = acc_buffer2 + sizeof(acc_buffer2)/sizeof(*acc_buffer2);
				}
			} if(slave != slaves + num_slaves){
				for(i = 0; i < 10; i++){
					chThdSleep(MS2ST(100));
					leds[0].toggle();
					leds[1].toggle();
					leds[2].toggle();
					leds[3].toggle();
				}
				break;
			}
#endif
			
			leds[0].toggle();
		}
		
		timer_stop();
		
		for(slave = slaves; slave < slaves + num_slaves; slave++){
			slave->stop();
		}
		
		leds[0].clear();
		leds[1].clear();
		leds[2].clear();
		leds[3].clear();
	
		chThdSleep(MS2ST(250));
	}
}
