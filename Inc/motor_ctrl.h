#ifndef motor_ctrl
#define motor_ctrl

#include "main.h"
#include "cmsis_os.h"

class motor_controller{
	static TIM_HandleTypeDef& stepper_motor_tim;
	static TIM_HandleTypeDef& servomechanism_tim;

	static void update_joint_1(void* p);
	static void update_joint_2(void* p);
	static void update_joint_3(void* p);
	static void update_joint_4(void* p);
	static void update_joint_5(void* p);



public:
	static osSemaphoreId_t main_joint_sem;
	static void main(void*);
};

#endif
