#include <string.h>

//Includes FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"

//Includes ESP-IDF
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_mac.h"

//Include LwIP
#include "lwip/err.h"
#include "lwip/sys.h"

// Nossa Biblioteca
#include "wifi.h"

// TAG para info
static const char *TAG = "Biblioteca Wi-Fi";

//Função para tratamento dos eventos
static void ap_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data){

    // Verificar se o evento gerado é Wi-Fi e qual o motivo
    if(event_id == WIFI_EVENT_AP_STACONNECTED){
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "MAC do Station: "MACSTR" CONECTADO, AID = %d", MAC2STR(event->mac), event->aid);
    } else if(event_id == WIFI_EVENT_AP_STADISCONNECTED){
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "MAC do Station: "MACSTR" DESCONECTADO, AID = %d", MAC2STR(event->mac), event->aid);
    }
}

void wifi_init(void){
    
    // Init Phase - Wi-Fi Driver and LwIP
    ESP_ERROR_CHECK(esp_netif_init());                       // LwIP
    ESP_ERROR_CHECK(esp_event_loop_create_default());        // Event loop for event task   
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();     // Driver Wi-Fi
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Realizando o storage na RAM. Deixa a conexão um pouco mais rápida.
    // É uma outra forma de configurar, ao invés de modo persistente na NVS.
    // Obs: Perdendo a conexão é preciso realizar a configurações novamente (executa essa mesma rotina).
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
}

void wifi_init_ap(const char *ssid, const char *pass){
    
    // Função de configuração
    wifi_init();
    ESP_LOGI(TAG, "INICIALIZANDO O WI-FI!");
    esp_netif_create_default_wifi_ap();     // LwIP com configurações básicas para o modo AP.

    // Wi-Fi Configuration Phase
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &ap_event_handler, NULL, NULL));
    
    wifi_config_t wifi_config = {};
    strncpy((char *)wifi_config.ap.ssid, ssid, sizeof(wifi_config.ap.ssid) - 1);
    strncpy((char *)wifi_config.ap.password, pass, sizeof(wifi_config.ap.password) - 1);
    wifi_config.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;
    wifi_config.ap.max_connection = 5;
    wifi_config.ap.beacon_interval = 100;   // Deve ser múltiplo de 100. Faixa de 100 a 600000. Unit: TU(time unit, 1 TU = 1024 us).
    wifi_config.ap.channel = 1;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));

    // Wi-Fi start Phase
    esp_wifi_start();

    ESP_LOGI(TAG, "ESP32 COMO AP PRONTO! CONECTAR AO SSID: %s, PASSWORD: %s E CANAL: %d", 
            wifi_config.ap.ssid, wifi_config.ap.password, wifi_config.ap.channel);


}