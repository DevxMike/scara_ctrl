#ifndef neural_net_h
#define neural_net_h

#include "main.h"
#include "cmsis_os.h"

class neural_net{
public:
	static osSemaphoreId_t nn_sem;
	static void main(void* p);
};

#endif
