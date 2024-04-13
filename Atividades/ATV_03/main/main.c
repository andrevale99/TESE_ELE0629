#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "esp_log.h"
#include "esp_err.h"

#define LOW_PRIORITY 0
#define SIZE_BYTES_TASK 2048
#define MAX_WRITES_TASK 5
#define MAX_LENGTH_QUEU 10

SemaphoreHandle_t xMutexSemaphore = NULL;
QueueHandle_t xQueue = NULL;

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
    xMutexSemaphore = xSemaphoreCreateMutex();
    xQueue = xQueueCreate(MAX_LENGTH_QUEU, sizeof(char *)*15);

    BaseType_t iTestCreateTask = pdFAIL;    

    iTestCreateTask = xTaskCreate(vTaskTemperatura, "Task Umidade", SIZE_BYTES_TASK, NULL, LOW_PRIORITY+1, &xTaskTemperatura_Handle);
    if (iTestCreateTask == pdFAIL)
        ESP_LOGW(TAG_UMIDADE, "Erro ao criar a Task");
    
    // iTestCreateTask = xTaskCreate(vTaskUmidade, "Task Umidade", SIZE_BYTES_TASK, NULL, LOW_PRIORITY+1, &xTaskUmidade_Handle);
    // if (iTestCreateTask == pdFAIL)
    //     ESP_LOGW(TAG_UMIDADE, "Erro ao criar a Task");
    
    // iTestCreateTask = xTaskCreate(vTaskVelocidade, "Task Velocidade", SIZE_BYTES_TASK, NULL, LOW_PRIORITY+1, &xTaskVelocidade_Handle);
    // if (iTestCreateTask == pdFAIL)
    //     ESP_LOGW(TAG_VELOCIDADE, "Erro ao criar a Task");
    
    // iTestCreateTask = xTaskCreate(vTaskPeso, "Task Peso", SIZE_BYTES_TASK, NULL, LOW_PRIORITY+1, &xTaskPeso_Handle);
    // if (iTestCreateTask == pdFAIL)
    //     ESP_LOGW(TAG_PESO, "Erro ao criar a Task");
    
    // iTestCreateTask = xTaskCreate(vTaskDistancia, "Task Distancia", SIZE_BYTES_TASK, NULL, LOW_PRIORITY+1, &xTaskDistancia_Handle);
    // if (iTestCreateTask == pdFAIL)
    //     ESP_LOGW(TAG_DISTANCIA, "Erro ao criar a Task");
    
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
    const char *ucDataTemperatura = "Temperatura\0";

    while(1)
    {
        if (ucContTemperatura == MAX_WRITES_TASK)
        {
            ESP_LOGI(TAG_TEMPERATURA, "Escrita Finalizada");
            vTaskDelete(xTaskTemperatura_Handle);
        }
        else if(xSemaphoreTake(xMutexSemaphore, portMAX_DELAY) == pdTRUE)
        {
            ESP_LOGW(TAG_TEMPERATURA, "Ecrita Realizada");

            xQueueSend(xQueue, &ucDataTemperatura, portMAX_DELAY);
            ucContTemperatura += 1;
            xSemaphoreGive(xMutexSemaphore);            
            vTaskDelay( 100 / portTICK_PERIOD_MS);
        }
    }
}

void vTaskUmidade(void *pvParameter)
{
    ESP_LOGI(TAG_UMIDADE, "Task Criada");
    uint8_t ucContUmidade = 0;
    const char *ucDataUmidade = "Umidade\0";

    while(1)
    {
        if (ucContUmidade == MAX_WRITES_TASK)
        {
            ESP_LOGI(TAG_UMIDADE, "Escrita Finalizada");
            vTaskDelete(xTaskUmidade_Handle);
        }
        else if(xSemaphoreTake(xMutexSemaphore, portMAX_DELAY) == pdTRUE)
        {

            ESP_LOGW(TAG_UMIDADE, "Ecrita Realizada");

            xQueueSend(xQueue, &ucDataUmidade, portMAX_DELAY);
            ucContUmidade += 1;
            xSemaphoreGive(xMutexSemaphore);            
            vTaskDelay( 100 / portTICK_PERIOD_MS);
        }
    }
}

