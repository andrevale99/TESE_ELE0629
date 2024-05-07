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

#include <BMP280.h>

#define TXD1_PIN 1
#define RXD1_PIN 3
#define RX_BUFFER_SIZE 256

#define I2C_MASTER_SCL_IO 19
#define I2C_MASTER_SDA_IO 18
#define I2C_MASTER_FREQ_HZ 100000
//===================================================================
//  VARS & FUNCS
//===================================================================
SemaphoreHandle_t xSemaphoreBinario = NULL; // Mutex para os dados do BME

//  @brief Task para envio dos dados do MPU
//  pela serial
void vTaskUART_TX(void *pvParameter);
TaskHandle_t xTaskUARTHandle;
const char *TAG_UART = "[UART_TX]";

// @brief Task para requisitar os dados do MPU
void vTaskBMP(void *pvParameter);
TaskHandle_t xTaskBMPHandle = NULL;
const char *TAG_BMP = "[BMP]";

int32_t Temperatura = 0;

static i2c_master_bus_handle_t i2c_master_handle;
//===================================================================
//  APP MAIN
//===================================================================
void app_main(void)
{
    esp_log_level_set("*", ESP_LOG_DEBUG);

    xSemaphoreBinario = xSemaphoreCreateBinary();

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

    xTaskCreate(vTaskBMP, "TaskBMP", configMINIMAL_STACK_SIZE + 4096, NULL, 1, &xTaskBMPHandle);
    xTaskCreate(vTaskUART_TX, "TaskUARTTX", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTaskUARTHandle);

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));

        // ESP_LOGI("[MAIN]", "Main");
    }
}

//===================================================================
//  FUNCS
//===================================================================
void vTaskUART_TX(void *pvParameter)
{
    int txBytes = -1;
    char buffer[6];

    while (1)
    {
        if (xSemaphoreTake(xSemaphoreBinario, portMAX_DELAY) == pdTRUE)
        {
            snprintf(buffer, 5, "%ld\n", Temperatura);
            txBytes = uart_write_bytes(UART_NUM_1, buffer, sizeof(buffer));
            
            // ESP_LOGI(TAG_UART, "Tx bytes: %i | MSG: %s", txBytes, &buffer[0]);
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void vTaskBMP(void *pvParameter)
{
    bmp280_t bmp280;

    int32_t fine_temp = 0;

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = BMP280_ADDRESS_0,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ,
    };

    i2c_master_dev_handle_t dev_handle;
    i2c_master_bus_add_device(i2c_master_handle, &dev_cfg, &dev_handle);

    bmp280_set_timeout(&bmp280, 1000);

    //Delay para estabilizar o sensor
    vTaskDelay( pdMS_TO_TICKS(10) );

    bmp280_set_standby(&bmp280, dev_handle, TSB1000);
    bmp280_set_oversamplig(&bmp280, dev_handle, SKIP, x1);
    bmp280_set_mode(&bmp280, dev_handle, NORMAL);

    bmp280_get_trimming_params_temp(&bmp280, dev_handle);

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(2000));

        if (!(bmp280_is_measuring(&bmp280, dev_handle)))
        {
            bmp280_get_adc_T_P(&bmp280, dev_handle);
            bmp280_get_compesate_temperature(&bmp280, bmp280.adc_T, &fine_temp);

            Temperatura = bmp280.Temperature;

            //mudar para fila para enviar a var.
            xSemaphoreGive(xSemaphoreBinario);
        }
    }
}