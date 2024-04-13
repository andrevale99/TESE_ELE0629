#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_log.h"

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
TaskHandle_t xTaskleitura1Handle = NULL;
const char *TAG_TASKLEITURA1 = "[TASKLEITURA_1]";

void vTaskLeitura2(void *pvParameter);
TaskHandle_t xTaskleitura2Handle = NULL;
const char *TAG_TASKLEITURA2 = "[TASKLEITURA_2]";

void app_main(void)
{

}

void vTaskTemperatura(void *pvParameter)
{

}

void vTaskUmidade(void *pvParameter)
{

}

void vTaskVelocidade(void *pvParameter)
{

}

void vTaskPeso(void *pvParameter)
{

}

void vTaskDistancia(void *pvParameter)
{

}