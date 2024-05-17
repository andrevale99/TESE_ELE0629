#include <stdio.h>
#include <string.h>

//Includes FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//Includes ESP-IDF

#include "esp_log.h"
#include "nvs_flash.h"

//Biblioteca criada
#include "wifi.h"

static const char *TAG = "Exemplo Wi-Fi";

void app_main(void)
{
    // Inicializando NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    ESP_LOGI(TAG, "INICIALIZANDO ESP32 NO MODO AP!");
    wifi_init_ap("Rede ESP32", "zaqxswcde");

}
