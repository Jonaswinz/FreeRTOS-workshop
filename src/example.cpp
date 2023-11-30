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

struct WorkloadParams{
    uint8_t index;
    uint16_t delay;
};

SemaphoreHandle_t mutex;
TickType_t xLastWakeTime;

//Function to safely write to the serial console
void vSafePrint(const char *out) {

    //Wait for the semaphore to be free and take it
    xSemaphoreTake(mutex, portMAX_DELAY);
    //Print the cpu core the function is running on
    printf("%u:", get_core_num());
    //Print the input string
    printf(out);
    //Return the semaphore
    xSemaphoreGive(mutex);

}

//Task to blink the internal LED
void led_task(void *parameters)
{   

    xLastWakeTime = xTaskGetTickCount();

    //Setup RP2040 pin as output, where the LED is connected to
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    //Endless blink loop
    while (true) {
        //Print that the led is blinking
        vSafePrint("Blink\n") ;
        //Led on
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        //Delay until deadline of 500ms
        vTaskDelayUntil(&xLastWakeTime, 50);
        //Led off
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        //Delay until deadline of 500ms
        vTaskDelayUntil(&xLastWakeTime, 50);
    }
}

//Task to blink the internal LED
void workload_task(void *parameters)
{   
    //"Tell" the pointer that is pointing to a WorkloadParams object
    WorkloadParams * params = (WorkloadParams *) parameters;

    while(true){
        
        TickType_t start = xTaskGetTickCount();

        //BLocking wait (without blocking)
        while(xTaskGetTickCount() < start+params->delay){}

        //Output
        char output[50];
        snprintf(output, 50, "Workload %d finished\n", params->index);
        vSafePrint(output) ;
    }
}

int main()
{
    //Initializes the pico types
    stdio_init_all();

    //Create a simple semaphore (mutex)
    mutex = xSemaphoreCreateMutex();

    //Create a task with:
    //  That should run the function led_task
    //  Name "Led_Task"
    //  Stack depth of 256 (with 16Bit width = 512Byte)
    //  Without any parameter to hand to led_task
    //  With a priority of 2
    //  Without a handle (additional feature)
    xTaskCreate(led_task, "LED_Task", 256, NULL, 2, NULL);

    //Create workload task and pasing WorkloadParams
    WorkloadParams params1 = {0, 500};
    xTaskCreate(workload_task, "Workload_Task", 256, &params1, 1, NULL);

    //Create workload task and pasing WorkloadParams
    WorkloadParams params2 = {1, 200};
    xTaskCreate(workload_task, "Workload_Task", 256, &params2, 1, NULL);

    //Starts the scheduler
    vTaskStartScheduler();

    while(1){};
}