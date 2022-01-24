#ifndef command_exec_h
#define command_exec_h

#include "main.h"
#include "cmsis_os.h"

constexpr uint8_t cmd_quantity{ 17 };


class command_exec{
public:
	static void main(void* p);
};

#endif
