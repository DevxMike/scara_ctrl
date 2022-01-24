#ifndef communication_h
#define communication_h

#include "main.h"
#include "uart_wrapper.h"
#include "cmsis_os.h"

class communicator{
	static DMA_HandleTypeDef& copy_dma;
	static osSemaphoreId_t copy_sem;

	static void read_thread(void* p);
	static void copy_thread(void* p);
	static void mem_cpy_cplt_callback(DMA_HandleTypeDef* hdma);

public:
	static uint8_t cmd_num;
	static double cmd_arg;

	static uart_wrapper uart_handle;
	static uint8_t cmd_buffer[26];
	static osSemaphoreId_t cmd_sem;
	static osSemaphoreId_t cmd_ready_sem;

	static void main(void* p);
	static void tx_callback();
	static void rx_callback();
	static void adc_callback();
};

#endif
