
#include <stdio.h>
#include <string.h>

#define SERVER_ADDRESS "https://api.thingspeak.com"
#define WRITE_API_KEY "GC0BF7125UDLHVQ6"
#define READ_API_KEY "FSLIKU9TBV4ERAJJ"


int send_humidity_status(int humidityPercentage);
int get_valve_open_command(void);