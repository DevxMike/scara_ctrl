/*
 * app.h
 *
 *  Created on: Jan 7, 2022
 *      Author: Michael
 */

#ifndef INC_APP_H_
#define INC_APP_H_
#include "main.h"
#include "uart_wrapper.h"
#include "cmsis_os.h"
#include "matrix.h"

class app {
private:
	static UART_HandleTypeDef& uart_handle;
	static ADC_HandleTypeDef& adc_handle;
	static uint16_t adc_data[2];
	static float temperature;
	static float voltage;
	static uart_wrapper uart;
	static osSemaphoreId_t adc_sem; //adc semaphore
	static matrix w1, w2, w3, w4; //neural network wages
	static matrix b1, b2, b3, b4; //neural network biases
	static double neural_net_in[3]; //normalised neural net in
	static double neural_net_out[3]; //normalised neural net out
	static osSemaphoreId_t neural_sem; //neural net semaphore
	static osSemaphoreId_t input_sem; //input semaphore

	static void input_check(void* p);
	static void manage_lcd(void* p);
	static void neural_net_task(void* p);

public:
	static void main(void*);
	static void adc_callback();
};

#endif /* INC_APP_H_ */
