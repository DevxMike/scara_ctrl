#ifndef uart_wrapper_h
#define uart_wrapper_h

#include "main.h"
#include <stdlib.h>

constexpr unsigned buff_len = 128;

class uart_wrapper{
private:
	class circle_buffer{
	private:
		unsigned char read_index;
		unsigned char write_index;
		const unsigned char buffer_len;
		char data[128];

	public:
		circle_buffer() = delete;
		circle_buffer(unsigned char len):
			read_index{ 0 }, write_index{ 0 }, buffer_len{ len }, data{ 0 } {}

		void add_to_buffer(unsigned char val){
			data[write_index++] = val;
			write_index = write_index % buffer_len;
		}
		char get_from_buffer(){
			if(is_empty()){
				return 0;
			}

			auto tmp = data[read_index++];
			read_index = read_index % buffer_len;

			return tmp;
		}
		bool is_empty()const{
			return read_index == write_index;
		}
	};

	char single_byte;
	UART_HandleTypeDef* handle;
	unsigned char precision;
	bool tx_available;
	bool rx_available;
	circle_buffer buffer;

public:
	uart_wrapper(UART_HandleTypeDef* h, unsigned char float_prec = 2):
		handle{ h }, precision{ float_prec }, buffer{ buff_len } {
			tx_available = true;
			rx_available = true;
			single_byte = -1;
	}
	uart_wrapper& operator<<(unsigned char c);
	uart_wrapper& operator<<(char c);
	uart_wrapper& operator<<(const char* s);
	uart_wrapper& operator<<(int i);
	uart_wrapper& operator<<(double d);
	uart_wrapper& operator<<(float f);
	bool get_tx_available()const { return tx_available; }
	bool get_rx_available()const { return rx_available; }
	void set_tx_available(){ tx_available = true; }
	void set_rx_available(){ rx_available = true; }
	void receive_byte();
	char get_char(){ return buffer.get_from_buffer(); }
};

#endif
