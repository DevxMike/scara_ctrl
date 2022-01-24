/*
 * lcd.cpp
 *
 *  Created on: Dec 9, 2021
 *      Author: Michael
 */

#include "lcd.h"
#include <stdlib.h>

void hd44780::lcd::init(){
	set_gpio_out();
	HAL_Delay(15);
	clr_e();
	clr_rs();
	clr_rw();
	HAL_Delay(15);
	switch(lcd_mode){
		case hd44780::lcd::mode::lcd_4_bit:
		set_e();
		write_half(0x03);
		clr_e();
		HAL_Delay(5);
		set_e();
		write_half(0x03);
		clr_e();
		HAL_Delay(1);
		set_e();
		write_half(0x03);
		clr_e();
		HAL_Delay(1);
		set_e();
		write_half(0x02);
		clr_e();
		HAL_Delay(1);
		send_command(hd44780::function_set_4_bit);
		break;

		case hd44780::lcd::mode::lcd_8_bit:
		set_e();
		write_full(0x30);
		clr_e();
		HAL_Delay(5);
		set_e();
		write_full(0x30);
		clr_e();
		HAL_Delay(1);
		set_e();
		write_full(0x30);
		clr_e();
		HAL_Delay(1);

		send_command(hd44780::function_set_8_bit);
		break;
	}
	send_command(cmd::display_on);
	send_command(cmd::clear);
	send_command(cmd::entry_mode);
	send_command(cmd::home);
	HAL_Delay(15);
}

void hd44780::lcd::write_half(unsigned char half){
	if(half & (1 << 0)){ HAL_GPIO_WritePin(pinout.D4, pinout.D4_pin, GPIO_PIN_SET); }
	else{ HAL_GPIO_WritePin(pinout.D4, pinout.D4_pin, GPIO_PIN_RESET); }
	if(half & (1 << 1)){ HAL_GPIO_WritePin(pinout.D5, pinout.D5_pin, GPIO_PIN_SET); }
	else{ HAL_GPIO_WritePin(pinout.D5, pinout.D5_pin, GPIO_PIN_RESET); }
	if(half & (1 << 2)){ HAL_GPIO_WritePin(pinout.D6, pinout.D6_pin, GPIO_PIN_SET); }
	else{ HAL_GPIO_WritePin(pinout.D6, pinout.D6_pin, GPIO_PIN_RESET); }
	if(half & (1 << 3)){ HAL_GPIO_WritePin(pinout.D7, pinout.D7_pin, GPIO_PIN_SET); }
	else{ HAL_GPIO_WritePin(pinout.D7, pinout.D7_pin, GPIO_PIN_RESET); }
}

void hd44780::lcd::write_full(unsigned char byte){
	if(byte & (1 << 0)){ HAL_GPIO_WritePin(pinout.D0, pinout.D0_pin, GPIO_PIN_SET); }
	else{ HAL_GPIO_WritePin(pinout.D0, pinout.D0_pin, GPIO_PIN_RESET); }

	if(byte & (1 << 1)){ HAL_GPIO_WritePin(pinout.D1, pinout.D1_pin, GPIO_PIN_SET); }
	else{ HAL_GPIO_WritePin(pinout.D1, pinout.D1_pin, GPIO_PIN_RESET); }

	if(byte & (1 << 2)){ HAL_GPIO_WritePin(pinout.D2, pinout.D2_pin, GPIO_PIN_SET); }
	else{ HAL_GPIO_WritePin(pinout.D2, pinout.D2_pin, GPIO_PIN_RESET); }

	if(byte & (1 << 3)){ HAL_GPIO_WritePin(pinout.D3, pinout.D3_pin, GPIO_PIN_SET); }
	else{ HAL_GPIO_WritePin(pinout.D3, pinout.D3_pin, GPIO_PIN_RESET); }

	if(byte & (1 << 4)){ HAL_GPIO_WritePin(pinout.D4, pinout.D4_pin, GPIO_PIN_SET); }
	else{ HAL_GPIO_WritePin(pinout.D4, pinout.D4_pin, GPIO_PIN_RESET); }

	if(byte & (1 << 5)){ HAL_GPIO_WritePin(pinout.D5, pinout.D5_pin, GPIO_PIN_SET); }
	else{ HAL_GPIO_WritePin(pinout.D5, pinout.D5_pin, GPIO_PIN_RESET); }

	if(byte & (1 << 6)){ HAL_GPIO_WritePin(pinout.D6, pinout.D6_pin, GPIO_PIN_SET); }
	else{ HAL_GPIO_WritePin(pinout.D6, pinout.D6_pin, GPIO_PIN_RESET); }

	if(byte & (1 << 7)){ HAL_GPIO_WritePin(pinout.D7, pinout.D7_pin, GPIO_PIN_SET); }
	else{ HAL_GPIO_WritePin(pinout.D7, pinout.D7_pin, GPIO_PIN_RESET); }
}

void hd44780::lcd::send_byte(unsigned char byte){
	set_gpio_out();
	clr_rw();

	switch(lcd_mode){
		case hd44780::lcd::mode::lcd_4_bit:
			set_e();
			write_half(byte >> 4);
			clr_e();
			set_e();
			write_half(byte);
			clr_e();
		break;

		case hd44780::lcd::mode::lcd_8_bit:
			set_e();
			write_full(byte);
			clr_e();
		break;
	}

	while(get_busy_flag() & (1 << 7)){
		continue;
	}
}

