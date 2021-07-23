
#include "parser.h"
#include <stdlib.h>
#include "cJSON.h"

int Get_Valve_Open_Cmd_From_Json_Text(char *text) {
	char* valve_open_cmd_text;
	cJSON* monitor_json;
	cJSON* inner_object;

	monitor_json = cJSON_Parse(text);
	if (monitor_json == NULL) {
		return NULL;
	}
	inner_object = cJSON_GetObjectItemCaseSensitive(monitor_json, "field2");
	
	if (cJSON_IsString(inner_object) && (inner_object->valuestring != NULL)) {
		valve_open_cmd_text = inner_object->valuestring;
		return atoi(valve_open_cmd_text);
	}
	
	return NULL;
	
}
	
void Get_Response_Body(char* raw_response, char* body_buffer) {
	int i = 0;
	int response_length = strlen(raw_response);
	int start_index = 0;
	int end_index = response_length - 1;

	for (i = 0; i < response_length; i++) {
		if (start_index <= 0 && StartsWith(&raw_response[i] ,"3F\r\n")) {
			start_index = i + strlen("3F\r\n");
		}
		if ((end_index >= response_length - 1) && StartsWith(&raw_response[response_length - i], "}\r\n")) {
			end_index = response_length - i; 
		}
	}
	strncpy(body_buffer, &raw_response[start_index], end_index - start_index + 1);
  strncat(body_buffer, "\0", 1);

//	strncpy(body_buffer, "\0", 1);
}

/**
json_t* To_Json(char *text) {
	json_t *res;
	json_error_t  error;

	res = json_loads(text, 0, &error);

	return res;
}

int Get_Valve_Open_Cmd_From_Json_Text(char *text) {
	json_t* json_object;
	json_t* valve_open_cmd_object;
	const char* valve_open_cmd_text;
	
	json_object = To_Json(text);
	
	valve_open_cmd_object = json_object_get(json_object, VALVE_OPEN_CMD_FIELD);
	valve_open_cmd_text = json_string_value(valve_open_cmd_object);
	
	json_decref(valve_open_cmd_object);
	json_decref(json_object);

	return atoi(valve_open_cmd_text);
}
*/

