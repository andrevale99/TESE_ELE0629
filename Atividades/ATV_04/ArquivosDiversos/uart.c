#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "string.h"
#include "esp_system.h"
#include "esp_log.h"

#define TXD1_PIN 26
#define RXD1_PIN 25

static const int RX_BUF_SIZE = 512;        

int sendData(const char* logName, const char* data);
void tx_task(void *pvParameters);
void rx_task(void *pvParameters);

void app_main(void)
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
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE, 0, 0, NULL, 0);    
                           


    xTaskCreate(rx_task, "uart_rx_task", configMINIMAL_STACK_SIZE + 1024, NULL, 1, NULL);
    xTaskCreate(tx_task, "uart_tx_task", configMINIMAL_STACK_SIZE + 1024, NULL, 1, NULL);
}


int sendData(const char* logName, const char* data)
{
    const int len = strlen(data);                                   
    const int txBytes = uart_write_bytes(UART_NUM_1, data, len);    
    ESP_LOGI(logName, "Wrote %d bytes - Mesage: %s", txBytes,data); 
    return txBytes;                                                 // Retorna o número de dados enviados
}


void tx_task(void *pvParameters)
{
    int cnt = 0;
    char data[64];
    static const char *TX_TASK_TAG = "TX_TASK";    //Para enviar a função sendData a TAG da task                
    esp_log_level_set(TX_TASK_TAG, ESP_LOG_INFO);                  

    while (1) 
    {
        sprintf(data, "Counting: %d\r\n", cnt);                    
        sendData(TX_TASK_TAG, data);                  
        cnt++;
        vTaskDelay(pdMS_TO_TICKS(1000));                             
    }
}


void rx_task(void *pvParameters)
{
    static const char *RX_TASK_TAG = "RX_TASK";            
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);          

    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);      // "RX_BUF_SIZE+1" para colocar o terminador nulo ao fim da string

    while (1) 
    {
        const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, pdMS_TO_TICKS(1000));    // read data from UART
        if (rxBytes > 0)                                                        
        {                                                                   
            data[rxBytes] = '\0';     // Adicionando terminador nulo
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, data);       
        }
    }
}