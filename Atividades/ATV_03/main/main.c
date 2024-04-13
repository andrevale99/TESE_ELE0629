#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_log.h"

#define SIZE_BYTES_TASK 2048

void vTaskTemperatura(void *pvParameter);
TaskHandle_t xTaskTemperaturaHandle = NULL;
const char *TAG_TEMPERATURA = "[TEMPERATURA]";

void vTaskUmidade(void *pvParameter);
TaskHandle_t xTaskUmidadeHandle = NULL;
const char *TAG_UMIDADE = "[UMIDADE]";

void vTaskVelocidade(void *pvParameter);
TaskHandle_t xTaskVelocidadeHandle = NULL;
const char *TAG_VELOCIDADE = "[VELOCIDADE]";

void vTaskPeso(void *pvParameter);
TaskHandle_t xTaskPesoHandle = NULL;
const char *TAG_PESO = "[PESO]";

void vTaskDistancia(void *pvParameter);
TaskHandle_t xTaskDistanciaHandle = NULL;
const char *TAG_DISTANCIA = "[DISTACIA]";

void vTaskLeitura1(void *pvParameter);
TaskHandle_t xTaskleitura_1_Handle = NULL;
const char *TAG_TASKLEITURA1 = "[TASKLEITURA_1]";

void vTaskLeitura2(void *pvParameter);
TaskHandle_t xTaskleitura_2_Handle = NULL;
const char *TAG_TASKLEITURA2 = "[TASKLEITURA_2]";

void app_main(void)
{
    xTaskCreate(vTaskTemperatura, "Task Temperatura", SIZE_BYTES_TASK, NULL, 1, xTaskTemperaturaHandle);
    xTaskCreate(vTaskUmidade, "Task Umidade", SIZE_BYTES_TASK, NULL, 1, xTaskUmidadeHandle);
    xTaskCreate(vTaskVelocidade, "Task Velocidade", SIZE_BYTES_TASK, NULL, 1, xTaskVelocidadeHandle);
    xTaskCreate(vTaskPeso, "Task Peso", SIZE_BYTES_TASK, NULL, 1, xTaskPesoHandle);
    xTaskCreate(vTaskDistancia, "Task Distancia", SIZE_BYTES_TASK, NULL, 1, xTaskDistanciaHandle);
    
    xTaskCreate(vTaskLeitura1, "Task Leitura_1", SIZE_BYTES_TASK, NULL, 1, xTaskleitura_1_Handle);
    xTaskCreate(vTaskLeitura2, "Task Leitura_2", SIZE_BYTES_TASK, NULL, 1, xTaskleitura_2_Handle);
}

void vTaskTemperatura(void *pvParameter)
{
    ESP_LOGI(TAG_TEMPERATURA, "Task Criada");
    uint8_t ucContTemperatura = 0;

    while(1)
    {
        vTaskDelay( 1000 / portTICK_PERIOD_MS);
    }
}

void vTaskUmidade(void *pvParameter)
{
    ESP_LOGI(TAG_UMIDADE, "Task Criada");
    uint8_t ucContUmidade = 0;

    while(1)
    {
        vTaskDelay( 1000 / portTICK_PERIOD_MS);
    }
}

void vTaskVelocidade(void *pvParameter)
{
    ESP_LOGI(TAG_VELOCIDADE, "Task Criada");
    uint8_t ucContVelocidade = 0;

    while(1)
    {
        vTaskDelay( 1000 / portTICK_PERIOD_MS);
    }
}

void vTaskPeso(void *pvParameter)
{
    ESP_LOGI(TAG_PESO, "Task Criada");
    uint8_t ucContPeso = 0;

    while(1)
    {
        vTaskDelay( 1000 / portTICK_PERIOD_MS);
    }
}

void vTaskDistancia(void *pvParameter)
{  
    ESP_LOGI(TAG_DISTANCIA, "Task Criada");
    uint8_t ucContDistancia = 0;

    while(1)
    {
        vTaskDelay( 1000 / portTICK_PERIOD_MS);
    }
}

void vTaskLeitura1(void *pvParameter)
{
    ESP_LOGI(TAG_TASKLEITURA1, "Task Criada");

    while(1)
    {
        vTaskDelay( 1000 / portTICK_PERIOD_MS);
    }
}

void vTaskLeitura2(void *pvParameter)
{
    ESP_LOGI(TAG_TASKLEITURA2, "Task Criada");

    while(1)
    {
        vTaskDelay( 1000 / portTICK_PERIOD_MS);
    }
}