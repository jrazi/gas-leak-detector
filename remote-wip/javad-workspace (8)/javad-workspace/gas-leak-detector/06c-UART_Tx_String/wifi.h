
typedef void (*atResponseHandlerFunPtr)(void);

typedef struct
{ 
	atResponseHandlerFunPtr default_Handler;
	atResponseHandlerFunPtr IPD_Handler;
	int AP_Connected;
	int TCPCON_Open;
} wifi_status_st;


int Initialize_Wifi(void);
void Send_HTTP_Request_NonBlocking(atResponseHandlerFunPtr callback);
void Send_HTTP_Request(char* http_request, char* response_buffer);
int Is_Ready_to_Send(void);
wifi_status_st Get_Status(void);

