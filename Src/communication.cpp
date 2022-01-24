#include "communication.h"

const osThreadAttr_t read_task_attr = {
	.name = "read_task",
	.priority = osPriorityLow,
};

uint8_t communicator::cmd_buffer[26]{ 0 };

osSemaphoreId_t communicator::cmd_sem{ osSemaphoreNew(1, 1, NULL) };
osSemaphoreId_t communicator::copy_sem{ osSemaphoreNew(1, 1, NULL) };

void communicator::mem_cpy_cplt_callback(DMA_HandleTypeDef* hdma){
	osSemaphoreRelease(communicator::copy_sem);
}

void communicator::main(void* p){
	copy_dma.XferCpltCallback = communicator::mem_cpy_cplt_callback;
	copy_dma.XferAbortCallback = copy_dma.XferErrorCallback = NULL;
	copy_dma.XferHalfCpltCallback = NULL;

	osThreadNew(read_thread, NULL, &read_task_attr);
	osSemaphoreAcquire(cmd_sem, osWaitForever);

	uart_handle.receive_byte();

	while(1){
		osSemaphoreAcquire(cmd_sem, osWaitForever);
		uart_handle << "cmd: ";
		osDelay(2);
		uart_handle << (const char*)cmd_buffer;
		osDelay(2);
		uart_handle << "\n\r";
	}
	osThreadTerminate(NULL);
}

void communicator::read_thread(void* p){
	static uint8_t buffer[30]{ 0 };
	static uint8_t i = 0;

	while(1){
			auto c = uart_handle.get_char();

			if(c > 0){
				buffer[i++] = c;
				if(c == 'E' && i != 26){
					uart_handle << "Len error, try again!\n\r";
					i = 0;
				}
				else if(c == 'E' && i == 26){
					HAL_DMA_Start_IT(&copy_dma, (uint32_t)&buffer, (uint32_t)cmd_buffer, 26);
					osSemaphoreAcquire(copy_sem, osWaitForever);
					osSemaphoreRelease(cmd_sem);
					i = 0;
				}
				else if(i == 26){
					i = 0;
					uart_handle << "Format error!\r\n";
				}
			}
	}
	osThreadTerminate(NULL);
}

void communicator::tx_callback(){
	uart_handle.set_tx_available();
}

void communicator::rx_callback(){
	uart_handle.set_rx_available();
	uart_handle.receive_byte();
}
