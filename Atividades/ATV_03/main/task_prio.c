#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

/****   Definindo TAGS para as Tarefas      ****/
static const char *TAGADC = "Test Task1";
static const char *TAGPWM = "Test Task2";

void vTaskADC(void *pvParameter);      /*Prototipo da TaskADC*/
TaskHandle_t xTaskADCHandle = NULL;    /*Handle da TaskADC*/

void vTaskPWM(void *pvParameter);      /*Prototipo da TaskPWM*/
TaskHandle_t xTaskPWMHandle = NULL;    /*Handle da TaskPWM*/

void app_main(void)
{
    xTaskCreate(vTaskADC, "Task ADC", 2048, NULL, 2, &xTaskADCHandle);
    xTaskCreate(vTaskPWM, "Task PWM", 2048, NULL, 2, &xTaskPWMHandle);
}

/***    Definicoes da TaskADC      ***/
void vTaskADC(void *pvParameter)
{
    while(1){
        ESP_LOGI(TAGADC, "Fazendo Leitura ADC!!!");
        vTaskDelay(1000/portTICK_PERIOD_MS);

        for(int i = 0; i < 1000000; i++){
            /*Lendo ADC*/
        }
    }
}

/***    Definicoes da TaskADC      ***/
void vTaskPWM(void *pvParameter)
{
    while(1){
        ESP_LOGI(TAGPWM, "Atualizando PWM!!!");
        vTaskDelay(1000/portTICK_PERIOD_MS);
        
        for(int i = 0; i < 1000000; i++){
            /*Atualizando PWM*/
        }
    }
}