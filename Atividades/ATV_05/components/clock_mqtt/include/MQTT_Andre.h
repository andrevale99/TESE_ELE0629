#ifndef MQTT_ANDRE_H
#define MQTT_ANDRE_H

void AndreMQTT_start(esp_mqtt_client_handle_t *client);
int AndreMQTT_subscribe(esp_mqtt_client_handle_t *client, char *topic);
int AndreMQTT_unsubscribe(esp_mqtt_client_handle_t *client, char *topic);
int AndreMQTT_publish(esp_mqtt_client_handle_t *client, char *topic, char *payload, int retain);

#endif