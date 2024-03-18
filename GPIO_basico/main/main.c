#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_check.h"
#include "esp_log.h"

#include "driver/gpio.h"
#include "hal/gpio_types.h"

#define BT_1 23
#define BT_2 22

#define LED_R 18
#define LED_G 17
#define LED_Y 16

static const char *LED_TAG = "LEDS";

void app_main() {
  
  esp_log_level_set("*", ESP_LOG_INFO);

  gpio_config_t ledsConfig =
  {
    .pin_bit_mask = (1<<LED_R) | (1<<LED_G) | (1<<LED_Y),
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_ENABLE,
    .intr_type = GPIO_INTR_DISABLE
  };

  ESP_ERROR_CHECK(gpio_config(&ledsConfig));

  gpio_config_t botoesConfig =
  {
    .pin_bit_mask = (1<<BT_1) | (1<<BT_2),
    .mode = GPIO_MODE_INPUT,
    .pull_up_en = GPIO_PULLUP_ENABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE
  };

  ESP_ERROR_CHECK(gpio_config(&botoesConfig));

  gpio_set_level(LED_R, 0);
  gpio_set_level(LED_G, 0);
  gpio_set_level(LED_Y, 0);

  uint8_t statusLeds = 0x00;

  while (true) 
  {
    ESP_LOGI(LED_TAG, "LED_R: %d", gpio_get_level(LED_R));
    ESP_LOGI(LED_TAG, "LED_R: %d", gpio_get_level(LED_G));

    if(!gpio_get_level(BT_1))
      gpio_set_level(LED_R, !gpio_get_level(LED_R));

    if(!gpio_get_level(BT_2))
      gpio_set_level(LED_G, !gpio_get_level(LED_G));

    gpio_set_level(LED_Y, !gpio_get_level(LED_Y));

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}