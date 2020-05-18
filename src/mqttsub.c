#include <stdint.h>
#include <stdlib.h>
#include <mosquitto.h>
#include "config.h"
#include "mqttsub.h"
#include <stdio.h>

struct conn_config
{
	char* host;
	uint16_t port;
	uint16_t keepalive;
};

struct mqtt_client_pub
{
	struct mosquitto* mosq;
	char* topic;
	uint32_t mid;
	uint8_t qos;
	bool retain;
	struct conn_config* conn_sett;
};

int conn_config_set_from_json(Config* config_json, struct conn_config* conn_sett) {
	if((config_json==NULL) || (conn_sett==NULL))
		return -1;
	if (config_json_object_get_string(config_json, &(conn_sett->host), "conn_sett.host") != ERR_SUCCESS){
		conn_sett->host= NULL;
		return -1;
	}
		
	if (config_json_object_get_int_number(config_json, &(conn_sett->port), "conn_sett.port") != ERR_SUCCESS)
		return -1;
	if (config_json_object_get_int_number(config_json, &(conn_sett->keepalive), "conn_sett.keepalive") != ERR_SUCCESS)
		return -1;
	return 0;
}

void conn_config_destroy(struct conn_config* conn) {
	free(conn->host);
	free(conn);
}

void display_conn_config(struct conn_config* conn){
	if(conn != NULL){
		printf("Hostname: %s\n",conn->host);
		printf("Port: %d\n",conn->port);
		printf("Keepalive: %d\n",conn->keepalive);
	}
}

int mqtt_client_pub_set_from_json(Config* config_json, struct mqtt_client_pub* pub) {
	if (config_json_object_get_string(config_json, &(pub->topic), "topic") != ERR_SUCCESS){
		pub->topic=NULL;
		return -1;
	}	
	if (config_json_object_get_int_number(config_json, &(pub->mid), "mid") != ERR_SUCCESS)
		return -1;
	if (config_json_object_get_int_number(config_json, &(pub->qos), "qos") != ERR_SUCCESS)
		return -1;
	if (config_json_object_get_boolean(config_json, &(pub->retain), "retain") != ERR_SUCCESS)
		return -1;
	conn_config_set_from_json(config_json, pub->conn_sett);

}

int  mqtt_mosq_set(struct mqtt_client_pub* pub) {
	pub->mosq = mosquitto_new(NULL, true, NULL);
	if (!pub->mosq) return -1;
	return 0;
}

void display_mqtt_client_config(struct mqtt_client_pub* pub){
	if(pub != NULL){
		printf("Topic: %s\n",pub->topic);
		printf("MID: %d\n",pub->mid);
		printf("QoS: %d\n",pub->qos);
		printf("Retain: %d\n",pub->retain);
		display_conn_config(pub->conn_sett);
	}
}
struct mqtt_client_pub* mqtt_client_allocate() {

	struct mqtt_client_pub* pub = malloc(sizeof(struct mqtt_client_pub));
	if (pub == NULL) {
		return NULL;
	}
	pub->conn_sett = malloc(sizeof(struct conn_config));
	if ((pub->conn_sett) == NULL) {
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
	   ||((mqtt_mosq_set(pub) == -1))){
		mqtt_client_delete(pub);
		config_delete(config_json);
		return NULL;
	}
	config_delete(config_json);
	return pub;
}


enum mosq_err_t mqtt_client_connect(struct mqtt_client_pub* pub) {
	if (pub==NULL)
		return MOSQ_ERR_INVAL;
	
	enum mosq_err_t status = mosquitto_connect(pub->mosq,
											   pub->conn_sett->host,
											   pub->conn_sett->port,
											   pub->conn_sett->keepalive);

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
	//free(pub->topic);
	printf("topic deleted");
	mosquitto_destroy(pub->mosq);
	printf("mosq deleted");
	conn_config_destroy(pub->conn_sett);
	printf("conn sett deleted");
	//free(pub);
	printf("client deleted");
}



//to do 
// add callbacks 
// add publish overloads
//add function set mqtt pub from json and from values


