#include "mqttpub.h"
#include "config.h"
#include <Windows.h>
#include <stdio.h>

int  main() {
	
	struct mqtt_client_pub* pub  = mqtt_client_create("test.txt");

	int status = mqtt_client_connect(pub);
	/*char* topic = NULL;
	bool* retain = NULL;
	int port;
	get_config_from_file("test.txt");
	get_json_string(&topic, "topic");
	get_json_int_number(&port, "conn_sett.port");
	get_json_bool
	ean(&retain, "retain");*/


	//printf("%s \n", pub->topic);
	int i = 0;
	while (++i < 100) {
		status = mqtt_publish(pub, "test", 4);
		printf("%d \n", status);
		Sleep(2000);
	}
	mqtt_client_delete(pub);
	pub = NULL;
	printf("%d tesing is working \n", 0 );
}