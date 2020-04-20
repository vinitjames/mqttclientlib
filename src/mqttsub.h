#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <mosquitto.h>

struct conn_config;

struct mqtt_client_pub;

struct mqtt_client_pub* mqtt_client_create(char* path);

enum mosq_err_t mqtt_client_connect(struct mqtt_client_pub* pub);

enum mosq_err_t mqtt_publish(struct mqtt_client_pub* pub, char* msg, size_t len);

void mqtt_client_delete(struct mqtt_client_pub* pub);