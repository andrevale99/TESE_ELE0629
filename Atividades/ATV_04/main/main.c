#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <freertos/queue.h>

#include <driver/uart.h>
#include <driver/i2c.h>

#include <esp_log.h>
#include <esp_check.h>

#include <mpu6050.h>

#define TXD1_PIN        26
#define RXD1_PIN        25
#define RX_BUFFER_SIZE  256

#define I2C_MASTER_SCL_IO   32                           
#define I2C_MASTER_SDA_IO   33                           
#define I2C_MASTER_FREQ_HZ  400000     
//===================================================================
//  VARS & FUNCS
//===================================================================
SemaphoreHandle_t xSemaphoreMutex = NULL; //Mutex para os dados do MPU

//  @brief Task para envio dos dados do MPU
//  pela serial
void vTaskUART_TX(void *pvParameter);
TaskHandle_t xUartHandle;
const char *TAG_UART = "[UART_TX]";

// @brief Task para requisitar os dados do MPU
void vTaskMPU(void *pvParameter);
TaskHandle_t xMPUHandle = NULL;
const char *TAG_MPU = "[MPU]";

mpu6050_handle_t xmpu6050Handle = NULL;
mpu6050_raw_gyro_value_t xRawGyro = {};
mpu6050_raw_acce_value_t xRawAcce = {};
mpu6050_gyro_value_t xGyro = {};
mpu6050_acce_value_t xAcce = {};
// @brief Funcao de configuracao da Serial
void vUart_Setup();

//  @brief Funcao de configuracao do I2C
void vI2C_Setup();

//===================================================================
//  APP MAIN
//===================================================================
void app_main(void)
{
    vUart_Setup();
    vI2C_Setup();

    xTaskCreate(vTaskUART_TX, "Task UART TX", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xUartHandle);

    while(1)
    {
        vTaskDelay( 1000 / portTICK_PERIOD_MS);
    }
}

//===================================================================
//  FUNCS
//===================================================================
void vTaskUART_TX(void *pvParameter)
{
    while(1)
    {
        // ucTxBytes = uart_write_bytes(UART_NUM_1, &fData, ucLenData);
        // ESP_LOGI(TAG_UART, "Bytes Send: %d \t Message: %f", ucTxBytes, fData);
        // fData = 0.1 + fData;

        vTaskDelay( pdMS_TO_TICKS(100) );
    }
}

void vTaskMPU(void *pvParameter)
{

}

void vUart_Setup()
{
    ESP_LOGI("[UART CONF]", "Configurando a UART");

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
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_1, &uart_config)); 
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_1, TXD1_PIN, RXD1_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_1, RX_BUFFER_SIZE, 0, 0, NULL, 0));    
    
    ESP_LOGI("[UART CONF]", "Configuracao da UART FINALIZADA");
}

void vI2C_Setup()
{
    ESP_LOGI("[I2C CONF]", "Configurando o I2C");

	i2c_config_t i2c_config = {
	.mode = I2C_MODE_MASTER,                    
	.sda_io_num = I2C_MASTER_SDA_IO,            
    .sda_pullup_en = GPIO_PULLUP_ENABLE,        
	.scl_io_num = I2C_MASTER_SCL_IO,            
    .scl_pullup_en = GPIO_PULLUP_ENABLE,        
	.master.clk_speed = I2C_MASTER_FREQ_HZ,     
    };

	ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &i2c_config));                                             
	ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0)); 
    
    ESP_LOGI("[I2C CONF]", "Configuracao da I2C FINALIZADA");
}