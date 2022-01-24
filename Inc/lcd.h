/*
 * lcd.h
 *
 *  Created on: Dec 9, 2021
 *      Author: Michael
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "stm32f4xx_hal.h"

constexpr unsigned char rows_count = 8;

namespace hd44780{
	constexpr unsigned char function_set_4_bit = 0x28, function_set_8_bit = 0x38;
	constexpr unsigned char max_index = 40;

	struct gpio{
		GPIO_TypeDef* D0, * D1, * D2, * D3, * D4, * D5, * D6, * D7;
		GPIO_TypeDef* RS, * RW, * E;

		uint16_t D0_pin, D1_pin, D2_pin, D3_pin, D4_pin, D5_pin, D6_pin, D7_pin,
					RS_pin, RW_pin, E_pin;
	};

	class lcd {
	public:
		enum class mode{ lcd_4_bit, lcd_8_bit };
		enum cmd{
			clear = 0x01, home = 0x02, entry_mode = 0x06,
			display_off = 0x08, display_on = 0x0C, disp_shift_left = 0x18,
			disp_shift_right = 0x1C, set_cgram = 0x40, set_ddram = 0x80
		};
		enum status{
			lcd_ok, lcd_nok
		};

	private:
		class shift_index{
		private:
			unsigned char index;
			const unsigned char max_index;
		public:
			shift_index() = delete;
			shift_index(unsigned char max): index{ 0 }, max_index{ max } {}
			bool operator==(unsigned char i)const { return index == i; }
			bool operator>(unsigned char i)const { return index > i; }
			bool operator>=(unsigned char i)const { return index >= i; }
			bool operator<(unsigned char i)const { return index < i; }
			bool operator<=(unsigned char i)const { return index <= i; }
			bool operator!=(unsigned char i)const { return index != i; }

			shift_index& operator++(){
				if(index >= max_index - 1){
					index = 0;
				}
				else{
					++index;
				}

				return *this;
			}
			shift_index& operator--(){
				if(index == 0){
					index = max_index - 1;
				}
				else{
					--index;
				}

				return *this;
			}
		};

		gpio pinout;
		mode lcd_mode;
		unsigned char custom_chars_count;
		shift_index index;

		// low level private functions
		void set_rs() { HAL_GPIO_WritePin(pinout.RS, pinout.RS_pin, GPIO_PIN_SET); }
		void clr_rs() { HAL_GPIO_WritePin(pinout.RS, pinout.RS_pin, GPIO_PIN_RESET); }
		void set_e() { HAL_GPIO_WritePin(pinout.E, pinout.E_pin, GPIO_PIN_SET); }
		void clr_e() { HAL_GPIO_WritePin(pinout.E, pinout.E_pin, GPIO_PIN_RESET); }
		void set_rw() { HAL_GPIO_WritePin(pinout.RW, pinout.RW_pin, GPIO_PIN_SET); }
		void clr_rw() { HAL_GPIO_WritePin(pinout.RW, pinout.RW_pin, GPIO_PIN_RESET); }

		void init();
		void set_gpio_out();
		void set_gpio_in();
		void write_half(unsigned char);
		void write_full(unsigned char);
		unsigned char read_half();
		unsigned char read_full();
		void send_byte(unsigned char byte);
		unsigned char read_byte();
		unsigned char get_busy_flag(){ clr_rs(); return read_byte();  }

	public:
		lcd() = delete;
		lcd(const gpio& pins, mode m = mode::lcd_4_bit): pinout{ pins }, lcd_mode{ m }, custom_chars_count{ 0 }, index{ max_index } { init(); }
		~lcd(){ }

		unsigned char read_data(){ set_rs(); return read_byte(); }
		status read_line(char* buffer, unsigned char line);
		void send_command(unsigned char cmd) { clr_rs(); send_byte(cmd); }
		void send_data_byte(unsigned char data) { set_rs(); send_byte(data); }
		void send_digit(uint8_t digit){ send_data_byte(digit + '0'); }
		void clear_disp(){ send_command(cmd::clear); }
		void shift_left(){ send_command(cmd::disp_shift_left); ++index; }
		void shift_right(){ send_command(cmd::disp_shift_right); --index; }
		void ret_home(){ send_command(cmd::home); }
		void write_number(uint32_t number);
		void write(const unsigned char* data_byte_array){
			for(auto i = data_byte_array; *i != '\0'; ++i){
				send_data_byte(*i);
			}
		}
		status locate(unsigned char row, unsigned char col);
		status define_custom(const unsigned char* bin_data);
		void shift_home();
	};

}
#endif /* INC_LCD_H_ */
