#include "uart_wrapper.h"
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include <string>


uart_wrapper& uart_wrapper::operator<<(unsigned char c){
	if(!tx_available){
		return *this;
	}

	tx_available = false;

	HAL_UART_Transmit_DMA(handle, (uint8_t*)&c, 1);

	return *this;
}

uart_wrapper& uart_wrapper::operator<<(char c){
	if(!tx_available){
		return *this;
	}

	tx_available = false;

	HAL_UART_Transmit_DMA(handle, (uint8_t*)&c, 1);

	return *this;
}

uart_wrapper& uart_wrapper::operator<<(const char* s){
	if(!tx_available){
		return *this;
	}

	tx_available = false;

	HAL_UART_Transmit_DMA(handle, (uint8_t*)s, strlen(s));

	return *this;
}
void uart_wrapper::receive_byte(){
	if(!rx_available){
		return;
	}

	rx_available = false;

	if(single_byte){
		this->buffer.add_to_buffer(single_byte);
	}

	HAL_UART_Receive_IT(handle, (uint8_t*)&single_byte, 1);
}
