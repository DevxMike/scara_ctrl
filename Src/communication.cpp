#include "communication.h"
#include <cctype>
#include <cstdlib>

const osThreadAttr_t read_task_attr = {
	.name = "read_task",
	.priority = osPriorityLow,
};

const char* wrong_packet_code = "A1E";
const char* acknowledge_packet_code = "A0E";

uint8_t communicator::cmd_buffer[26]{ 0 };
uint8_t communicator::cmd_num{ 0 };
double communicator::cmd_arg{ 0.0 };

osSemaphoreId_t communicator::cmd_sem{ osSemaphoreNew(1, 1, NULL) };
osSemaphoreId_t communicator::copy_sem{ osSemaphoreNew(1, 1, NULL) };
osSemaphoreId_t communicator::cmd_ready_sem{ osSemaphoreNew(1, 1, NULL) };

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

		if(cmd_buffer[0] == 'S' && cmd_buffer[25] == 'E'){
			if(isdigit(cmd_buffer[1]) && isdigit(cmd_buffer[2])){
				auto cmd = (cmd_buffer[1] - '0') * 10 + (cmd_buffer[2] - '0');
				if(isdigit(cmd_buffer[3]) && isdigit(cmd_buffer[4])){
					auto data_len = (cmd_buffer[3] - '0') * 10 + (cmd_buffer[4] - '0');
					auto dummy_set_zeros = true;

					for(uint8_t i = 5 + data_len; i < 25; ++i){
						if(cmd_buffer[i] != '0'){
							dummy_set_zeros = false;
							break;
						}
					}
					if(dummy_set_zeros){
						auto arg = strtod((char*)&cmd_buffer[5], NULL);

						cmd_arg = arg;
						cmd_num = cmd;

						uart_handle << acknowledge_packet_code;

						osSemaphoreRelease(cmd_ready_sem);
					}
					else{
						uart_handle << wrong_packet_code;
					}
				}
				else{
					uart_handle << wrong_packet_code;
				}
			}
			else{
				uart_handle << wrong_packet_code;
			}
		}
		else{
			uart_handle << wrong_packet_code;
		}
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
				uart_handle << wrong_packet_code;
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
				uart_handle << wrong_packet_code;
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
