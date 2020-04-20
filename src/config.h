#pragma once

#include <io.h>
#include <stdint.h>

enum CONF_ERROR {
    ERR_UNPARSABLE = -3,
    ERR_FILENOTEXISTS = -2,
    ERR_UNREADBLE = -1,
    ERR_SUCCESS = 0,
    ERR_KEYNOTEXIST = 1

};


enum CONF_ERR get_config_from_file(const char* path);

enum CONF_ERR get_json_string(char**value, char* key);

enum CONF_ERROR get_json_boolean(int8_t* value, char* key);

enum CONF_ERROR get_json_number(double* value, char* key);

enum CONF_ERROR get_json_int_number(int* value, char* key);

void config_delete();