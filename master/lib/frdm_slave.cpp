#include "lib/frdm_slave.h"

FRDMSlave::FRDMSlave(SPI& spi, GPIO_TypeDef *gpio, uint16_t bit) :
	spi(spi),
	slave_config(SPI::mk_slave(*gpio, bit, SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_CPOL | SPI_CR1_CPHA)),
	xfer(&slave_config, 0, SPI::OP_EXCHANGE)
{
	chSemInit(&transfer_sem, 0);
	xfer.tc_sem(&transfer_sem);
}

bool FRDMSlave::ping(){
	return true;
}


void FRDMSlave::start_read(){
	xfer.rx_buff = rx_buffer;
	xfer.tx_buff = tx_buffer;
	tx_buffer[0] = 0;
	xfer.n = transfer_size;
	spi.transfer(xfer);
}

void FRDMSlave::finish_read(FRDMSlave::data_t * dst){
	data_t * const end = dst + samples_per_transfer;
	data_t * iter = (data_t *)(rx_buffer + header_size);
	chSemWait(&transfer_sem);
	for(; dst < end; dst++, iter++){
		*dst = *iter;
	}
}
