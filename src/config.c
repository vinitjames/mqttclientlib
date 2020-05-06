#include "parson.h"
#include "config.h"


static JSON_Value* config_value = NULL;
static JSON_Object* config_object = NULL;

enum CONF_ERROR is_readable(const char* path) {
    
    if (_access(path, 0) == -1) {
        printf("config file does not exist or cannot be accesed");
        return ERR_FILENOTEXISTS;
    }

    if (_access(path, 4) == -1) {
        printf("config file does not have read permision");
        return ERR_UNREADBLE;
    }

    return ERR_SUCCESS;

}

 enum CONF_ERR get_config_from_file(const char* path) {
     enum CONF_ERR status = is_readable(path);
     if (status != ERR_SUCCESS)
         return status;
     config_value = json_parse_file(path);
     JSON_Array* arr = json_value_get_array(config_value);
     config_object = json_array_get_object(arr, 0);

     if (!config_object)
         return ERR_UNPARSABLE;
     return ERR_SUCCESS;
    
}

 enum CONF_ERR get_json_string(char** value, char* key) {
     if (!config_object)
         return -1;
     *value = json_object_dotget_string(config_object, key);
     if (!value)
         return ERR_KEYNOTEXIST;
     return ERR_SUCCESS;
}

 enum CONF_ERROR get_json_boolean(int8_t *value, char* key) {
     if (!config_object)
         return -1;
     *value = json_object_dotget_boolean(config_object, key);
     if (*value == -1)
        return ERR_KEYNOTEXIST;
     return ERR_SUCCESS;
}
 
 enum CONF_ERROR get_json_number(double* value, char* key) {
     if (!config_object)
         return -1;
     *value = json_object_dotget_number(config_object, key);
     if (*value == 0)
         return ERR_KEYNOTEXIST;
     return ERR_SUCCESS;
 }

 enum CONF_ERROR get_json_int_number(int* value, char* key) {
     if (!config_object)
         return -1;
     *value = (int)json_object_dotget_number(config_object, key);
     if (*value == 0)
         return ERR_KEYNOTEXIST;
     return ERR_SUCCESS;
 }

 void config_delete() {
     json_value_free(config_value);
     config_value = NULL;
     config_object = NULL;
 
 }
/* Add a function to recursivelz check for key in the current config object and remove static config object passing it to the user in get config function */


