#ifndef CLOCK_MQTT_H
#define CLOCK_MQTT_H

void clockMQTT_config(void);

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

#endif