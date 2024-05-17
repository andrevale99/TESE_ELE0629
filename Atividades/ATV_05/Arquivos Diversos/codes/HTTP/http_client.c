#include "esp_event.h"
#include "esp_log.h"
#include "esp_http_client.h"

//Nossa biblioteca
#include "http_client.h"

static const char *TAG = "HTTP CLIENT";

// Semaforo para que a tarefa inicia apenas apos o WiFi conectado (já declarado no wifi.c)
extern SemaphoreHandle_t wifi_semaphore;

void http_client_task(void *pvParameters){

    // Esperamos até uma conexão ser estabelecida
    xSemaphoreTake(wifi_semaphore, portMAX_DELAY);

    while(1){
        ESP_LOGI(TAG, "Fazendo uma requisição HTTP - http_client_request()!");
        http_client_request();
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
    
}

//http client handle
esp_err_t _http_event_handle(esp_http_client_event_t *evt)
{
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            if (!esp_http_client_is_chunked_response(evt->client)) {
                printf("%.*s", evt->data_len, (char*)evt->data);
            }
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            break;
        case HTTP_EVENT_REDIRECT:
            ESP_LOGD(TAG, "HTTP_EVENT_REDIRECT");
            break;
    }
    return ESP_OK;
}

void http_client_request()
{
    esp_http_client_config_t config = {
        .url = "http://api.openweathermap.org/data/2.5/weather?q=Natal,BR&APPID=c39826247b66d7102eb9c63081b57335",
        .event_handler = _http_event_handle,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Status = %i, content_length = %lli", esp_http_client_get_status_code(client), esp_http_client_get_content_length(client));
    }
    esp_http_client_cleanup(client);
}