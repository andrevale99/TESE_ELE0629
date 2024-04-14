#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "driver/uart.h"
#include "driver/i2c.h"

#include "esp_log.h"
#include "esp_check.h"

#define TXD1_PIN 26
#define RXD1_PIN 25
#define TX_BUFFER_SIZE 256

SemaphoreHandle_t xSemaphoreMutex = NULL;

void vTaskUART_TX(void *pvParameter);
TaskHandle_t xUartHandle;
const char *TAG_UART = "[UART_TX]";

void vTaskMPU(void *pvParameter);
TaskHandle_t xMPUHandle = NULL;
const char *TAG_MPU = "[MPU]";

void vUart_Setup();
void vI2C_Setup();

void app_main(void)
{
    vUart_Setup();
    vI2C_Setup();

    while(1)
    {
        vTaskDelay( 1000 / portTICK_PERIOD_MS);
    }
}

void vTaskUART_TX(void *pvParameter)
{
    while(1)
    {

    }
}

void vTaskMPU(void *pvParameter)
{

}

void vUart_Setup()
{
    // Estrutura com parametros da uart
    const uart_config_t uart_config = {
        .baud_rate = 115200,                       
        .data_bits = UART_DATA_8_BITS,             
        .parity = UART_PARITY_DISABLE,              
        .stop_bits = UART_STOP_BITS_1,              
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,     
        .source_clk = UART_SCLK_DEFAULT,            
    };

    //Passando a estrutura, configurando os pinos e instalando o driver uart
    uart_param_config(UART_NUM_1, &uart_config); 
    uart_set_pin(UART_NUM_1, TXD1_PIN, RXD1_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_1, 0, 32, 0, NULL, 0);    
 
}

void vI2C_Setup()
{

}