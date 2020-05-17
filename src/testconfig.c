
#include "config.h"

#include <stdio.h>
#include <stdbool.h>
int  main() {
	
	
	char* topic = NULL;
	int retain ;
	int port;
	Config* config = get_config_from_file("../test.txt");
	if(config_json_object_get_string(config, &topic, "topic")==ERR_SUCCESS)
		printf("%s\n", topic );
	if(config_json_object_get_int_number(config, &port, "conn_sett.port")==ERR_SUCCESS)
		printf("%d\n", port );
	if(config_json_object_get_boolean(config, &retain, "retain")==ERR_SUCCESS)
		printf("%d\n", retain );
    config_delete(config);
	printf("%s\n", "config deleted");
	printf("%s\n", topic );
	printf("%d\n", port );
	printf("%d\n", retain );
}
