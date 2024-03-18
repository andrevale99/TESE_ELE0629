#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_check.h"
#include "esp_log.h"

#include "hal/adc_types.h"
#include "esp_adc/adc_oneshot.h"
#include "driver/gpio.h"
#include "hal/gpio_types.h"
#include "driver/ledc.h"

#define LED GPIO_NUM_21

const char *TAG_DUTY_CICLE = "DUTY_CICLE";

void app_main() {
    
  esp_log_level_set("*", ESP_LOG_INFO);

  gpio_config_t ledConfig =
  {
    .pin_bit_mask = GPIO_NUM_21,
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_ENABLE,
    .intr_type = GPIO_INTR_DISABLE
  };

  ESP_ERROR_CHECK(gpio_config(&ledConfig));

  ledc_timer_config_t TimerConfig = 
  {
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .timer_num = LEDC_TIMER_0,
    .freq_hz = 10000,
    .duty_resolution = LEDC_TIMER_10_BIT,
    .clk_cfg = LEDC_AUTO_CLK
  };

  ESP_ERROR_CHECK(ledc_timer_config(&TimerConfig));

  ledc_channel_config_t ChannelConfig = 
  {
    .channel = LEDC_CHANNEL_0,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .timer_sel = LEDC_TIMER_0,
    .intr_type = LEDC_INTR_DISABLE,
    .gpio_num = GPIO_NUM_21,
    .duty = 1
  };

  ESP_ERROR_CHECK(ledc_channel_config(&ChannelConfig));

  adc_oneshot_unit_handle_t adc_1_handle;
  
  adc_oneshot_unit_init_cfg_t InitConfig = 
  {
    .unit_id = ADC_UNIT_1,
  };

  ESP_ERROR_CHECK(adc_oneshot_new_unit(&InitConfig, &adc_1_handle));

  adc_oneshot_chan_cfg_t AdcConfig =
  {
    .bitwidth = ADC_BITWIDTH_10,
    .atten = ADC_ATTEN_DB_12,
  };

  ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_1_handle, ADC_CHANNEL_0, &AdcConfig));

  int adc_value = 0;
  int duty_cicle = 0;

  while (true) 
  {
    ESP_ERROR_CHECK(adc_oneshot_read(adc_1_handle, ADC_CHANNEL_0, &adc_value));

    duty_cicle = adc_value;

    ESP_LOGI(TAG_DUTY_CICLE, "Duty Cicle: %d", duty_cicle);

    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty_cicle);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}