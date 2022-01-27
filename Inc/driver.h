#ifndef driver_h
#define driver_h

#include "main.h"
#include "cmsis_os.h"

class driver{
public:
	static osSemaphoreId_t driver_sem;
	static void main(void* p);
};

#endif
