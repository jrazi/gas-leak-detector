#include "gateway.h"

char read_response_buffer[1000] = "";
char write_response_buffer[1000] = "";

int Send_Humidity_Status(int humidityPercentage) {
	char http_raw_request[128] = "";
	
	sprintf(http_raw_request, "GET /update?api_key=%s&field1=%d HTTP/1.1\r\nHost: %s\r\n\r\n", WRITE_API_KEY, humidityPercentage, API_DOMAIN);

  Send_HTTP_Request(http_raw_request, write_response_buffer);
	
	return 0;
}

int Get_Valve_Open_Cmd() {
	char body_buffer[500] = "";
	char http_raw_request[128] = "";
	int valve_cmd;
	
	sprintf(http_raw_request, "GET /channels/1408802/fields/2/last.json?api_key=%s HTTP/1.1\r\nHost: %s\r\n\r\n", READ_API_KEY, API_DOMAIN);

  Send_HTTP_Request(http_raw_request, read_response_buffer);
  Get_Response_Body(read_response_buffer, body_buffer);
  valve_cmd = Get_Valve_Open_Cmd_From_Json_Text(body_buffer);
	
	return valve_cmd;
}

