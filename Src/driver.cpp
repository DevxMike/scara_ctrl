#include "driver.h"
#include "motor_ctrl.h"
#include "communication.h"

osSemaphoreId_t driver::driver_sem{ osSemaphoreNew(1, 1, NULL) };

void driver::main(void* p){
	osSemaphoreAcquire(driver_sem, osWaitForever);

	while(1){
		osSemaphoreAcquire(driver_sem, osWaitForever);
		//get servo angles
		motor_controller::desired_tick = motor_controller::get_impulse_qty(communicator::cmd_arg);
		auto arg = (motor_controller::desired_tick >= 0? true : false);
		motor_controller::desired_tick = abs(motor_controller::desired_tick);
		motor_controller::start_stepper(arg);

		osSemaphoreRelease(motor_controller::main_joint_sem);
		osSemaphoreAcquire(motor_controller::joint_sem[0], osWaitForever);
		osDelay(10000);
	}
}
