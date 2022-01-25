#include "neural_net.h"
#include "motor_ctrl.h"
#include "communication.h"

osSemaphoreId_t neural_net::nn_sem{ osSemaphoreNew(1, 1, NULL) };

void neural_net::main(void* p){
	while(1){
		osSemaphoreAcquire(nn_sem, osWaitForever);
		//get servo angles
		osDelay(1);
		communicator::uart_handle << "angles calculated\n\r";
		osSemaphoreRelease(motor_controller::main_joint_sem);
	}
}
