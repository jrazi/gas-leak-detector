#ifndef PARSER_H
#define	PARSER_H

#include <string.h>
#include "utils.h"
#include "stdutils.h"

#define HUMIDITY_FIELD "field1"
#define VALVE_OPEN_CMD_FIELD "field2"


int Get_Valve_Open_Cmd_From_Json_Text(char *text);

void Get_Response_Body(char* raw_response, char* body_buffer);

#endif