unsigned char hd44780::lcd::read_half(){
	unsigned char tmp = 0x00;

	if(HAL_GPIO_ReadPin(pinout.D4, pinout.D4_pin)){
		tmp |= (1 << 0);
	}
	if(HAL_GPIO_ReadPin(pinout.D5, pinout.D5_pin)){
		tmp |= (1 << 1);
	}
	if(HAL_GPIO_ReadPin(pinout.D6, pinout.D6_pin)){
		tmp |= (1 << 2);
	}
	if(HAL_GPIO_ReadPin(pinout.D7, pinout.D7_pin)){
		tmp |= (1 << 3);
	}

	return tmp;
}

unsigned char hd44780::lcd::read_full(){
	unsigned char tmp = 0x00;

	if(HAL_GPIO_ReadPin(pinout.D0, pinout.D0_pin)){
		tmp |= (1 << 0);
	}
	if(HAL_GPIO_ReadPin(pinout.D1, pinout.D1_pin)){
		tmp |= (1 << 1);
	}
	if(HAL_GPIO_ReadPin(pinout.D2, pinout.D2_pin)){
		tmp |= (1 << 2);
	}
	if(HAL_GPIO_ReadPin(pinout.D3, pinout.D3_pin)){
		tmp |= (1 << 3);
	}
	if(HAL_GPIO_ReadPin(pinout.D4, pinout.D4_pin)){
		tmp |= (1 << 4);
	}
	if(HAL_GPIO_ReadPin(pinout.D5, pinout.D5_pin)){
		tmp |= (1 << 5);
	}
	if(HAL_GPIO_ReadPin(pinout.D6, pinout.D6_pin)){
		tmp |= (1 << 6);
	}
	if(HAL_GPIO_ReadPin(pinout.D7, pinout.D7_pin)){
		tmp |= (1 << 7);
	}

	return tmp;
}

unsigned char hd44780::lcd::read_byte(){
	unsigned char byte = 0x00;

	set_gpio_in();
	set_rw();

	switch(lcd_mode){
		case hd44780::lcd::mode::lcd_4_bit:
			set_e();
			byte = read_half();
			clr_e();
			byte = byte << 4;
			set_e();
			byte = read_half();
			clr_e();
		break;

		case hd44780::lcd::mode::lcd_8_bit:
			set_e();
			byte = read_full();
			clr_e();
		break;
	}

	return byte;
}

void hd44780::lcd::set_gpio_out(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = pinout.D4_pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(pinout.D4, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = pinout.D5_pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(pinout.D5, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = pinout.D6_pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(pinout.D6, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = pinout.D7_pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(pinout.D7, &GPIO_InitStruct);

	if(lcd_mode == mode::lcd_8_bit){
		GPIO_InitStruct.Pin = pinout.D0_pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(pinout.D0, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = pinout.D1_pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(pinout.D1, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = pinout.D2_pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(pinout.D2, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = pinout.D3_pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(pinout.D3, &GPIO_InitStruct);
	}
}

void hd44780::lcd::set_gpio_in(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = pinout.D4_pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(pinout.D4, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = pinout.D5_pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(pinout.D5, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = pinout.D6_pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(pinout.D6, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = pinout.D7_pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(pinout.D7, &GPIO_InitStruct);

	if(lcd_mode == mode::lcd_8_bit){
		GPIO_InitStruct.Pin = pinout.D0_pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(pinout.D0, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = pinout.D1_pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(pinout.D1, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = pinout.D2_pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(pinout.D2, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = pinout.D3_pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(pinout.D3, &GPIO_InitStruct);
	}
}

hd44780::lcd::status hd44780::lcd::locate(unsigned char r, unsigned char c){
	if(c > 39 || r > 1){
		ret_home();
		return status::lcd_nok;
	}

	unsigned char addr = (unsigned char)cmd::set_ddram + c + 0x40*r;
	send_command(addr);
	return status::lcd_ok;
}

hd44780::lcd::status hd44780::lcd::define_custom(const unsigned char* bin_data){
	if(custom_chars_count >= 7){
		return status::lcd_nok;
	}

	unsigned char addr = (unsigned char)cmd::set_cgram + custom_chars_count++ * rows_count;
	send_command(addr);

	for(unsigned char i = 0; i < rows_count; ++i){
		send_data_byte(bin_data[i]);
	}

	return status::lcd_ok;
}

void hd44780::lcd::shift_home(){
	if(index == 0){ return; }

	if(index >= (max_index / 2)){
		while(index != 0){
			shift_left();
		}
	}
	else{
		while(index != 0){
			shift_right();
		}
	}
}

hd44780::lcd::status hd44780::lcd::read_line(char* buffer, unsigned char line){
	if(line <= 1){
		locate(line, 0);

		for(uint8_t i = 0; i < 40; ++i){
			buffer[i] = read_data();
		}

		return hd44780::lcd::status::lcd_ok;
	}

	return hd44780::lcd::status::lcd_nok;
}

void hd44780::lcd::write_number(uint32_t number){
	unsigned char buffer[33]{ 0 };
	itoa(number, (char*)buffer, 10);
	write(buffer);
}
