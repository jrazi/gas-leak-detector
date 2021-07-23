
#include "stdutils.h"

#define RECORD_EXPIRE_MINUTES 10


typedef struct 
{
    uint32_t timestamp;
    uint8_t humidity_percentage;
} humidify_record;

typedef struct
{
    uint32_t timestamp;
    uint8_t valve_cmd;
} valve_cmd_record;

void Initialize_Store(void);

void Insert_Humidity_Record(humidify_record record);
void Insert_Valve_Cmd_Record(valve_cmd_record record);

void Get_Latest_Humidity_Records(int since_minutes_ago, humidify_record* buffer);
void Get_Latest_Valve_Cmd_Records(int since_minutes_ago, valve_cmd_record* buffer);
