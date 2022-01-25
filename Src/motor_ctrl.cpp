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

osSemaphoreId_t motor_controller::main_joint_sem{ osSemaphoreNew(1, 1, NULL) };
osSemaphoreId_t motor_controller::joint_sem[5]{
	osSemaphoreNew(1, 1, NULL), osSemaphoreNew(1, 1, NULL), osSemaphoreNew(1, 1, NULL),
			osSemaphoreNew(1, 1, NULL), osSemaphoreNew(1, 1, NULL)
};

double motor_controller::v1{ 0.0 }, motor_controller::v2{ 0.0 },
	   motor_controller::v3{ 0.0 }, motor_controller::v4{ 0.0 },
	   motor_controller::v5{ 0.0 }, motor_controller::x{ 0.0 },
	   motor_controller::y{ 0.0 }, motor_controller::z{ 0.0 };

void motor_controller::main(void* p){
	osThreadNew(update_joint_1, NULL, &joint1_ctrl_attr);
	osThreadNew(update_joint_2, NULL, &joint2_ctrl_attr);
	osThreadNew(update_joint_3, NULL, &joint3_ctrl_attr);
	osThreadNew(update_joint_4, NULL, &joint4_ctrl_attr);
	osThreadNew(update_joint_5, NULL, &joint5_ctrl_attr);

	osSemaphoreAcquire(main_joint_sem, osWaitForever);

	while(1){
		osSemaphoreAcquire(main_joint_sem, osWaitForever);
		for(uint8_t i = 0; i < 5; ++i){
			osSemaphoreRelease(joint_sem[i]);
			osDelay(50);
		}
	}
	osThreadTerminate(NULL);
}

void motor_controller::update_joint_1(void* p){ //stepper motor thread
	while(1){
		osSemaphoreAcquire(joint_sem[0], osWaitForever);

	}
	osThreadTerminate(NULL);
}

void motor_controller::update_joint_2(void* p){ //servo1
	while(1){
		osSemaphoreAcquire(joint_sem[1], osWaitForever);
		HAL_TIM_PWM_Start(&servomechanism_tim, TIM_CHANNEL_1);
		osDelay(1000);
		HAL_TIM_PWM_Stop(&servomechanism_tim, TIM_CHANNEL_1);
		osDelay(1000);
	}
	osThreadTerminate(NULL);
}

void motor_controller::update_joint_3(void* p){ //servo2
	while(1){
		osSemaphoreAcquire(joint_sem[2], osWaitForever);
		HAL_TIM_PWM_Start(&servomechanism_tim, TIM_CHANNEL_2);
		osDelay(500);
		HAL_TIM_PWM_Stop(&servomechanism_tim, TIM_CHANNEL_2);
		osDelay(500);
	}
	osThreadTerminate(NULL);
}

void motor_controller::update_joint_4(void* p){ //servo3
	while(1){
		osSemaphoreAcquire(joint_sem[3], osWaitForever);
		HAL_TIM_PWM_Start(&servomechanism_tim, TIM_CHANNEL_3);
		osDelay(2000);
		HAL_TIM_PWM_Stop(&servomechanism_tim, TIM_CHANNEL_3);
		osDelay(2000);
	}
	osThreadTerminate(NULL);
}

void motor_controller::update_joint_5(void* p){//servo 4
	while(1){
		osSemaphoreAcquire(joint_sem[4], osWaitForever);
		HAL_TIM_PWM_Start(&servomechanism_tim, TIM_CHANNEL_4);
		osDelay(1000);
		HAL_TIM_PWM_Stop(&servomechanism_tim, TIM_CHANNEL_4);
		osDelay(1000);
	}
	osThreadTerminate(NULL);
}
