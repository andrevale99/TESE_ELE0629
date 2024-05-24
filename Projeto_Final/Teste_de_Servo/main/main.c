#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <driver/i2c_master.h>

#include <esp_log.h>

#include "ds3231.h"
#include "servo_teste.h"

#define I2C_MASTER_SCL_IO 23
#define I2C_MASTER_SDA_IO 22
#define I2C_MASTER_FREQ_HZ 100000

//=====================================================
//  PROTOTIPOS E VARS
//=====================================================

void vTask_SD(void *pvParameter);
const char *TAG_SD = "[SD]";
TaskHandle_t xTaskDS3231_handle;

void vTask_DS3231(void *pvParameter);
const char *TAG_DS3231 = "[DS3231]";

static void I2C_setup();

static i2c_master_bus_handle_t i2c_master_handle;

//=====================================================
//  APP_MAIN
//=====================================================
void app_main(void)
{
    const char *TAG = "[APP_MAIN]";

    I2C_setup();

    xTaskCreate(vTask_DS3231, "Task_DS3231", configMINIMAL_STACK_SIZE + 1024, NULL, 0, &xTaskDS3231_handle);

    // while (true)
    // {
        // vTaskDelay(pdMS_TO_TICKS(10));
    // }
}

//=====================================================
//  FUNCOES
//=====================================================

//  @brief Task Para escrita do SD
void vTask_SD(void *pvParameter)
{
}

//  @brief Task Para Leitura do Relogio
void vTask_DS3231(void *pvParameter)
{
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = DS3231_ADDR,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ,
    };

    i2c_master_dev_handle_t dev_handle;
    i2c_master_bus_add_device(i2c_master_handle, &dev_cfg, &dev_handle);

    if (i2c_master_probe(i2c_master_handle, DS3231_ADDR, -1) != ESP_OK)
    {
        ESP_LOGE(TAG_DS3231, "DS3231 nao encontrado, suspendendo a TASK");
        vTaskSuspend(xTaskDS3231_handle);
    }

    ds3231_t sensor;

    while (true)
    {

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// @brief Configuracao do I2C
static void I2C_setup()
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