#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_log.h>

//=====================================================
//  PROTOTIPOS E VARS
//=====================================================

void vTaskSD(void *pvParameter);
const char *TAG_SD = "[SD]";

void vTaskDS3231(void *pvParameter);
const char *TAG_DS3231 = "[DS3231]";

//=====================================================
//  APP_MAIN
//=====================================================
void app_main(void)
{
    const char *TAG = "[APP_MAIN]";

    while(1)
    {
        vTaskDelay ( pdMS_TO_TICKS(1000) );
    }
}

//=====================================================
//  FUNCOES
//=====================================================
