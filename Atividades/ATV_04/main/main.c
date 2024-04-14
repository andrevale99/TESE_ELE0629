#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

SemaphoreHandle_t xSemaphoreMutex = NULL;

void vTaskUART_TX(void *pvParameter);
TaskHandle_t xUartHandle;
const char *TAG_UART = "[UART_TX]";

void vTaskMPU(void *pvParameter);
TaskHandle_t xMPUHandle = NULL;
const char *TAG_MPU = "[MPU]";

void app_main(void)
{

}

void vTaskUART_TX(void *pvParameter)
{
    while(1)
    {

    }
}
