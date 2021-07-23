

#include <stdarg.h>
#include "wifi.h"
#include "stdutils.h"
#include "uart.h"
#include "delay.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>

char at_response_buffer[100] = "";
char at_line_buffer[600] = "";

wifi_status_st WifiStatus = {  NULL, NULL, 0, 0};

typedef struct
{ 
	int ok;
	char* text;
} at_response;

at_response response;

at_response Get_Command_Response(void);
at_response Get_HTTP_Response(char* response_buffer);


int Initialize_Wifi(void) {
	at_response response;
	
	UART2_Printf("WIFI INIT\r\n");

	UART1_TxString("AT\r\n");
	
	response = Get_Command_Response();
	

	if (response.ok <= 0) 
		return -1;
	

	UART1_TxString("AT+CWMODE=3\r\n");

	response = Get_Command_Response();

	if (response.ok <= 0) 
		return -1;	
	
	UART0_TxString("AT+CIPMUX=0\r\n");
	
	response = Get_Command_Response();

	if (response.ok <= 0) 
		return -1;

	UART1_TxString("AT+CWJAP=\"MPLab\",\"MpProject1400\"\r\n");
	
	response = Get_Command_Response();

	if (response.ok <= 0) 
		return -1;

  
	UART1_TxString("AT+CIFSR\r\n");
	
	response = Get_Command_Response();

	if (response.ok <= 0) 
		return -1;

	return 1;
}

int Start_TCP_CON() {
		UART1_TxString("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
		response = Get_Command_Response();
		return response.ok;
}

int Close_TCP_Con() {
	UART1_TxString("AT+CIPCLOSE\r\n");
	response = Get_Command_Response();
	return response.ok;
}

void Send_HTTP_Request(char* http_request, char* response_buffer) {
	char str[32];
	int handshake_result;
	at_response response;
	int packet_len = strlen(http_request);

	handshake_result = Start_TCP_CON();
	if (handshake_result <= 0)
		return;
	
  sprintf(str, "AT+CIPSEND=%d\r\n", packet_len);
	
	UART1_TxString(str);
	
	response = Get_Command_Response();

	if (response.ok <= 0) {
		Close_TCP_Con();
		return;
	}


	UART1_TxString(http_request);

	response = Get_HTTP_Response(response_buffer);


	Close_TCP_Con();
}

void Send_HTTP_Request_NonBlocking(atResponseHandlerFunPtr callback);
int Is_Ready_to_Send(void);
wifi_status_st Get_Status(void);





at_response Get_Command_Response(void) {
	char received;
	int done = 0;
	long counter = 0;
	int us_unit = 23;
	int timeout_ms = 8000;
	long counter_max = timeout_ms*20*us_unit;
	
	
	strcpy(at_response_buffer, "");
	response.text = "TIMEOUT";
	response.ok = -2;
	
//	UART2_Printf("HANDLING RESPONSE\r\n");

	while (!done && counter < counter_max) {
		DELAY_us(1);
	//	UART2_Printf("%d\r\n", counter);
		counter++;
		received = UART1_ReadRDR();
		if (received == NULL)
			continue;
	//	UART2_Printf("char: %c\r\n", received);
    strncat(at_response_buffer, &received, 1);
		if (EndsWith(at_response_buffer, "OK\r\n")) {
			response.ok = 1;
			response.text = at_response_buffer;
			done = 1;
		}
		else if (EndsWith(at_response_buffer, "ERROR\r\n")) {
			response.ok = 0;
			response.text = at_response_buffer;
			done = 1;
		}
	}
	
	
	UART2_Printf("****** WIFI RESPONSE ******\r\n%s\r\n*******************\r\n", response.text);
	return response;
}


int Read_HTTP_Response_Line(char* line_buffer) {
	char received;
	long counter = 0;
	int us_unit = 23;
	int timeout_ms = 8000;
	long counter_max = timeout_ms*20*us_unit;
	
	
	strcpy(line_buffer, "");

	while (counter < counter_max) {
		DELAY_us(1);
		counter++;
		received = UART1_ReadRDR();
		if (received == NULL)
			continue;
    strncat(line_buffer, &received, 1);
		if (EndsWith(line_buffer, "\r\n")) {
			return 1;
		}
	}
	return 0;
}

at_response Get_HTTP_Response(char* response_buffer) {
	int done = 0;
	int line_read = 1;
	int reading_body = 0;
	int junk_line = 1;
	
	strcpy(response_buffer, "");
	response.text = "TIMEOUT";
	response.ok = -2;
	

	while (!done && line_read > 0) {
		line_read = Read_HTTP_Response_Line(at_line_buffer);
	 
		if (junk_line && StartsWith(at_line_buffer, "+IPD")) {
			junk_line = 0;
		}

		if (junk_line) continue;
		
		strcat(response_buffer, at_line_buffer);
		
		if (EndsWith(at_line_buffer, "3F\r\n")) {
			reading_body = 1;
		}
		else if (reading_body == 1 && EndsWith(at_line_buffer, "0\r\n")) {
			response.ok = 1;
			response.text = response_buffer;
			done = 1;
		}
	}
	
	
	UART2_Printf("****** HTTP RESPONSE ******\r\n%s\r\n*******************\r\n", response.text);
	return response;
}


