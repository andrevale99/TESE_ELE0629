#include <stdio.h>
#include "esp_mac.h"
#include "esp_log.h"


const char *TAG = "Endereço MAC ESP";

void app_main(void)
{
    uint8_t mac_addr[6];
    
    esp_efuse_mac_get_default(mac_addr);

    ESP_LOGI(TAG, "Endereço MAC: %02x:%02x:%02x:%02x:%02x:%02x", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);

}
