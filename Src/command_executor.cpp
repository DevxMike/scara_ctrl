#include "command_executor.h"
#include "communication.h"

const char* cmd_executed_code = "A2E";
const char* no_such_cmd_code = "A3E";

const osThreadAttr_t set_x_attr = {
	.name = "set_x",
	.priority = osPriorityLow,
};

const osThreadAttr_t set_y_attr = {
	.name = "set_y",
	.priority = osPriorityLow,
};

const osThreadAttr_t set_z_attr = {
	.name = "set_z",
	.priority = osPriorityLow,
};

const osThreadAttr_t set_point_attr = {
	.name = "set_point",
	.priority = osPriorityLow,
};

const osThreadAttr_t inc_var1_attr = {
	.name = "inc1",
	.priority = osPriorityLow,
};

const osThreadAttr_t inc_var2_attr = {
	.name = "inc2",
	.priority = osPriorityLow,
};

const osThreadAttr_t inc_var3_attr = {
	.name = "inc3",
	.priority = osPriorityLow,
};

const osThreadAttr_t inc_var4_attr = {
	.name = "inc4",
	.priority = osPriorityLow,
};

const osThreadAttr_t inc_var5_attr = {
	.name = "inc5",
	.priority = osPriorityLow,
};

const osThreadAttr_t dec_var1_attr = {
	.name = "dec1",
	.priority = osPriorityLow,
};

const osThreadAttr_t dec_var2_attr = {
	.name = "dec2",
	.priority = osPriorityLow,
};

const osThreadAttr_t dec_var3_attr = {
	.name = "dec3",
	.priority = osPriorityLow,
};

const osThreadAttr_t dec_var4_attr = {
	.name = "dec4",
	.priority = osPriorityLow,
};

const osThreadAttr_t dec_var5_attr = {
	.name = "dec5",
	.priority = osPriorityLow,
};

const osThreadAttr_t open_handle_attr = {
	.name = "open_handle",
	.priority = osPriorityLow,
};

const osThreadAttr_t close_handle_attr = {
	.name = "close_handle",
	.priority = osPriorityLow,
};

const osThreadAttr_t set_handle_rot_attr = {
	.name = "set_rot",
	.priority = osPriorityLow,
};


void command_exec::main(void* p){
	osThreadNew(set_x, NULL, &set_x_attr);
	osThreadNew(set_y, NULL, &set_y_attr);
	osThreadNew(set_z, NULL, &set_z_attr);
	osThreadNew(set_point, NULL, &set_point_attr);
	osThreadNew(open_handle, NULL, &open_handle_attr);
	osThreadNew(close_handle, NULL, &close_handle_attr);
	osThreadNew(set_handle_rotation, NULL, &set_handle_rot_attr);
	osThreadNew(inc_var1, NULL, &inc_var1_attr);
	osThreadNew(dec_var1, NULL, &dec_var1_attr);
	osThreadNew(inc_var2, NULL, &inc_var2_attr);
	osThreadNew(dec_var2, NULL, &dec_var2_attr);
	osThreadNew(inc_var3, NULL, &inc_var3_attr);
	osThreadNew(dec_var3, NULL, &dec_var3_attr);
	osThreadNew(inc_var4, NULL, &inc_var4_attr);
	osThreadNew(inc_var4, NULL, &dec_var4_attr);
	osThreadNew(inc_var5, NULL, &inc_var5_attr);
	osThreadNew(dec_var5, NULL, &dec_var5_attr);

	for(uint8_t i = 0; i < cmd_quantity; ++i){
		osSemaphoreAcquire(cmd_exec_sem[i], osWaitForever);
	}

	while(1){
		osSemaphoreAcquire(communicator::cmd_ready_sem, osWaitForever);
		auto cmd = communicator::cmd_num;

		if(cmd >= 0 && cmd < 17){
			osSemaphoreRelease(cmd_exec_sem[cmd]);
		}
		else{
			communicator::uart_handle << no_such_cmd_code;
		}
	}
	osThreadTerminate(NULL);
}

void command_exec::set_x(void* p){
	while(1){
		osSemaphoreAcquire(cmd_exec_sem[0], osWaitForever);
	}
	osThreadTerminate(NULL);
}

void command_exec::set_y(void* p){
	while(1){
		osSemaphoreAcquire(cmd_exec_sem[1], osWaitForever);

	}
	osThreadTerminate(NULL);
}

void command_exec::set_z(void* p){
	while(1){
		osSemaphoreAcquire(cmd_exec_sem[2], osWaitForever);
	}
	osThreadTerminate(NULL);
}

void command_exec::set_point(void* p){
	while(1){
		osSemaphoreAcquire(cmd_exec_sem[3], osWaitForever);
	}
	osThreadTerminate(NULL);
}

void command_exec::open_handle(void* p){
	while(1){
		osSemaphoreAcquire(cmd_exec_sem[4], osWaitForever);
	}
	osThreadTerminate(NULL);
}

void command_exec::close_handle(void* p){
	while(1){
		osSemaphoreAcquire(cmd_exec_sem[5], osWaitForever);
	}
	osThreadTerminate(NULL);
}

void command_exec::set_handle_rotation(void* p){
	while(1){
		osSemaphoreAcquire(cmd_exec_sem[6], osWaitForever);
	}
	osThreadTerminate(NULL);
}

void command_exec::inc_var1(void* p){
	while(1){
		osSemaphoreAcquire(cmd_exec_sem[7], osWaitForever);
	}
	osThreadTerminate(NULL);
}

void command_exec::dec_var1(void* p){
	while(1){
		osSemaphoreAcquire(cmd_exec_sem[8], osWaitForever);
	}
	osThreadTerminate(NULL);
}

void command_exec::inc_var2(void* p){
	while(1){
		osSemaphoreAcquire(cmd_exec_sem[9], osWaitForever);
	}
	osThreadTerminate(NULL);
}

void command_exec::dec_var2(void* p){
	while(1){
		osSemaphoreAcquire(cmd_exec_sem[10], osWaitForever);
	}
	osThreadTerminate(NULL);
}

void command_exec::inc_var3(void* p){
	while(1){
		osSemaphoreAcquire(cmd_exec_sem[11], osWaitForever);
	}
	osThreadTerminate(NULL);
}

void command_exec::dec_var3(void* p){
	while(1){
		osSemaphoreAcquire(cmd_exec_sem[12], osWaitForever);
	}
	osThreadTerminate(NULL);
}

void command_exec::inc_var4(void* p){
	while(1){
		osSemaphoreAcquire(cmd_exec_sem[13], osWaitForever);
	}
	osThreadTerminate(NULL);
}

void command_exec::dec_var4(void* p){
	while(1){
		osSemaphoreAcquire(cmd_exec_sem[14], osWaitForever);
	}
	osThreadTerminate(NULL);
}

void command_exec::inc_var5(void* p){
	while(1){
		osSemaphoreAcquire(cmd_exec_sem[15], osWaitForever);
	}
	osThreadTerminate(NULL);
}

void command_exec::dec_var5(void* p){
	while(1){
		osSemaphoreAcquire(cmd_exec_sem[16], osWaitForever);
	}
	osThreadTerminate(NULL);
}
