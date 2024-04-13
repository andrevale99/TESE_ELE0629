#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_log.h"
#include "esp_err.h"

#define LOW_PRIORITY 0
#define SIZE_BYTES_TASK 2048

void vTaskTemperatura(void *pvParameter);
TaskHandle_t xTaskTemperatura_Handle = NULL;
const char *TAG_TEMPERATURA = "[TEMPERATURA]";

void vTaskUmidade(void *pvParameter);
TaskHandle_t xTaskUmidade_Handle = NULL;
const char *TAG_UMIDADE = "[UMIDADE]";

void vTaskVelocidade(void *pvParameter);
TaskHandle_t xTaskVelocidade_Handle = NULL;
const char *TAG_VELOCIDADE = "[VELOCIDADE]";

void vTaskPeso(void *pvParameter);
TaskHandle_t xTaskPeso_Handle = NULL;
const char *TAG_PESO = "[PESO]";

void vTaskDistancia(void *pvParameter);
TaskHandle_t xTaskDistancia_Handle = NULL;
const char *TAG_DISTANCIA = "[DISTACIA]";

void vTaskLeitura1(void *pvParameter);
TaskHandle_t xTaskleitura_1_Handle = NULL;
const char *TAG_TASKLEITURA1 = "[TASKLEITURA_1]";

void vTaskLeitura2(void *pvParameter);
TaskHandle_t xTaskleitura_2_Handle = NULL;
const char *TAG_TASKLEITURA2 = "[TASKLEITURA_2]";

void app_main(void)
{ 
    BaseType_t iTestCreateTask = NULL;    

    iTestCreateTask = xTaskCreate(vTaskUmidade, "Task Umidade", SIZE_BYTES_TASK, NULL, LOW_PRIORITY+1, &xTaskUmidade_Handle);
    if (iTestCreateTask == pdFAIL)
        ESP_LOGW(TAG_UMIDADE, "Erro ao criar a Task");
    
    iTestCreateTask = xTaskCreate(vTaskVelocidade, "Task Velocidade", SIZE_BYTES_TASK, NULL, LOW_PRIORITY+1, &xTaskVelocidade_Handle);
    if (iTestCreateTask == pdFAIL)
        ESP_LOGW(TAG_VELOCIDADE, "Erro ao criar a Task");
    
    iTestCreateTask = xTaskCreate(vTaskPeso, "Task Peso", SIZE_BYTES_TASK, NULL, LOW_PRIORITY+1, &xTaskPeso_Handle);
    if (iTestCreateTask == pdFAIL)
        ESP_LOGW(TAG_PESO, "Erro ao criar a Task");
    
    iTestCreateTask = xTaskCreate(vTaskDistancia, "Task Distancia", SIZE_BYTES_TASK, NULL, LOW_PRIORITY+1, &xTaskDistancia_Handle);
    if (iTestCreateTask == pdFAIL)
        ESP_LOGW(TAG_DISTANCIA, "Erro ao criar a Task");
    
    iTestCreateTask = xTaskCreate(vTaskLeitura1, "Task Leitura_1", SIZE_BYTES_TASK, NULL, LOW_PRIORITY+1, &xTaskleitura_1_Handle);
    if (iTestCreateTask == pdFAIL)
        ESP_LOGW(TAG_TASKLEITURA1, "Erro ao criar a Task");
    
    iTestCreateTask = xTaskCreate(vTaskLeitura2, "Task Leitura_2", SIZE_BYTES_TASK, NULL, LOW_PRIORITY+1, &xTaskleitura_2_Handle);
    if (iTestCreateTask == pdFAIL)
        ESP_LOGW(TAG_TASKLEITURA2, "Erro ao criar a Task");
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