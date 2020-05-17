#include <stdint.h>
#include <stdlib.h>
#include <mosquitto.h>
#include "config.h"
#include "mqttsub.h"

struct conn_config
{
	char* host;
	uint16_t port;
	uint16_t keepalive;
};

struct mqtt_client_pub
{
	struct mosquitto* mosq;
	const char* topic;
	uint32_t mid;
	uint8_t qos;
	bool retain;
	struct conn_config* conn_set;
};

int conn_config_set_from_json(Config* config_json, struct conn_config* conn_sett) {
	if((config_json==NULL) || (conn_sett==NULL))
		return -1;
	if (config_json_object_get_string(config_json, &(conn_sett->host), "conn_sett.host") != ERR_SUCCESS)
		return -1;
	if (config_json_object_get_int_number(config_json, &(conn_sett->port), "conn_sett.port") != ERR_SUCCESS)
		return -1;
	if (config_json_object_get_int_number(config_json, &(conn_sett->keepalive), "conn_sett.keepalive") != ERR_SUCCESS)
		return -1;
	return 0;
}

void conn_config_destroy(struct conn_config* conn) {
	conn->host = NULL;
	free(conn);
}

int mqtt_client_pub_set_from_json(Config* config_json, struct mqtt_client_pub* pub) {
	if (config_json_object_get_string(config_json, &(pub->topic), "topic") != ERR_SUCCESS)
		return -1;
	if (config_json_object_get_int_number(config_json, &(pub->mid), "mid") != ERR_SUCCESS)
		return -1;
	if (config_json_object_get_int_number(config_json, &(pub->qos), "qos") != ERR_SUCCESS)
		return -1;
	if (config_json_object_get_boolean(config_json, &(pub->retain), "retain") != ERR_SUCCESS)
		return -1;
	conn_config_set_from_json(config_json, pub->conn_set);

}

enum mosq_err_t mqtt_mosq_set(struct mqtt_client_pub* pub) {
	pub->mosq = mosquitto_new(NULL, true, NULL);
	if (!pub->mosq) return MOSQ_ERR_NOMEM;
	return MOSQ_ERR_SUCCESS;
}

struct mqtt_client_pub* mqtt_client_allocate() {

	struct mqtt_client_pub* pub = malloc(sizeof(struct mqtt_client_pub));
	if (pub == NULL) {
		return NULL;
	}
	pub->conn_set = malloc(sizeof(struct conn_config));
	if ((pub->conn_set) == NULL) {
		free(pub);
		return NULL;
	}
	return pub;
}

struct mqtt_client_pub* mqtt_client_create_from_json(const char* path) {
	mosquitto_lib_init();
	Config* config_json = get_config_from_file(path);
	if(config_json == NULL)
		return NULL;
	struct mqtt_client_pub* pub = mqtt_client_allocate();
	
	if (pub == NULL) {
		config_delete(config_json);
		return NULL;
	}
	
	if((mqtt_client_pub_set_from_json(config_json, pub) == -1)
	   ||((mqtt_mosq_set(pub)==-1))){
		mqtt_client_delete(pub);
		config_delete(config_json);
		return NULL;
	}
	config_delete(config_json);
	return pub;
}


enum mosq_err_t mqtt_client_connect(struct mqtt_client_pub* pub) {
	enum mosq_err_t status = mosquitto_connect(pub->mosq,
		pub->conn_set->host,
		pub->conn_set->port,
		pub->conn_set->keepalive);

	if (status == MOSQ_ERR_SUCCESS) {
		return mosquitto_loop_start(pub->mosq);
	}
	return status;

}

enum mosq_err_t mqtt_publish(struct mqtt_client_pub* pub, char* msg, size_t len) {
	return mosquitto_publish(pub->mosq, &(pub->mid), pub->topic, len, msg,
		pub->qos, pub->retain);
}


void mqtt_client_delete(struct mqtt_client_pub* pub) {
	pub->topic = NULL;
	pub->mid = NULL;
	mosquitto_destroy(pub->mosq);
	conn_config_destroy(pub->conn_set);
	pub->conn_set = NULL;
	free(pub);
}



//to do 
// add callbacks 
// add publish overloads
//add function set mqtt pub from json and from values


