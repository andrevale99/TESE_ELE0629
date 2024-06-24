#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_adc/adc_oneshot.h>
#include <hal/adc_types.h>
#include <driver/mcpwm_prelude.h>

#include <esp_log.h>
#include <esp_err.h>

#include <nvs_flash.h>

#include "MQTT_lib.h"
#include "WiFi.h"

#define I2C_MASTER_SCL_IO 23
#define I2C_MASTER_SDA_IO 22
#define I2C_MASTER_FREQ_HZ 100000

// Please consult the datasheet of your servo before changing the following parameters
#define SERVO_MIN_PULSEWIDTH_US 500  // Minimum pulse width in microsecond
#define SERVO_MAX_PULSEWIDTH_US 2500 // Maximum pulse width in microsecond
#define SERVO_MIN_DEGREE -90         // Minimum angle
#define SERVO_MAX_DEGREE 90          // Maximum angle

#define SERVO_PULSE_GPIO 0                   // GPIO connects to the PWM signal line
#define SERVO_TIMEBASE_RESOLUTION_HZ 1000000 // 1MHz, 1us per tick
#define SERVO_TIMEBASE_PERIOD 20000          // 20000 ticks, 20ms

//=====================================================
//  PROTOTIPOS E VARS
//=====================================================

void vTaskLCD(void *pvParameters);
TaskHandle_t xTaskLCD_handle = NULL;
const char *TAG_LCD = "[LCD]";

void vTaskADC(void *pvParameters);
TaskHandle_t xTaskADC_handle = NULL;
const char *TAG_ADC = "[ADC]";

void vTaskMQTT(void *pvParameters);
TaskHandle_t xTaskMQTT_handle = NULL;
const char *TAG_MQTT = "[MQTT]";

void ADC_config(void);
adc_oneshot_unit_handle_t xADC1_handle = NULL;

void MCPWM_config(void);
mcpwm_timer_handle_t timer = NULL;
mcpwm_oper_handle_t oper = NULL;
mcpwm_cmpr_handle_t comparator = NULL;
mcpwm_gen_handle_t generator = NULL;

void MQTT_config(void);
const char *topic_LastWill = "UFRN/LiuKang/Status";
const char *uri = "mqtt://mqtt.eclipseprojects.io";

static inline uint32_t example_angle_to_compare(int angle)
{
    return (angle - SERVO_MIN_DEGREE) * (SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US) / (SERVO_MAX_DEGREE - SERVO_MIN_DEGREE) + SERVO_MIN_PULSEWIDTH_US;
}

//=====================================================
//  APP_MAIN
//=====================================================
void app_main(void)
{
    const char *TAG = "[APP_MAIN]";

    ADC_config();
    MCPWM_config();
    MQTT_config();

    xTaskCreate(vTaskLCD, "LCD task", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTaskLCD_handle);
    xTaskCreate(vTaskADC, "ADC1 task", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTaskADC_handle);
    xTaskCreate(vTaskMQTT, "MQTT task", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTaskMQTT_handle);

    // int angle = 0;
    // int step = 2;
    // while (true)
    // {
    //     ESP_LOGI(TAG, "Angle of rotation: %d", angle);
    //     ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, example_angle_to_compare(angle)));
    //     // Add delay, since it takes time for servo to rotate, usually 200ms/60degree rotation under 5V power supply
    //     vTaskDelay(pdMS_TO_TICKS(500));
    //     if ((angle + step) > 60 || (angle + step) < -60)
    //     {
    //         step *= -1;
    //     }
    //     angle += step;
    // }
}

//=====================================================
//  FUNCOES
//=====================================================

void vTaskLCD(void *pvParameters)
{
    while (1)
    {

        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    vTaskDelete(xTaskLCD_handle);
}

void vTaskADC(void *pvParameters)
{
    int adc_raw = 0;
    while (1)
    {
        adc_oneshot_read(xADC1_handle, ADC_CHANNEL_0, &adc_raw);
        ESP_LOGI(TAG_ADC, "%d", adc_raw);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    vTaskDelete(xTaskADC_handle);
}

void vTaskMQTT(void *pvParameters)
{
    mqtt_subscribe("UFRN/LiuKang/Teste", 0);

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    vTaskDelete(xTaskMQTT_handle);
}

void ADC_config(void)
{
    adc_oneshot_unit_init_cfg_t ADC1_config =
        {
            .unit_id = ADC_UNIT_1};
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&ADC1_config, &xADC1_handle));

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten = ADC_ATTEN_DB_12,
    };

    ESP_ERROR_CHECK(adc_oneshot_config_channel(xADC1_handle, ADC_CHANNEL_0, &config));
}

void MCPWM_config(void)
{
    const char *TAG = "[MCPWM_CONFIG]";

    ESP_LOGI(TAG, "Create timer and operator");
    // mcpwm_timer_handle_t timer = NULL;
    mcpwm_timer_config_t timer_config = {
        .group_id = 0,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = SERVO_TIMEBASE_RESOLUTION_HZ,
        .period_ticks = SERVO_TIMEBASE_PERIOD,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    };
    ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config, &timer));

    // mcpwm_oper_handle_t oper = NULL;
    mcpwm_operator_config_t operator_config = {
        .group_id = 0, // operator must be in the same group to the timer
    };
    ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config, &oper));

    ESP_LOGI(TAG, "Connect timer and operator");
    ESP_ERROR_CHECK(mcpwm_operator_connect_timer(oper, timer));

    ESP_LOGI(TAG, "Create comparator and generator from the operator");
    // mcpwm_cmpr_handle_t comparator = NULL;
    mcpwm_comparator_config_t comparator_config = {
        .flags.update_cmp_on_tez = true,
    };
    ESP_ERROR_CHECK(mcpwm_new_comparator(oper, &comparator_config, &comparator));

    // mcpwm_gen_handle_t generator = NULL;
    mcpwm_generator_config_t generator_config = {
        .gen_gpio_num = SERVO_PULSE_GPIO,
    };
    ESP_ERROR_CHECK(mcpwm_new_generator(oper, &generator_config, &generator));

    // set the initial compare value, so that the servo will spin to the center position
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, example_angle_to_compare(0)));

    ESP_LOGI(TAG, "Set generator action on timer and compare event");
    // go high on counter empty
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(generator,
                                                              MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
    // go low on compare threshold
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(generator,
                                                                MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator, MCPWM_GEN_ACTION_LOW)));

    ESP_LOGI(TAG, "Enable and start timer");
    ESP_ERROR_CHECK(mcpwm_timer_enable(timer));
    ESP_ERROR_CHECK(mcpwm_timer_start_stop(timer, MCPWM_TIMER_START_NO_STOP));
}

void MQTT_config(void)
{
    // Inicializando NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    wifi_init_sta();

    mqtt_start(uri, topic_LastWill); // Iniciando conexão MQTT. Função da Lib criada, MQTT.h
}