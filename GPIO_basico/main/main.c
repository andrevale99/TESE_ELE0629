#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_check.h"
#include "esp_log.h"

#include "driver/gpio.h"

#define BT_1 GPIO_NUM_23
#define BT_2 GPIO_NUM_22

#define LED_R GPIO_NUM_18
#define LED_G GPIO_NUM_17
#define LED_Y GPIO_NUM_16

static const char *LED_TAG = "LEDS";

void app_main() {
  
  esp_log_level_set("*", ESP_LOG_INFO);

  gpio_config_t ledsConfig =
  {
    .pin_bit_mask = (1<<LED_R) | (1<<LED_G) | (1<<LED_Y),
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
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

  int status_led = 0;

  while (true) 
  {
    status_led = gpio_get_level(LED_R);  
    ESP_LOGI(LED_TAG, "LED_R: %d", status_led);
    status_led = gpio_get_level(LED_G);
    ESP_LOGI(LED_TAG, "LED_G: %d", status_led);
    status_led = gpio_get_level(LED_Y);
    ESP_LOGI(LED_TAG, "LED_Y: %d \n", status_led);

    if(!gpio_get_level(BT_1))
      gpio_set_level(LED_R, !gpio_get_level(LED_R));

    if(!gpio_get_level(BT_2))
      gpio_set_level(LED_G, !gpio_get_level(LED_G));

    gpio_set_level(LED_Y, !gpio_get_level(LED_Y));

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}