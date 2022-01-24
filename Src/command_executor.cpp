#include "command_executor.h"
#include "communication.h"

const char* cmd_executed_code = "A2E";
const char* no_such_cmd_code = "A3E";

const osThreadAttr_t set_x_attr = {
	.name = "set_x",
	.priority = osPriorityLow,
};

void command_exec::main(void* p){


	while(1){
		osSemaphoreAcquire(communicator::cmd_ready_sem, osWaitForever);
		auto cmd = communicator::cmd_num;

		if(cmd >= 0 && cmd < 17){
			communicator::uart_handle << cmd_executed_code;
			osDelay(1);
			switch(cmd){
			case 0: communicator::uart_handle << "set x\n\r"; break;
			case 1: communicator::uart_handle << "set y\n\r"; break;
			case 2: communicator::uart_handle << "set z\n\r"; break;
			case 3: communicator::uart_handle << "set point\n\r"; break;
			case 4: communicator::uart_handle << "open handle\n\r"; break;
			case 5: communicator::uart_handle << "close handle\n\r"; break;
			case 6: communicator::uart_handle << "set rot handle\n\r"; break;
			case 7: communicator::uart_handle << "inc1\n\r"; break;
			case 8: communicator::uart_handle << "dec1\n\r"; break;
			case 9: communicator::uart_handle << "inc2\n\r"; break;
			case 10: communicator::uart_handle << "dec2\n\r"; break;
			case 11: communicator::uart_handle << "inc3\n\r"; break;
			case 12: communicator::uart_handle << "dec3\n\r"; break;
			case 13: communicator::uart_handle << "inc4\n\r"; break;
			case 14: communicator::uart_handle << "dec4\n\r"; break;
			case 15: communicator::uart_handle << "inc5\n\r"; break;
			case 16: communicator::uart_handle << "dec5\n\r"; break;
			}
		}
		else{
			communicator::uart_handle << no_such_cmd_code;
		}
	}
	osThreadTerminate(NULL);
}
