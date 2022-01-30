#ifndef motor_ctrl
#define motor_ctrl

#include "main.h"
#include "cmsis_os.h"

namespace max_values{
		constexpr uint16_t max_v1 = 950;
		constexpr uint16_t min_v1 = 210;
		constexpr uint8_t max_servo_angle = 180;
		constexpr uint8_t min_servo_angle = 0;
		constexpr uint16_t max_x = 590;
		constexpr uint16_t max_y = 740;
		constexpr uint16_t max_z = 240;

		constexpr uint32_t resolution = 16;
		constexpr uint32_t steps_per_revolution = 200;
		constexpr uint32_t revolutions_per_cm = 5;
};

class motor_controller{
	static TIM_HandleTypeDef& stepper_motor_tim;
	static TIM_HandleTypeDef& servomechanism_tim;

	static int32_t stepper_tick;
	static void update_joint_1(void* p);
	static void update_joint_2(void* p);
	static void update_joint_3(void* p);
	static void update_joint_4(void* p);
	static void update_joint_5(void* p);
	static uint16_t get_pwm(double angle); //get duty cycle for given servo angle (0 - 180)

	static void stepper_watchdog(void* p);

public:
	static int32_t desired_tick;
	static double v1, v2, v3, v4, v5;
	static double x, y, z; //z - height
	static osSemaphoreId_t main_joint_sem;
	static osSemaphoreId_t joint_sem[5];

	static int32_t get_impulse_qty(int16_t height); //returns qty of impulses needed to set the height, if its less than 0 then motor needs to move construction down
	static void stepper_tim_callback();
	static void main(void*);
	static void start_stepper(bool clkwise);
};

#endif
