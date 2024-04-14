#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_random.h"
#include "esp_log.h"

static int var_global = 0;

void Task1(void *pvParameter);
void Task2(void *pvParameter);
TaskHandle_t xTaskTask1Handle = NULL;
TaskHandle_t xTaskTask2Handle = NULL;

//SemaphoreHandle_t xMutexSemaphore = NULL;

void app_main(void)
{
//    xMutexSemaphore = xSemaphoreCreateMutex();
    xTaskCreate(Task1, "Task1", 2048, NULL, 1, &xTaskTask1Handle);
    xTaskCreate(Task2, "Task2", 2048, NULL, 4, &xTaskTask2Handle);
}

void Task1(void *pvParameter){
    int var_local;

    while(1){
        //if(xSemaphoreTake(xMutexSemaphore, portMAX_DELAY) == pdTRUE){
            var_local = var_global;
            var_local++;
            vTaskDelay(((uint)esp_random()/0x004FFFFF) / portTICK_PERIOD_MS);
            var_global = var_local;
            ESP_LOGI("Task1","Variavel Global = %d", var_global);

       //     xSemaphoreGive(xMutexSemaphore);
            vTaskDelay(10/portTICK_PERIOD_MS);
       // }
       // else{
            //fazer algo enquanto espera
       // }
    }
}

void Task2(void *pvParameter){
    int var_local;

    while(1){
       // if(xSemaphoreTake(xMutexSemaphore, portMAX_DELAY) == pdTRUE){
            var_local = var_global;
            var_local++;
            vTaskDelay(((uint)esp_random()/0x004FFFFF) / portTICK_PERIOD_MS);
            var_global = var_local;
            ESP_LOGI("Task2","Variavel Global = %d", var_global);

        //    xSemaphoreGive(xMutexSemaphore);
            vTaskDelay(10/portTICK_PERIOD_MS);
        //}
        //else{
            //fazer algo enquanto espera
       // }
    }
}