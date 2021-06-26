#include "gateway.h"
#include "parser.h"

enum HttpMethod {GET, POST};

char* send_request(char* path, enum HttpMethod method);

int send_humidity_status(int humidityPercentage) {
	char path[50];
	snprintf(path, 50, "/update?api_key=%s&field1=%d", WRITE_API_KEY, humidityPercentage);
	
	char* response = send_request(path, GET); 
	if (!response) return -1;
	return 0;
}

int get_valve_open_command() {
	char path[50];
	snprintf(path, 50, "/channels/1408802/fields/2/last.json?api_key=%s", READ_API_KEY);
	
	char* response = send_request(path, GET); 
	if (!response) return -1;
	int cmd = get_valve_open_cmd_from_response_text(response);
	return cmd;
}


char* send_request(char* path, enum HttpMethod method) {
	char url[50];
	char* res;
	return res;
}

