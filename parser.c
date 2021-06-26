
#include "parser.h"


int get_valve_open_cmd_from_response_text(char *text) {
	json_t* json_object = to_json(text);
	
	json_t* valve_open_cmd_object = json_object_get(json_object, VALVE_OPEN_CMD_FIELD);
	const char* valve_open_cmd_text = json_string_value(valve_open_cmd_object);
	json_decref(valve_open_cmd_object);
	json_decref(json_object);

	return atoi(valve_open_cmd_text);
}
	
json_t* to_json(char *text) {
	json_t *res;
	json_error_t  error;

	res = json_loads(text, 0, &error);

	return res;
}
