#include "mqttsub.h"
#include "config.h"
#include <stdio.h>
#include <unistd.h>
 
int  main() {
	
	struct mqtt_client_pub* pub  = mqtt_client_create_from_json("../test.txt");
    display_mqtt_client_config(pub);
	
	int status = mqtt_client_connect(pub);
	/*char* topic = NULL;
	bool* retain = NULL;
	int port;
	get_config_from_file("test.txt");
	get_json_string(&topic, "topic");
	get_json_int_number(&port, "conn_sett.port");
	get_json_bool
	ean(&retain, "retain");*/


	printf("%d \n", status);
	int i = 0;
	while (++i < 5) {
		status = mqtt_publish(pub, "test", 4);
		printf("%d \n", status);
		sleep(2);
	}
	printf("clean up started \n");
	mqtt_client_delete(pub);
	pub = NULL;
	printf("%d tesing is working \n");
}
