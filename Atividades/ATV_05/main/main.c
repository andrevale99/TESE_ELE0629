#include <stdio.h>

// Includes FreeRTOS
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

// Includes ESP-IDF
#include <esp_system.h>
#include <esp_wifi.h>
#include <esp_log.h>
#include <esp_check.h>
#include <nvs_flash.h>
#include <mqtt_client.h>

#include "DS3231.h"
#include "Wifi.h"
#include "MQTT_Andre.h"

#define I2C_MASTER_SCL_IO 23
#define I2C_MASTER_SDA_IO 22
#define I2C_MASTER_FREQ_HZ 100000

//===================================================================
//  VARS & FUNCS
//===================================================================

void vTaskMQTT(void *pvParameter);
TaskHandle_t xTaskMQTT_handle = NULL;
const char *TAG_MQTT = "[MQTT_TASK]";
const char *Topic_DS3231 = "ANDRE/sensors/DS3231";

void vTaskDS3231(void *pvParameter);
TaskHandle_t xTaskDS3231_handle = NULL;
const char *TAG_DS3231 = "[DS3231_TASK]";
char pcDS3231data[10];

esp_mqtt_client_handle_t client = NULL;

SemaphoreHandle_t xSemaphore_Binario = NULL;

static i2c_master_bus_handle_t xI2CMaster_handle = NULL;
//===================================================================
//  APP MAIN
//===================================================================
void app_main(void)
{
    xSemaphore_Binario = xSemaphoreCreateBinary();

    ESP_LOGI("[I2C CONF]", "Configurando o I2C");

    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &xI2CMaster_handle));
    ESP_LOGI("[I2C CONF]", "Configuracao da I2C FINALIZADA");

    // Inicializando NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    wifi_init_sta();

    AndreMQTT_start(&client);

    xTaskCreate(vTaskDS3231, "TaskDS3231", configMINIMAL_STACK_SIZE + 1024, NULL, 0, &xTaskDS3231_handle);
    xTaskCreate(vTaskMQTT, "TaskMQTT", configMINIMAL_STACK_SIZE + 1024, NULL, 0, &xTaskMQTT_handle);
}

//===================================================================
//  FUNCS
//===================================================================

void vTaskMQTT(void *pvParameter)
{
    int iMsgID = 0;

    AndreMQTT_subscribe(&client, "ANDRE/sensors/DS3231");

    while (true)
    {
        if (xSemaphoreTake(xSemaphore_Binario, 100) == pdTRUE)
        {
            iMsgID = AndreMQTT_publish(&client, "ANDRE/sensors/DS3231", pcDS3231data, -1);
            ESP_LOGI(TAG_MQTT, "Mensagem ID: %i", iMsgID);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
    AndreMQTT_unsubscribe(&client, "ANDRE/sensors/DS3231");
}

void vTaskDS3231(void *pvParameter)
{
    ds3231_t sensor;

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = DS3231_ADDR,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ,
    };

    i2c_master_dev_handle_t dev_handle;
    ESP_ERROR_CHECK(i2c_master_bus_add_device(xI2CMaster_handle, &dev_cfg, &dev_handle));

    sensor.Timeout = 1000;

    while (true)
    {
        vDS3231_get_clock(dev_handle, &sensor);
    
        snprintf(pcDS3231data, sizeof(pcDS3231data), "%x:%x:%x ", sensor.hour, sensor.min, sensor.sec);

        xSemaphoreGive(xSemaphore_Binario);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}