#ifndef __IMU_FRDM_SLAVE_H_
#define __IMU_FRDM_SLAVE_H_

#include "ch.h"
#include "hal.h"
#include "lib/spi.h"

/*!
 @brief A Freescale Freedom board used as an SPI slave
 */
class FRDMSlave {
public:
	typedef Euclidean3<int16_t> data_t;
	
	static constexpr uint32_t samples_per_transfer = 32;
	static constexpr uint32_t sample_rate          = 800;
	
	FRDMSlave(SPI &spi, GPIO_TypeDef *gpio, uint16_t bit);
	bool ping();
	void start_read();
	void finish_read(data_t * dst);
	
protected:
	SPI &spi;
	SPI::slave_config_t const slave_config;
	SPI::xfer_t xfer;
	Semaphore transfer_sem;
	
	static constexpr uint32_t header_size          = 1;
	
	static constexpr uint32_t transfer_size        = samples_per_transfer * sizeof(data_t) + header_size;
	
	uint8_t tx_buffer[transfer_size];
	uint8_t rx_buffer[transfer_size];
};

#endif // __IMU_FRDM_SLAVE_H_
