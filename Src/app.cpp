/*
 * app.cpp
 *
 *  Created on: Jan 7, 2022
 *      Author: Michael
 */

#include <app.h>
#include "lcd.h"


float app::temperature{ 0.0f };
float app::voltage{ 0.0f };

uart_wrapper app::uart{ &app::uart_handle };

double app::neural_net_in[3]{ -0.200430, 0.391892, - 1.647278 };
double app::neural_net_out[3]{ 0.0 };

const osThreadAttr_t lcd_task_attr = {
	.name = "lcd task",
	.priority = osPriorityLow
};

const osThreadAttr_t input_check_attr = {
	.name = "input check",
	.priority = osPriorityLow
};

const osThreadAttr_t nn_task_attr = {
	.name = "nn task",
	.priority = osPriorityLow,
};

osSemaphoreId_t app::adc_sem{ osSemaphoreNew(1, 1, NULL) };
osSemaphoreId_t app::neural_sem{ osSemaphoreNew(1, 1, NULL) };
osSemaphoreId_t app::input_sem{ osSemaphoreNew(1, 1, NULL) };


void app::main(void * params){
	hd44780::gpio lcd_gpio;
	lcd_gpio.D4 = DB4_GPIO_Port;
	lcd_gpio.D4_pin = DB4_Pin;
	lcd_gpio.D5 = DB5_GPIO_Port;
	lcd_gpio.D5_pin = DB5_Pin;
	lcd_gpio.D6 = DB6_GPIO_Port;
	lcd_gpio.D6_pin = DB6_Pin;
	lcd_gpio.D7 = DB7_GPIO_Port;
	lcd_gpio.D7_pin = DB7_Pin;

	lcd_gpio.RS = RS_GPIO_Port;
	lcd_gpio.RW = RW_GPIO_Port;
	lcd_gpio.E = E_GPIO_Port;
	lcd_gpio.RS_pin = RS_Pin;
	lcd_gpio.RW_pin = RW_Pin;
	lcd_gpio.E_pin = E_Pin;

	hd44780::lcd lcd_display{ lcd_gpio, hd44780::lcd::mode::lcd_4_bit };

	unsigned char txt[] = "inicjalizacja";


	lcd_display.write(txt);

	osThreadNew(
			app::manage_lcd,
			reinterpret_cast<void*>(&lcd_display),
			&lcd_task_attr
	);

	osThreadNew(
			app::input_check,
			NULL,
			&input_check_attr
	);

	HAL_ADC_Start_DMA(&adc_handle, (uint32_t*)app::adc_data, 2);



	while(1){
		osSemaphoreAcquire(input_sem, osWaitForever);
		HAL_GPIO_TogglePin(SLEEP_GPIO_Port, SLEEP_Pin);
		HAL_GPIO_TogglePin(DIR_GPIO_Port, DIR_Pin);
		/*if(HAL_GPIO_ReadPin(REL_GPIO_Port, REL_Pin) == GPIO_PIN_RESET){
				HAL_Delay(50);
				if(HAL_GPIO_ReadPin(REL_GPIO_Port, REL_Pin) == GPIO_PIN_RESET){
					HAL_GPIO_TogglePin(SLEEP_GPIO_Port, SLEEP_Pin);
					HAL_GPIO_TogglePin(DIR_GPIO_Port, DIR_Pin);
					while(HAL_GPIO_ReadPin(REL_GPIO_Port, REL_Pin) == GPIO_PIN_RESET){
						continue;
					}
				}

		}*/
	}
	osThreadTerminate(NULL);
}

void app::adc_callback(){
	float temp = (adc_data[0] * 3.3f * 11) / 4095;
	voltage = 0.9 * voltage + 0.1 * temp;

	temp = (((float)adc_data[1])  * 3300 ) / 4095;
	temp = ((temp - 760.0) / 2.5) + 25;

	temperature = 0.9 * temperature + 0.1 * temp;

	osSemaphoreRelease(adc_sem);
}

void app::manage_lcd(void* p){
	auto lcd_display = reinterpret_cast<hd44780::lcd*>(p);
	const unsigned char text1[] = "T: ";
	const unsigned char text2[] = "Vin: ";

	while(1){
		osSemaphoreAcquire(adc_sem, osWaitForever);
		lcd_display->clear_disp();
		osDelay(1);
		lcd_display->locate(0, 0);
		osDelay(1);
		lcd_display->write(text1);
		osDelay(1);
		lcd_display->write_number(temperature);
		osDelay(1);
		lcd_display->locate(1, 0);
		osDelay(1);
		lcd_display->write(text2);
		osDelay(1);
		lcd_display->write_number(voltage);
		osDelay(1);
		lcd_display->send_data_byte('.');
		osDelay(1);
		lcd_display->write_number(((uint32_t)(voltage * 10)) % 10);
		osDelay(1000);
		HAL_ADC_Start_DMA(&adc_handle, (uint32_t*)adc_data, 2);
	}
	osThreadTerminate(NULL);
}

