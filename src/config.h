#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

enum CONF_ERROR {
    ERR_UNPARSABLE,
    ERR_UNREADBLE,
    ERR_SUCCESS ,
    ERR_KEYNOTEXIST

};


enum CONF_ERR get_config_from_file(const char* path);

enum CONF_ERR get_json_string(char**value, char* key);

enum CONF_ERROR get_json_boolean(int8_t* value, char* key);

enum CONF_ERROR get_json_number(double* value, char* key);

enum CONF_ERROR get_json_int_number(int* value, char* key);

void config_delete();

#endif /* CONFIG_H */
