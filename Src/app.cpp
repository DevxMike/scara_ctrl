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


const osThreadAttr_t lcd_task_attr = {
	.name = "lcd task",
	.priority = osPriorityLow
};

const osThreadAttr_t input_check_attr = {
	.name = "input check",
	.priority = osPriorityLow
};


osSemaphoreId_t app::adc_sem{ osSemaphoreNew(1, 1, NULL) };
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

	uint8_t customChar[8] = {
		0b01110,
		0b10001,
		0b10001,
		0b01110,
		0b00000,
		0b00000,
		0b00000,
		0b00000
	};

	lcd_display.define_custom(customChar);

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
		osThreadSuspend(NULL);
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
	const unsigned char text1[] = "Board Temp: ";
	const unsigned char text2[] = "Supply: ";

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
		lcd_display->send_data_byte(0);
		osDelay(1);
		lcd_display->send_data_byte('C');
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
		osDelay(1);
		lcd_display->send_data_byte('V');
		osDelay(1);
		osDelay(500);
		HAL_ADC_Start_DMA(&adc_handle, (uint32_t*)adc_data, 2);
	}
	osThreadTerminate(NULL);
}

void app::input_check(void * p){
	while(1){
		if(HAL_GPIO_ReadPin(REL_GPIO_Port, REL_Pin) == GPIO_PIN_RESET){
			osDelay(40);
			if(HAL_GPIO_ReadPin(REL_GPIO_Port, REL_Pin) == GPIO_PIN_RESET){
				osSemaphoreRelease(input_sem);

			}
		}
	}
	osThreadTerminate(NULL);
}
