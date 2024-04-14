#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/gpio.h"
#include "esp_log.h"

#define LED1_PIN 21
#define LED2_PIN 19
#define LED3_PIN 2
#define BUTTON1_PIN 23
#define BUTTON2_PIN 22

static const char *TAG = "BUTTON_TEST";

QueueHandle_t gpio_evt_queue = NULL;

void buttonTask(void *pvParameters);

static void IRAM_ATTR gpio_isr_handler(void *arg){
    uint32_t gpio_num = (uint32_t) arg;
    BaseType_t xHigherPriorityTaskWoken = pdTRUE;   //Para "acordar" tarefa de maior prioridade

    // Queremos que a tarefa que trata o botao seja executada
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, &xHigherPriorityTaskWoken);
}


void app_main(void)
{
    gpio_config_t io_config = {};
        
    //Configura todos os pinos que queremos como entrada (nesse caso) por uma máscara de bits
    io_config.pin_bit_mask = (1ULL<<LED1_PIN)|(1ULL<<LED2_PIN)|(1ULL<<LED3_PIN);
    io_config.mode = GPIO_MODE_OUTPUT;
    io_config.pull_up_en = GPIO_PULLUP_DISABLE;
    io_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_config.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_config);

    //Colocando o estado inicial dos leds para apagado
    gpio_set_level(LED1_PIN, 0);
    gpio_set_level(LED2_PIN, 0);
    gpio_set_level(LED3_PIN, 0);

    //Configura todos os pinos que queremos como entrada (nesse caso) por uma máscara de bits
    io_config.pin_bit_mask = (1ULL<<BUTTON1_PIN)|(1ULL<<BUTTON2_PIN);
    io_config.mode = GPIO_MODE_INPUT;
    io_config.pull_up_en = GPIO_PULLUP_ENABLE;
    io_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_config.intr_type = GPIO_INTR_NEGEDGE;
    gpio_config(&io_config);
    
    //Instalando o serviço de interrupção
    gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);

    //Definir o handle de quem vai tratar a interrupção. Habilitar o tratamento da tarefa por meio da fila
    gpio_isr_handler_add(BUTTON1_PIN, gpio_isr_handler, (void*)BUTTON1_PIN);
    gpio_isr_handler_add(BUTTON2_PIN, gpio_isr_handler, (void*)BUTTON2_PIN);

    gpio_evt_queue = xQueueCreate(2, sizeof(uint32_t));

    xTaskCreate(buttonTask, "buttonTask", 2048, NULL, 2, NULL);

    while (1)
    {
        gpio_set_level(LED3_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_set_level(LED3_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}


void buttonTask(void *pvParameters){
    uint32_t gpio_num;
    uint8_t led1 = 0, led2 = 0;
    TickType_t last_button_press = 0;

    while (1)
    {
        xQueueReceive(gpio_evt_queue, &gpio_num, portMAX_DELAY);
        ESP_LOGI(TAG, "GPIO[%li] intr \n", gpio_num);

        TickType_t current_time = xTaskGetTickCount();
        
        if (current_time - last_button_press >= pdMS_TO_TICKS(250)){
            last_button_press = current_time;
            if(gpio_num == BUTTON1_PIN){
                gpio_set_level(LED1_PIN, led1 ^= 1);
            }
            else if(gpio_num == BUTTON2_PIN){
                gpio_set_level(LED2_PIN, led2 ^= 1);
            }
        }
    }
    
}
