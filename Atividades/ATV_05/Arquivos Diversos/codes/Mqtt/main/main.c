#include <stdio.h>
#include <string.h>

//Includes FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//Includes ESP-IDF
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_random.h"

//Biblioteca criada
#include "wifi.h"
#include "MQTT_lib.h"

static const char *TAG = "Exemplo MQTT Main";

void app_main(void)
{
    // Inicializando NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    wifi_init_sta();    //Inicializando o WiFi. Função da Lib criada, wifi.h
    ESP_LOGI(TAG, "WiFi foi inicializado!");

    mqtt_start();       //Iniciando conexão MQTT. Função da Lib criada, MQTT.h
    ESP_LOGI(TAG, "MQTT foi inicializado!");

    //Inscrevendo nos tópicos
    mqtt_subscribe("UFRN/Lab/Temp", 0);
    mqtt_subscribe("UFRN/Lab/Umidade", 0);

//    while(1){
//        int temp = esp_random() % 30;
//        int umidade = esp_random() % 50;
//        char temp_str[10], umidade_str[10];
//        sprintf(temp_str, "%d", temp);
//        sprintf(umidade_str, "%d", umidade);
//
//        if(mqtt_connected()){
//            mqtt_publish("UFRN/Lab/Temp", temp_str, 0, 0);
//            ESP_LOGI(TAG, "Tempetatura: %d", temp);
//            mqtt_publish("UFRN/Lab/Umidade", umidade_str, 0, 0);
//            ESP_LOGI(TAG, "Umidade: %d", umidade);
//        }
//
//
//        vTaskDelay(5000/portTICK_PERIOD_MS);
//    }
}
