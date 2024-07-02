/**
 * @author André Menezes de Freitas Vale
 * @version 1.0.0
 *
 * @brief Codigo para leitura da Tensao de uma bateria a partir do
 * movimento do servo. Os dados da hora e tensao são enviados pela
 * UART pelo USB e por MQTT.
 *
 * @note O programa pode ser modificado para caber na lógica
 * de um sistema embarcado para uma aeromodelo.
 *
 * @note Acrescentar um botao para Parar/Retomar o Teste
 */

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

#include <esp_log.h>
#include <esp_err.h>

#include "configs.h"
//=====================================================
//  PROTOTIPOS E VARS
//=====================================================

/**
 * @brief Task para mostrar em um LCD
 *
 * @note Falta só o LCD e a logica
 */
static void vTaskLCD(void *pvParameters);
TaskHandle_t xTaskLCD_handle = NULL;
const char *TAG_LCD = "[LCD]";

/**
 * @brief Rotina principal para realizar os testes
 */
static void vTaskRotina(void *pvParameters);
TaskHandle_t xTaskRotina_handle = NULL;
const char *TAG_ROTINA = "[ROTINA]";
static int adc_raw = 0;
ds3231_t relogio;

/**
 * @brief Task para enviar os dados via SERIAL UART
 */
static void vTaskUART(void *pvParameters);
TaskHandle_t xTaskUART_handle = NULL;
const char *TAG_UART = "[UART]";
SemaphoreHandle_t xSemaphore_RotinaToUART_handle = NULL;
SemaphoreHandle_t xSemaphore_UARTToMQTT_handle = NULL;

// Buffer generico, pode ser utilizado em outras funcoes caso necessario
char buffer[MAX_BUFFER_SIZE];

/**
 * @brief Task para envio via MQTT
 */
static void vTaskMQTT(void *pvParameters);
TaskHandle_t xTaskMQTT_handle = NULL;
const char *TAG_MQTT = "[MQTT]";
//=====================================================
//  APP_MAIN
//=====================================================
void app_main(void)
{
    const char *TAG = "[APP_MAIN]";

    xSemaphore_RotinaToUART_handle = xSemaphoreCreateBinary();
    xSemaphore_UARTToMQTT_handle = xSemaphoreCreateBinary();

    ADC_config();
    MCPWM_config();
    MQTT_config();
    I2C_config();
    UART_config();

    xTaskCreate(vTaskLCD, "LCD task", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTaskLCD_handle);
    xTaskCreate(vTaskMQTT, "MQTT task", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTaskMQTT_handle);
    xTaskCreate(vTaskRotina, "Rotina task", configMINIMAL_STACK_SIZE + 1024, NULL, 3, &xTaskRotina_handle);
    xTaskCreate(vTaskUART, "UART task", configMINIMAL_STACK_SIZE + 1024, NULL, 2, &xTaskUART_handle);
}

//=====================================================
//  TASKS
//=====================================================

static void vTaskLCD(void *pvParameters)
{
    while (1)
    {
        // Compre um LCD I2C peste
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    vTaskDelete(xTaskLCD_handle);
}

static void vTaskRotina(void *pvParameters)
{
    i2c_device_config_t dev_cfg =
        {
            .dev_addr_length = I2C_ADDR_BIT_LEN_7,
            .device_address = DS3231_ADDR,
            .scl_speed_hz = I2C_MASTER_FREQ_HZ,
        };

    ESP_ERROR_CHECK(i2c_master_bus_add_device(xI2CMaster_handle, &dev_cfg, &(relogio.dev_handle)));

    relogio.Timeout = 500;

    int angle = 0;
    int step = 2;
    while (1)
    {
        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, example_angle_to_compare(angle)));
        if ((angle + step) > 60 || (angle + step) < -60)
        {
            step *= -1;
        }
        angle += step;

        ds3231_get_clock(relogio.dev_handle, &relogio);
        adc_oneshot_read(xADC1_handle, ADC_CHANNEL_0, &adc_raw);

        xSemaphoreGive(xSemaphore_RotinaToUART_handle);

        vTaskDelay(pdMS_TO_TICKS(100));
    }

    vTaskDelete(xTaskRotina_handle);
}

static void vTaskUART(void *pvParameters)
{
    int txBytes = -1;
    while (1)
    {
        if (xSemaphoreTake(xSemaphore_RotinaToUART_handle, 100) == pdTRUE)
        {
            snprintf(&buffer[0], 18, "255;%x:%x:%x;%d\n", relogio.hour, relogio.min, relogio.sec, adc_raw);
            txBytes = uart_write_bytes(UART_NUM_1, buffer, sizeof(buffer));

            if (txBytes == -1)
            {
                // ALgum Sinalizador de Error
            }
        }

        xSemaphoreGive(xSemaphore_UARTToMQTT_handle);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    vTaskDelete(xTaskUART_handle);
}

static void vTaskMQTT(void *pvParameters)
{
    char *Mqtt_Hora = "UFRN/LiuKang/Hora";
    char *Mqtt_Tensao = "UFRN/LiuKang/Tensao";

    mqtt_subscribe(Mqtt_Hora, 0);
    mqtt_subscribe(Mqtt_Tensao, 0);

    while (1)
    {
        if (xSemaphoreTake(xSemaphore_UARTToMQTT_handle, 100) == pdTRUE)
        {
            snprintf(&buffer[0], 10, "%x:%x:%x", relogio.hour, relogio.min, relogio.sec);
            mqtt_publish(Mqtt_Hora, &buffer[0], 0, -1);

            snprintf(&buffer[0], 4, "%d", adc_raw);
            mqtt_publish(Mqtt_Tensao, &buffer[0], 0, -1);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    vTaskDelete(xTaskMQTT_handle);
}

//=====================================================
//  FUNCOES
//=====================================================