void app::input_check(void * p){
	while(1){
		if(HAL_GPIO_ReadPin(REL_GPIO_Port, REL_Pin) == GPIO_PIN_RESET){
			osDelay(40);
			if(HAL_GPIO_ReadPin(REL_GPIO_Port, REL_Pin) == GPIO_PIN_RESET){
				while(HAL_GPIO_ReadPin(REL_GPIO_Port, REL_Pin) == GPIO_PIN_RESET)
				osDelay(100);
				osSemaphoreRelease(input_sem);
			}
		}
	}
	osThreadTerminate(NULL);
}

void app::neural_net_task(void * p){
	while(1){
		matrix d( 3, 1, app::neural_net_in );
		auto a1 = w1 * d;
		a1 = a1 + b1;
		matrix::element_wise_fn(a1, tanh);

		osDelay(1);

		auto a2 = w2 * a1;
		a2 = a2 + b2;
		matrix::element_wise_fn(a2, tanh);

		osDelay(1);

		auto a3 = w3 * a2;
		a3 = a3 + b3;
		matrix::element_wise_fn(a3, tanh);

		osDelay(1);

		auto a4 = w4 * a3;
		a4 = a4 + b4;
		matrix::element_wise_fn(a4, matrix::logsig);
		app::neural_net_out[0] = a4[0];
		app::neural_net_out[1] = a4[1];
		app::neural_net_out[2] = a4[2];

		osDelay(50);
	}
	osThreadTerminate(NULL);
}

double t1[]{
	-0.48466888,  -0.02772117,  -0.56858855,
	-0.71588149, 0.09251538 , 0.36220800,
	0.12139439,  0.52683263,  -0.07512999
};
matrix app::w1{ 3, 3, t1 };
double t2[]{
	-0.13023305,
	0.05342775,
	-0.54410190
};
matrix app::b1{ 3, 1, t2 };
double t3[]{
	1.19703017, 4.95831115, -0.92103620,
	0.78644863, 2.92063664, -0.54997715,
	-1.04869871, 1.63298993, -0.37941664 ,
	0.38132462, 0.82491510, 1.09168423,
	-0.33781541, -0.10879320, 1.40038886 ,
	1.97207363, -0.17483729, 0.08332292 ,
	1.44167371, -0.01253153, -0.39216339
};
matrix app::w2{ 7, 3, t3 };
double t4[]{
	-0.30463793,
	1.50417816,
	-0.00412936,
	-0.03696238,
	0.44088420,
	-1.44050641,
	1.31509051
};
matrix app::b2{ 7, 1, t4 };
double t5[]{
	-2.71311412, 1.32878499, 5.21361655, -0.47754644, 0.12236628, 1.64632762, -1.66452792,
	0.00676326, 0.19924505, -0.10010434, 1.33548757, 2.35509182, 0.12378472, 0.44082493,
	-0.29383685, 0.99806483, 0.77177001, 0.10271539, -0.10759323, 0.00034232, 0.17381803,
	1.53857986, 0.12035402, -1.02363730, 0.53038117, -0.31404730, 0.45138702, 0.42398475,
	0.02633888, -0.49483457, 2.34180014, 1.28707211, -1.02766281, 3.91092167, 0.06803062,
	-0.10397240, -0.08078058, -0.15021623, 2.03318847, 2.10571761, -0.09380264, -0.52113299,
	1.64344464, 4.17683385, -0.91427208, 0.69863058, 0.51676965, -2.12814467, 1.42177840
};
matrix app::w3{ 7, 7, t5 };
double t6[]{
	-1.53971178,
	1.20354525,
	-1.17355805,
	1.56110037,
	1.84535518,
	-0.61083426,
	0.30657361
};
matrix app::b3{ 7,1, t6 };
double t7[]{
	-0.03190653, 2.90133833, 0.11088433, 0.06109504, -0.03959719, 3.61028087, 0.14707115,
	4.38583105, 0.06082046, 0.73061741, 4.64518478, 2.20166683, -0.05391765, -1.64402204,
	-0.07215400, 0.03850719, 2.44910097, -2.36970837, -4.35487053, 0.07770238, 3.61526287
};
matrix app::w4{ 3, 7, t7 };
double t8[]{
	0.85795074,
	3.80410709,
	-4.30926582
};
matrix app::b4{ 3, 1, t8 };
