
#include <jansson.h>
#include <string.h>

#define HUMIDITY_FIELD "field1"
#define VALVE_OPEN_CMD_FIELD "field2"


int get_valve_open_cmd_from_response_text(char *text);

json_t* to_json(char *text);
