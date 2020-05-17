#include "../dep/parson.h"
#include "config.h"
#include "utils/fileaccess.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>



typedef struct Config{
	char* path;
	void* value;
	JSON_Value_Type value_type;
} Config;


static bool _set_path(Config* config, const char* path){
	if((config == NULL)||(path == NULL))
		return false;
	config->path = (char*)malloc(strlen(path));
	if (!config->path)
		return false; 
	if(!memcpy((void*)config->path, (const void*)path, strlen(path))){
		free(config->path);
		config->path = NULL;
		return false;
	}
	return true;
}

static bool _set_config_value_type(Config* config, JSON_Value* value){
	if ((config == NULL)||(value == NULL))
		return false;
	config->value_type = json_type(value);
	if ((config->value_type == JSONError)||(config->value_type == JSONNull))
		return false;
	return true;
}

static bool _set_config_value(Config*config, JSON_Value* value, JSON_Value_Type value_type){
	if ((config == NULL)||(value == NULL))
		return false;
	config->value = NULL;
	if (value_type == JSONArray)
		config->value = json_value_get_array(value);
	if (value_type == JSONObject)
		config->value = json_value_get_object(value);
	if (config->value == NULL)
		return false;
	return true;
	}

static bool _set_config(Config* config, const char* path){
	if ((config == NULL)||(path == NULL))
		return false;
	if(!_set_path(config, path))
		return false;
	JSON_Value* value = json_parse_file(path);
	if(!_set_config_value_type(config, value))
		return false;
	if(!_set_config_value(config, value, config->value_type))
		return false;
	return true;
}

void config_delete(Config* config){
	if(config != NULL)
		free(config->path);
	    free(config->value);
	free(config);
	config = NULL;
}

Config* get_config_from_file(const char* path) {
	if (is_readable(path) != FILE_READABLE)
		return NULL;
	Config* config = (Config*)malloc(sizeof(Config));
	if (config==NULL)
		return NULL;
	if(!_set_config(config, path)){
		config_delete(config);
		return NULL;
	}
    return config;
}

enum CONF_ERROR config_json_object_get_string(Config* config, char** value, const char* key) {
	if (config == NULL)
		return ERR_NULLCONFIG;
	if (config->value_type != JSONObject)
		return ERR_WRONGVALUETYPE;
	*value = json_object_dotget_string((JSON_Object*)config->value, key);
	if (!value)
		return ERR_KEYNOTEXIST;
    return ERR_SUCCESS;
}

 enum CONF_ERROR config_json_object_get_boolean(Config* config, int* value, const char* key) {
    if (config == NULL)
		return ERR_NULLCONFIG;
	if (config->value_type != JSONObject)
		return ERR_WRONGVALUETYPE;
	*value = json_object_dotget_boolean((JSON_Object*)config->value, key);
	if (*value == -1)
        return ERR_KEYNOTEXIST;
	return ERR_SUCCESS;
}
 
 enum CONF_ERROR config_json_object_get_number(Config* config, double* value, const char* key) {
	 if (config == NULL)
		return ERR_NULLCONFIG;
	 if (config->value_type != JSONObject)
		return ERR_WRONGVALUETYPE;
	 *value = json_object_dotget_number((JSON_Object*)config->value, key);
     if (*value == 0)
         return ERR_KEYNOTEXIST;
     return ERR_SUCCESS;
 }

 enum CONF_ERROR config_json_object_get_int_number(Config* config, int* value, const char* key) {
     if (config == NULL)
		 return ERR_NULLCONFIG;
	 if (config->value_type != JSONObject)
		 return ERR_WRONGVALUETYPE;
     *value = json_object_dotget_number((JSON_Object*)config->value, key);
     if (*value == 0)
		 return ERR_KEYNOTEXIST;
	 return ERR_SUCCESS;
 }

 



