#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_now.h"
#include "esp_mac.h"

#include "driver/gpio.h"
#include "esp_log.h"

#define BT_PIN 4

const char *TAG = "Sender ESP-NOW";

uint8_t mac_receiver[6] = {0xD4, 0x8A, 0xFC, 0xA5, 0xC4, 0xF4};
//uint8_t mac_receiver[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

void espnow_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status){
    char msg_buffer[50];

    if(status == ESP_NOW_SEND_SUCCESS){
        sprintf(msg_buffer, "Mensagem enviada para: " MACSTR, MAC2STR(mac_addr));
        ESP_LOGI(TAG, "%s", msg_buffer);
    }else{
        sprintf(msg_buffer, "ERRO no envio da mensagem para: " MACSTR, MAC2STR(mac_addr));
        ESP_LOGI(TAG, "%s", msg_buffer); 
    }
}

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if(ret == ESP_ERR_NVS_NO_FREE_PAGES){
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    //Inicializando o Wi-Fi
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    //Inicialização do ESP-NOW
    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_send_cb(espnow_send_cb));   //Função de callback para tratar o evento de envio

    // Adicionando o dispositivo (peer) a receber msg
    esp_now_peer_info_t peer;
    memset(&peer, 0, sizeof(esp_now_peer_info_t));      // Zerando o espaço em memória de peer
    memcpy(peer.peer_addr, mac_receiver, 6);            // Copiando o mac do receiver para peer
    esp_now_add_peer(&peer);

    // Configurando o pino gerar alterar valor do led no receiver
    gpio_reset_pin(BT_PIN);
    gpio_set_direction(BT_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BT_PIN, GPIO_PULLUP_ONLY);

    uint8_t bt_status = 1;
    uint8_t led_status = 0;
    char send_msg_buffer[100];

    while(1){
        int new_status = gpio_get_level(BT_PIN);

        if (new_status != bt_status){
            bt_status = new_status;

            if(bt_status == 0){
                ESP_LOGI(TAG, "Botão pressionado!");
                led_status = ! led_status;
                sprintf(send_msg_buffer, "Led %s", led_status ? "ON" : "OFF");
                esp_now_send(mac_receiver, (uint8_t *)send_msg_buffer, strlen(send_msg_buffer));
            }else{
                ESP_LOGI(TAG, "Botão solto!");
            }
       }
       vTaskDelay(100/portTICK_PERIOD_MS);
    }

    // Finalizando o ESP-NOW
    ESP_ERROR_CHECK(esp_now_deinit());
    ESP_ERROR_CHECK(esp_wifi_stop());
}
