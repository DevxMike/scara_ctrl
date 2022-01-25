#include "command_executor.h"
#include "communication.h"
#include "motor_ctrl.h"

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
		auto cmd_arg = communicator::cmd_arg;

		if(cmd >= 0 && cmd < 17){
			communicator::uart_handle << cmd_executed_code;
			osDelay(1);
			switch(cmd){
			case 0: motor_controller::x = cmd_arg; break;
			case 1: motor_controller::y = cmd_arg; break;
			case 2: motor_controller::z = cmd_arg; break;
			case 3: communicator::uart_handle << "set point\n\r"; break;
			case 4: communicator::uart_handle << "open handle\n\r"; break;
			case 5: communicator::uart_handle << "close handle\n\r"; break;
			case 6: communicator::uart_handle << "set rot handle\n\r"; break;
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
				if(--motor_controller::v2 <= 0){
					motor_controller::v2 = 0.0;
				}
				break; //dec2
			case 11:
				if(++motor_controller::v3 > max_values::max_servo_angle){
					motor_controller::v3 = max_values::max_servo_angle;
				}
			break; //inc3
			case 12:
				if(--motor_controller::v3 <= 0){
					motor_controller::v3 = 0.0;
				}
				break; //dec3
			case 13:
				if(++motor_controller::v4 > max_values::max_servo_angle){
					motor_controller::v4 = max_values::max_servo_angle;
				}
				break; //inc4
			case 14:
				if(--motor_controller::v4 <= 0){
					motor_controller::v4 = 0.0;
				}
			break; //dec4
			case 15:
				if(++motor_controller::v5 > max_values::max_servo_angle){
					motor_controller::v5 = max_values::max_servo_angle;
				}
			break; //inc5
			case 16:
				if(--motor_controller::v5 <= 0){
					motor_controller::v5 = 0.0;
				}
			break; //dec5
			}
		}
		if(cmd > 2){
			osSemaphoreRelease(motor_controller::main_joint_sem);
		}
		else{
			communicator::uart_handle << no_such_cmd_code;
		}
	}
	osThreadTerminate(NULL);
}
