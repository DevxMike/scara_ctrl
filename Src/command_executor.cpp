#include <driver.h>
#include "command_executor.h"
#include "communication.h"
#include "motor_ctrl.h"

const char* cmd_executed_code = "A2E";
const char* no_such_cmd_code = "A3E";
const char* busy_code = "A4E";

const osThreadAttr_t set_x_attr = {
	.name = "set_x",
	.stack_size = 128 * 4,
	.priority = osPriorityLow,
};

void command_exec::main(void* p){

	while(1){
		osSemaphoreAcquire(communicator::cmd_ready_sem, osWaitForever);
		auto cmd = communicator::cmd_num;
		auto cmd_arg = communicator::cmd_arg;


		if(cmd >= 0 && cmd < 17){
			osDelay(1);
			switch(cmd){
			case 0: if(osSemaphoreGetCount(motor_controller::joint_sem[0]))osSemaphoreRelease(driver::driver_sem); else communicator::uart_handle << busy_code; break;
			case 1: motor_controller::v2 = cmd_arg; break;
			case 2: motor_controller::v3 = cmd_arg; break;
			//case 3: osSemaphoreRelease(driver::driver_sem); break;
			case 4: communicator::uart_handle << "open handle\n\r"; break;
			case 5: communicator::uart_handle << "close handle\n\r"; break;
			case 6: motor_controller::v4 = cmd_arg; break;
			case 7:
				if(++motor_controller::v1 >= max_values::max_v1){
					motor_controller::v1 = max_values::max_v1;
				}
				break; //increase height
			case 8:
				if(--motor_controller::v1 <= max_values::min_v1){
					motor_controller::v1 = max_values::min_v1;
				}

				break; //decrease height
			case 9:
				if(++motor_controller::v2 > max_values::max_servo_angle){
					motor_controller::v2 = max_values::max_servo_angle;
				}
				break; //inc2
			case 10:
				if(--motor_controller::v2 <= max_values::min_servo_angle){
					motor_controller::v2 = max_values::min_servo_angle;
				}
				break; //dec2
			case 11:
				if(++motor_controller::v3 > max_values::max_servo_angle){
					motor_controller::v3 = max_values::max_servo_angle;
				}
			break; //inc3
			case 12:
				if(--motor_controller::v3 <= max_values::min_servo_angle){
					motor_controller::v3 = max_values::min_servo_angle;
				}
				break; //dec3
			case 13:
				if(++motor_controller::v4 > max_values::max_servo_angle){
					motor_controller::v4 = max_values::max_servo_angle;
				}
				break; //inc4
			case 14:
				if(--motor_controller::v4 <= max_values::min_servo_angle){
					motor_controller::v4 = max_values::min_servo_angle;
				}
			break; //dec4
			case 15:
				if(++motor_controller::v5 > max_values::max_servo_angle){
					motor_controller::v5 = max_values::max_servo_angle;
				}
			break; //inc5
			case 16:
				if(--motor_controller::v5 <= max_values::min_servo_angle){
					motor_controller::v5 = max_values::min_servo_angle;
				}
			break; //dec5
			}
		}
		if(cmd > 0 && cmd < 17){
			osSemaphoreRelease(motor_controller::main_joint_sem);
			communicator::uart_handle << cmd_executed_code;
		}
		else if(cmd >= 17){
			communicator::uart_handle << no_such_cmd_code;
		}
	}
	osThreadTerminate(NULL);
}
