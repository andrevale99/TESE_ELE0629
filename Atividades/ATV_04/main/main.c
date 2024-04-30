#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <freertos/queue.h>

#include <driver/uart.h>
#include <driver/i2c_master.h>

#include <esp_log.h>
#include <esp_types.h>
#include <esp_check.h>

#include <BME280.h>

#define TXD1_PIN 26
#define RXD1_PIN 25
#define RX_BUFFER_SIZE 256

#define I2C_MASTER_SCL_IO 22
#define I2C_MASTER_SDA_IO 21
#define I2C_MASTER_FREQ_HZ 100000
//===================================================================
//  VARS & FUNCS
//===================================================================
SemaphoreHandle_t xSemaphoreMutex = NULL; // Mutex para os dados do BME

//  @brief Task para envio dos dados do MPU
//  pela serial
void vTaskUART_TX(void *pvParameter);
TaskHandle_t xTaskUARTHandle;
const char *TAG_UART = "[UART_TX]";

// @brief Task para requisitar os dados do MPU
void vTaskBME(void *pvParameter);
TaskHandle_t xTaskBMEHandle = NULL;
const char *TAG_BME = "[BME]";

int32_t Temperatura = 0;

// @brief Funcao de configuracao da Serial
void vUart_Setup();

//  @brief Funcao de configuracao do I2C
void vI2C_Setup();
i2c_master_bus_handle_t i2c_master_handle;

//===================================================================
//  APP MAIN
//===================================================================
void app_main(void)
{
    xSemaphoreMutex = xSemaphoreCreateMutex();
    
    vUart_Setup();
    vI2C_Setup();
    
    xTaskCreate(vTaskUART_TX, "TaskUARTTX", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTaskUARTHandle);
    xTaskCreate(vTaskBME, "TaskBME", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTaskBMEHandle);
}

//===================================================================
//  FUNCS
//===================================================================
void vTaskUART_TX(void *pvParameter)
{
    uint8_t len_Temperatura = sizeof(Temperatura);

    while (1)
    {
        if(xSemaphoreTake(xSemaphoreMutex, 1000 / portTICK_PERIOD_MS) == pdTRUE)
        {
            int txBytes = uart_write_bytes(UART_NUM_1, &Temperatura, len_Temperatura);
            // ESP_LOGI(TAG_UART, "Tx bytes: %i", txBytes);
            xSemaphoreGive(xSemaphoreMutex); 
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vTaskBME(void *pvParameter)
{
    bme280_t bme280;

    i2c_master_dev_handle_t bme280Device_handle;

    i2c_device_config_t bme280Device_config = 
    {
        .device_address = BME280_ADDRESS_0,
        .dev_addr_length = I2C_ADDR_BIT_7,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ
    };

    bme280_set_timeout(&bme280, 100);

    ESP_ERROR_CHECK(i2c_master_bus_add_device(i2c_master_handle, 
                    &bme280Device_config, &bme280Device_handle));

    bme280_get_trimming_params_temp(&bme280, bme280Device_handle);


    uint8_t addrs_adc_T[] = {BME280_TEMP_MSB , BME280_TEMP_LSB };
    uint8_t buffer[] = {0, 0};
    int32_t adc_T = 0;
    int32_t *fine_temp = 0;
    while (1)
    {
        i2c_master_transmit_receive(bme280Device_handle, &addrs_adc_T, 2,
                                    &buffer, 2, bme280.Timeout);
        
        adc_T = ((int32_t)buffer[0] << 8) | ((int32_t)buffer[1]);
        
        if(xSemaphoreTake(xSemaphoreMutex, 1000 / portTICK_PERIOD_MS) == pdTRUE)
        {
            bme280_get_temperature(&bme280, adc_T, &fine_temp);
            Temperatura = bme280.Temperatura;
            
            xSemaphoreGive(xSemaphoreMutex); 
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
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

    // Passando a estrutura, configurando os pinos e instalando o driver uart
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_1, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_1, TXD1_PIN, RXD1_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_1, RX_BUFFER_SIZE, 0, 0, NULL, 0));

    ESP_LOGI("[UART CONF]", "Configuracao da UART FINALIZADA");
}

void vI2C_Setup()
{
    ESP_LOGI("[I2C CONF]", "Configurando o I2C");

    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &i2c_master_handle));

    ESP_LOGI("[I2C CONF]", "Configuracao da I2C FINALIZADA");
}
