
#include <stdio.h>
#include <string.h>
#include "wifi.h"
#include "parser.h"

#define API_DOMAIN "api.thingspeak.com"
#define WRITE_API_KEY "GC0BF7125UDLHVQ6"
#define READ_API_KEY "FSLIKU9TBV4ERAJJ"


int Send_Humidity_Status(int humidityPercentage);
int Get_Valve_Open_Cmd(void);


