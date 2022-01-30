#include "motor_ctrl.h"
#include "app.h"
#include "communication.h"

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

const osThreadAttr_t watchdog_attr = {
  .name = "watchdog",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

int32_t motor_controller::stepper_tick{ 0 };
int32_t motor_controller::desired_tick{ max_values::min_v1 };
osSemaphoreId_t motor_controller::main_joint_sem{ osSemaphoreNew(1, 1, NULL) };
osSemaphoreId_t motor_controller::joint_sem[5]{
	osSemaphoreNew(1, 1, NULL), osSemaphoreNew(1, 1, NULL), osSemaphoreNew(1, 1, NULL),
			osSemaphoreNew(1, 1, NULL), osSemaphoreNew(1, 1, NULL)
};

double motor_controller::v1{ 300.0 }, motor_controller::v2{ 0.0 },
	   motor_controller::v3{ 90.0 }, motor_controller::v4{ 180.0 },
	   motor_controller::v5{ 90.0 }, motor_controller::x{ 0.0 },
	   motor_controller::y{ 0.0 }, motor_controller::z{ 0.0 };

void motor_controller::stepper_watchdog(void* p){
	osSemaphoreAcquire(app::input_sem, osWaitForever);
	v1 = max_values::min_v1;
	HAL_TIM_PWM_Stop_IT(&stepper_motor_tim, TIM_CHANNEL_1);
	communicator::uart_handle << "A2E";

	while(1){
		osSemaphoreAcquire(app::input_sem, osWaitForever);
		v1 = max_values::min_v1; desired_tick = stepper_tick = 0;
		HAL_TIM_PWM_Stop_IT(&stepper_motor_tim, TIM_CHANNEL_1);
		osSemaphoreRelease(joint_sem[0]);
	}
	osThreadTerminate(NULL);
}


void motor_controller::main(void* p){
	osSemaphoreAcquire(main_joint_sem, osWaitForever);
	osSemaphoreAcquire(app::input_sem, osWaitForever);
	osSemaphoreAcquire(joint_sem[0], osWaitForever);

	osThreadNew(update_joint_1, NULL, &joint1_ctrl_attr);
	osThreadNew(update_joint_2, NULL, &joint2_ctrl_attr);
	osThreadNew(update_joint_3, NULL, &joint3_ctrl_attr);
	osThreadNew(update_joint_4, NULL, &joint4_ctrl_attr);
	osThreadNew(update_joint_5, NULL, &joint5_ctrl_attr);
	osThreadNew(stepper_watchdog, NULL, &watchdog_attr);

	HAL_TIM_PWM_Start(&servomechanism_tim, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&servomechanism_tim, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&servomechanism_tim, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&servomechanism_tim, TIM_CHANNEL_4);

	__HAL_TIM_SET_COMPARE(&stepper_motor_tim, TIM_CHANNEL_1, 500-1);

	HAL_GPIO_WritePin(SLEEP_GPIO_Port, SLEEP_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET);
	HAL_TIM_PWM_Start(&stepper_motor_tim, TIM_CHANNEL_1);

	while(1){
		osSemaphoreAcquire(main_joint_sem, osWaitForever);
		for(uint8_t i = 1; i < 5; ++i){
			osSemaphoreRelease(joint_sem[i]);
		}
	}
	osThreadTerminate(NULL);
}

void motor_controller::start_stepper(bool clkwise){ //true -> clockwise, false -> counterclockwise
	if(clkwise){
		HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET);
	}

	HAL_TIM_PWM_Start_IT(&stepper_motor_tim, TIM_CHANNEL_1);
}

void motor_controller::update_joint_1(void* p){ //stepper motor thread
	while(1){
		if(stepper_tick > desired_tick){
			HAL_TIM_PWM_Stop_IT(&stepper_motor_tim, TIM_CHANNEL_1);
			stepper_tick = 0; desired_tick = 0;
			communicator::uart_handle << "A2E";
			osSemaphoreRelease(joint_sem[0]);
		}
	}
	osThreadTerminate(NULL);
}

void motor_controller::update_joint_2(void* p){ //servo1
	while(1){
		osSemaphoreAcquire(joint_sem[1], osWaitForever);
		__HAL_TIM_SET_COMPARE(&servomechanism_tim, TIM_CHANNEL_1, get_pwm(v2));
	}
	osThreadTerminate(NULL);
}

void motor_controller::update_joint_3(void* p){ //servo2
	while(1){
		osSemaphoreAcquire(joint_sem[2], osWaitForever);
		__HAL_TIM_SET_COMPARE(&servomechanism_tim, TIM_CHANNEL_2, get_pwm(v3));
	}
	osThreadTerminate(NULL);
}

void motor_controller::update_joint_4(void* p){ //servo3
	while(1){
		osSemaphoreAcquire(joint_sem[3], osWaitForever);
		__HAL_TIM_SET_COMPARE(&servomechanism_tim, TIM_CHANNEL_3, get_pwm(v4));
	}
	osThreadTerminate(NULL);
}

void motor_controller::update_joint_5(void* p){//servo 4
	while(1){
		osSemaphoreAcquire(joint_sem[4], osWaitForever);
		__HAL_TIM_SET_COMPARE(&servomechanism_tim, TIM_CHANNEL_4, get_pwm(v5));
	}
	osThreadTerminate(NULL);
}

uint16_t motor_controller::get_pwm(double angle){
	if(angle > max_values::max_servo_angle){
		angle = max_values::max_servo_angle;
	}
	else if(angle < max_values::min_servo_angle){
		angle = max_values::min_servo_angle;
	}

	return (5.0/9)*angle + 100;
}

int32_t motor_controller::get_impulse_qty(int16_t height){
	if(height > (int16_t)max_values::max_v1){
		height = (int16_t)max_values::max_v1;
	}
	else if(height < (int16_t)max_values::min_v1){
		height = (int16_t)max_values::min_v1;
	}

	int32_t diff = height - (int32_t)v1;

	v1 = (double)height;
	int32_t ret_val = (max_values::revolutions_per_cm * max_values::resolution * max_values::steps_per_revolution * abs(diff))/10; //measured in mm

	return (diff >= 0? ret_val : ret_val * -1);
}

void motor_controller::stepper_tim_callback(){
	++stepper_tick;
}
