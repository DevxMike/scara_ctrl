#include "motor_ctrl.h"

const osThreadAttr_t joint1_ctrl_attr = {
  .name = "stepper",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

const osThreadAttr_t joint2_ctrl_attr = {
  .name = "servo1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

const osThreadAttr_t joint3_ctrl_attr = {
  .name = "servo2",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

const osThreadAttr_t joint4_ctrl_attr = {
  .name = "servo3",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

const osThreadAttr_t joint5_ctrl_attr = {
  .name = "servo4",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

osSemaphoreId_t motor_controller::main_joint_sem = osSemaphoreNew(1, 1, NULL);

void motor_controller::main(void* p){
	unsigned duty = 500;

	osThreadNew(update_joint_1, reinterpret_cast<void*>(duty), &joint1_ctrl_attr);
	osThreadNew(update_joint_2, reinterpret_cast<void*>(duty), &joint2_ctrl_attr);
	osThreadNew(update_joint_3, reinterpret_cast<void*>(duty), &joint3_ctrl_attr);
	osThreadNew(update_joint_4, reinterpret_cast<void*>(duty), &joint4_ctrl_attr);
	osThreadNew(update_joint_5, reinterpret_cast<void*>(duty), &joint5_ctrl_attr);

	while(1){
		osSemaphoreAcquire(main_joint_sem, osWaitForever);
		//manage joints
	}
	osThreadTerminate(NULL);
}

void motor_controller::update_joint_1(void* p){ //stepper motor thread
	auto duty = reinterpret_cast<uint16_t*>(p);

	while(1){

	}
	osThreadTerminate(NULL);
}

void motor_controller::update_joint_2(void* p){ //servo1
	auto duty = reinterpret_cast<uint16_t*>(p);

	while(1){
		HAL_TIM_PWM_Start(&servomechanism_tim, TIM_CHANNEL_1);
		osDelay(1000);
		HAL_TIM_PWM_Stop(&servomechanism_tim, TIM_CHANNEL_1);
		osDelay(1000);
	}
	osThreadTerminate(NULL);
}

void motor_controller::update_joint_3(void* p){ //servo2
	auto duty = reinterpret_cast<uint16_t*>(p);

	while(1){
		HAL_TIM_PWM_Start(&servomechanism_tim, TIM_CHANNEL_2);
		osDelay(500);
		HAL_TIM_PWM_Stop(&servomechanism_tim, TIM_CHANNEL_2);
		osDelay(500);
	}
	osThreadTerminate(NULL);
}

void motor_controller::update_joint_4(void* p){ //servo3
	auto duty = reinterpret_cast<uint16_t*>(p);

	while(1){
		HAL_TIM_PWM_Start(&servomechanism_tim, TIM_CHANNEL_3);
		osDelay(2000);
		HAL_TIM_PWM_Stop(&servomechanism_tim, TIM_CHANNEL_3);
		osDelay(2000);
	}
	osThreadTerminate(NULL);
}

void motor_controller::update_joint_5(void* p){//servo 4
	auto duty = reinterpret_cast<uint16_t*>(p);

	while(1){
		HAL_TIM_PWM_Start(&servomechanism_tim, TIM_CHANNEL_4);
		osDelay(1000);
		HAL_TIM_PWM_Stop(&servomechanism_tim, TIM_CHANNEL_4);
		osDelay(1000);
	}
	osThreadTerminate(NULL);
}
