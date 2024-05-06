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

#define TXD1_PIN 26
#define RXD1_PIN 25
#define RX_BUFFER_SIZE 256

#define I2C_MASTER_SCL_IO 19
#define I2C_MASTER_SDA_IO 18
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

static i2c_master_bus_handle_t i2c_master_handle;
//===================================================================
//  APP MAIN
//===================================================================
void app_main(void)
{
    // xSemaphoreMutex = xSemaphoreCreateMutex();

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

    xTaskCreate(vTaskBME, "TaskBME", configMINIMAL_STACK_SIZE + 4096, NULL, 1, &xTaskBMEHandle);
    // xTaskCreate(vTaskUART_TX, "TaskUARTTX", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTaskUARTHandle);

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
    uint8_t len_Temperatura = sizeof(Temperatura);
    int txBytes = -1;

    while (1)
    {
        txBytes = uart_write_bytes(UART_NUM_1, &Temperatura, len_Temperatura);
        ESP_LOGI(TAG_UART, "Tx bytes: %i", txBytes);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void vTaskBME(void *pvParameter)
{
    esp_err_t xError = ESP_FAIL;

    uint8_t adc_T_regs[] = {BMP280_TEMP_MSB, BMP280_TEMP_LSB};
    int32_t adc_T = 0;
    uint8_t buffer[2];

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = 0x76,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ,
    };

    i2c_master_dev_handle_t dev_handle;
    i2c_master_bus_add_device(i2c_master_handle, &dev_cfg, &dev_handle);

    bmp280_t bmp280;

    bmp280_set_timeout(&bmp280, 1000);

    bmp280_set_standby(&bmp280, dev_handle, 0b101);
    bmp280_set_oversamplig(&bmp280, dev_handle, SKIP, x1);
    bmp280_set_mode(&bmp280, dev_handle, NORMAL);

    vTaskDelay(pdMS_TO_TICKS(50));

    bmp280_get_trimming_params_temp(&bmp280, dev_handle);

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(2000));

        if (!(bmp280_is_measuring(&bmp280, dev_handle)))
        {
            xError = i2c_master_transmit_receive(dev_handle, &adc_T_regs[0], 2, &buffer[0], 2, bmp280.Timeout);
            // ESP_LOGW(TAG_BME, "%s", esp_err_to_name(xError));

            adc_T = ((uint32_t)buffer[0] << 8) | ((uint32_t)buffer[1]);

            bmp280_get_temperature(&bmp280, adc_T);
            ESP_LOGI(TAG_BME, "Temperatura: %li", bmp280.Temperature);
        }
    }
}