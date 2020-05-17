#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

enum CONF_ERROR {
    ERR_UNPARSABLE,
    ERR_UNREADBLE,
    ERR_SUCCESS ,
    ERR_KEYNOTEXIST,
	ERR_WRONGVALUETYPE,
	ERR_NULLCONFIG
};

typedef struct Config Config;

Config* get_config_from_file(const char* path);

enum CONF_ERROR config_json_object_get_string(Config* config, char** value, const char* key); 

enum CONF_ERROR config_json_object_get_boolean(Config* config, int* value, const char* key);

enum CONF_ERROR config_json_object_get_number(Config* config, double* value, const char* key);

enum CONF_ERROR config_json_object_get_int_number(Config* config, int* value, const char* key); 

void config_delete(Config* config);

#endif /* CONFIG_H */
