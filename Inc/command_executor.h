#ifndef command_exec_h
#define command_exec_h

#include "main.h"
#include "cmsis_os.h"

constexpr uint8_t cmd_quantity{ 17 };


class command_exec{
private:
	osSemaphoreId_t cmd_exec_sem[cmd_quantity];

	static void set_x(void* p);
	static void set_y(void* p);
	static void set_z(void* p);
	static void set_point(void* p);
	static void open_handle(void* p);
	static void close_handle(void* p);
	static void set_handle_rotation(void* p);
	static void inc_var1(void* p);
	static void dec_var1(void* p);
	static void inc_var2(void* p);
	static void dec_var2(void* p);
	static void inc_var3(void* p);
	static void dec_var3(void* p);
	static void inc_var4(void* p);
	static void dec_var4(void* p);
	static void inc_var5(void* p);
	static void dec_var5(void* p);

public:
	static void main(void* p);
};

#endif