void vTaskVelocidade(void *pvParameter)
{
    ESP_LOGI(TAG_VELOCIDADE, "Task Criada");
    uint8_t ucContVelocidade = 0;
    const char *ucDataVelocidade = "Velocidade\0";

    while(1)
    {
        if (ucContVelocidade == MAX_WRITES_TASK)
        {
            ESP_LOGI(TAG_VELOCIDADE, "Escrita Finalizada");
            vTaskDelete(xTaskVelocidade_Handle);
        }
        else if(xSemaphoreTake(xMutexSemaphore, portMAX_DELAY) == pdTRUE)
        {
            ESP_LOGW(TAG_VELOCIDADE, "Ecrita Realizada");

            xQueueSend(xQueue, &ucDataVelocidade, portMAX_DELAY);
            ucContVelocidade += 1;
            xSemaphoreGive(xMutexSemaphore);            
            vTaskDelay( 100 / portTICK_PERIOD_MS);
        }
    }
}

void vTaskPeso(void *pvParameter)
{
    ESP_LOGI(TAG_PESO, "Task Criada");
    uint8_t ucContPeso = 0;
    const char *ucDataPeso = "Peso\0";

    while(1)
    {
        if (ucContPeso == MAX_WRITES_TASK)
        {
            ESP_LOGI(TAG_PESO, "Escrita Finalizada");
            vTaskDelete(xTaskPeso_Handle);
        }
        else if(xSemaphoreTake(xMutexSemaphore, portMAX_DELAY) == pdTRUE)
        {
            // ESP_LOGI(TAG_PESO, "Ecrita Realizada %u", ucContPeso);
            xQueueSend(xQueue, &ucDataPeso, portMAX_DELAY);
            ucContPeso += 1;
            xSemaphoreGive(xMutexSemaphore);            
            vTaskDelay( 500 / portTICK_PERIOD_MS);
        }
    }
}

void vTaskDistancia(void *pvParameter)
{  
    ESP_LOGI(TAG_DISTANCIA, "Task Criada");
    uint8_t ucContDistancia = 0;
    const char *ucDataDistancia = "Distancia\0";

    while(1)
    {
        if (ucContDistancia == MAX_WRITES_TASK)
        {
            ESP_LOGI(TAG_DISTANCIA, "Escrita Finalizada");
            vTaskDelete(xTaskDistancia_Handle);
        }
        else if(xSemaphoreTake(xMutexSemaphore, portMAX_DELAY) == pdTRUE)
        {
            // ESP_LOGI(TAG_DISTANCIA, "Ecrita Realizada %u", ucContDistancia);
            xQueueSend(xQueue, &ucDataDistancia, portMAX_DELAY);
            ucContDistancia += 1;
            xSemaphoreGive(xMutexSemaphore);            
            vTaskDelay( 100 / portTICK_PERIOD_MS);
        }
    }
}

void vTaskLeitura1(void *pvParameter)
{
    ESP_LOGI(TAG_TASKLEITURA1, "Task Criada");
    char *ucStringReceive = "0123456789abcd\0";
    uint8_t ucContLeitura1 = 0;

    while(1)
    {
        if(xSemaphoreTake(xMutexSemaphore, portMAX_DELAY) == pdTRUE)
        {
            if (xQueueReceive(xQueue, &ucStringReceive, portMAX_DELAY) == pdTRUE)
            {
                ucContLeitura1 += 1;
                ESP_LOGI(TAG_TASKLEITURA1, "Received: %s", ucStringReceive);
            }

            xSemaphoreGive(xMutexSemaphore);            
            vTaskDelay( 100 / portTICK_PERIOD_MS);
        }
    }
}

void vTaskLeitura2(void *pvParameter)
{
    ESP_LOGI(TAG_TASKLEITURA2, "Task Criada");
    char *ucStringReceive = "0123456789abcd\0";
    uint8_t ucContLeitura2 = 0;

    while(1)
    {
        if(xSemaphoreTake(xMutexSemaphore, portMAX_DELAY) == pdTRUE)
        {
            if (xQueueReceive(xQueue, &ucStringReceive, portMAX_DELAY) == pdTRUE)
            {
                ucContLeitura2 += 1;
                ESP_LOGI(TAG_TASKLEITURA2, "Received: %s", ucStringReceive);
            }

            xSemaphoreGive(xMutexSemaphore);            
            vTaskDelay( 200 / portTICK_PERIOD_MS);
        }
    }
}