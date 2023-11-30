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

TickType_t ledTimer;
TickType_t workloadTimer;
TaskHandle_t led_taskHandle;

bool computing = false;
uint8_t ledValue = 0;

//Task to blink the internal LED
void led_task(void *parameters)
{   

    ledTimer = xTaskGetTickCount();

    //Setup RP2040 pin as output, where the LED is connected to
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    //Endless blink loop
    while (true) {

        if(computing){
            gpio_put(PICO_DEFAULT_LED_PIN, 1);
            vTaskSuspend(led_taskHandle);
            gpio_put(PICO_DEFAULT_LED_PIN, 0);
        }else{
            ledValue = !ledValue;
            gpio_put(PICO_DEFAULT_LED_PIN, ledValue);
            vTaskDelayUntil(&ledTimer, 50);
        
        }
    }
}

//Task to blink the internal LED
void workload_task(void *parameters)
{

    while(true){
        
        printf("start");

        computing = true;
        xTaskAbortDelay(led_taskHandle);

        TickType_t start = xTaskGetTickCount();
        //BLocking wait (without blocking)
        while(xTaskGetTickCount() < start+200){}

        computing = false;
        vTaskResume(led_taskHandle);

        printf("finished");

        workloadTimer = xTaskGetTickCount();
        vTaskDelayUntil(&workloadTimer, 500);
    }
}

int main()
{
    //Initializes the pico types
    stdio_init_all();

    //Create a task with:
    //  That should run the function led_task
    //  Name "Led_Task"
    //  Stack depth of 256 (with 16Bit width = 512Byte)
    //  Without any parameter to hand to led_task
    //  With a priority of 2
    //  Without a handle (additional feature)
    xTaskCreate(led_task, "LED_Task", 256, NULL, 2, &led_taskHandle);

    //Create workload task and pasing WorkloadParams
    xTaskCreate(workload_task, "Workload_Task", 256, NULL, 1, NULL);

    //Starts the scheduler
    vTaskStartScheduler();

    while(1){};
}