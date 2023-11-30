#include <FreeRTOS.h>
#include <pico/stdlib.h>
#include <pico/bootrom.h>
#include <pico/malloc.h>
#include <pico/mutex.h>
#include <hardware/uart.h>
#include <semphr.h>
#include <task.h>
#include <queue.h>
#include <stdio.h>
#include <string.h>


//Empty task
void task(void *parameters)
{   

    //Endless execution loop
    while (true) {
        
        //TODO: implement something

    }
}

int main()
{
    //Initializes the pico types
    stdio_init_all();

    //Create a task with:
    //  That should run the function task
    //  Name "Empty_Task"
    //  Stack depth of 256 (with 16Bit width = 512Byte)
    //  Without any parameter to hand to led_task
    //  With a priority of 4
    //  Without a handle (additional feature)
    xTaskCreate(task, "Empty_Task", 256, NULL, 1, NULL);

    //Starts the scheduler
    vTaskStartScheduler();

    while(1){};
}