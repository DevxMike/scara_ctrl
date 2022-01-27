#include "driver.h"
#include "motor_ctrl.h"
#include "communication.h"

osSemaphoreId_t driver::driver_sem{ osSemaphoreNew(1, 1, NULL) };

void driver::main(void* p){
	while(1){
		osSemaphoreAcquire(driver_sem, osWaitForever);
		//get servo angles
		osDelay(1);

		osSemaphoreRelease(motor_controller::main_joint_sem);
	}
}
