
#include "store.h"

#define HUMIDITY_READ_FREQUENCY_PER_MINUTE 60/2
#define VALVE_CMD_READ_FREQUENCY_PER_MINUTE 60/4

#define HUMIDITY_STORE_SIZE RECORD_EXPIRE_MINUTES/HUMIDITY_READ_FREQUENCY_PER_MINUTE
#define VALVE_CMD_STORE_SIZE RECORD_EXPIRE_MINUTES/VALVE_CMD_READ_FREQUENCY_PER_MINUTE


humidify_record humidify_records[HUMIDITY_STORE_SIZE];
valve_cmd_record valve_cmd_records[VALVE_CMD_STORE_SIZE];

typedef void (*queueFuncPtr)(void);

typedef struct 
{
    uint16_t starting_index;
		uint16_t last_index;
		uint16_t queue_size;
} queue_status;

queue_status humidity_queue_status;
queue_status valve_cmd_queue_status;

int Get_Index_To_Insert(queue_status* current_status) ;
void Update_Status_On_Insert(queue_status* current_status) ;
int Get_Queue_Item_Count(queue_status* current_status);

void Insert_Humidity_Record(humidify_record record) 
{
	int index;
	index = Get_Index_To_Insert(&humidity_queue_status);
	
	humidify_records[index] = record;
	
	Update_Status_On_Insert(&humidity_queue_status);
}


void Insert_Valve_Cmd_Record(valve_cmd_record record) 
{
	int index;
	index = Get_Index_To_Insert(&valve_cmd_queue_status);
	
	valve_cmd_records[index] = record;
	
	Update_Status_On_Insert(&valve_cmd_queue_status);
}

void Get_Latest_Humidity_Records(int since_minutes_ago, humidify_record* buffer)
{
	int num_of_records;
	size_t buffer_size; 
	int i;

	num_of_records = HUMIDITY_READ_FREQUENCY_PER_MINUTE * since_minutes_ago;
	buffer_size = sizeof(buffer)/sizeof(buffer[0]);

	for (i = 0; i < num_of_records && i < buffer_size && i < Get_Queue_Item_Count(&humidity_queue_status); i++) 
	{
		int index;
		index = (humidity_queue_status.starting_index + i) % humidity_queue_status.queue_size;
		
		buffer[i] = humidify_records[index];
	}
}

void Get_Latest_Valve_Cmd_Records(int since_minutes_ago, valve_cmd_record* buffer)
{
	int num_of_records;
	size_t buffer_size; 
	int i;

	num_of_records = VALVE_CMD_READ_FREQUENCY_PER_MINUTE * since_minutes_ago;
	buffer_size = sizeof(buffer)/sizeof(buffer[0]);

	for (i = 0; i < num_of_records && i < buffer_size && i < Get_Queue_Item_Count(&valve_cmd_queue_status); i++) 
	{
		int index;
		index = (valve_cmd_queue_status.starting_index + i) % valve_cmd_queue_status.queue_size;
		
		buffer[i] = valve_cmd_records[index];
	}
}



void Initialize_Store(void) 
{
		humidity_queue_status.starting_index = -1;
		humidity_queue_status.last_index = -1;
		humidity_queue_status.queue_size = HUMIDITY_STORE_SIZE;

		valve_cmd_queue_status.starting_index = -1;
		valve_cmd_queue_status.last_index = -1;
		valve_cmd_queue_status.queue_size = VALVE_CMD_STORE_SIZE;
}

int Get_Index_To_Insert(queue_status* current_status) 
{
	return (current_status->last_index + 1) % current_status->queue_size;	
}

void Update_Status_On_Insert(queue_status* current_status) 
{
	if (current_status->starting_index < 0 || current_status->last_index < 0)
	{
		current_status->starting_index = 0;
		current_status->last_index = 0;
	}
	else if (((current_status->last_index+1)%current_status->queue_size) <=  current_status->starting_index)
	{
		current_status->starting_index = (current_status->starting_index + 1) % current_status->queue_size;
		current_status->last_index = (current_status->last_index + 1) % current_status->queue_size;
	}
}


int Get_Queue_Item_Count(queue_status* current_status)
{
	if (current_status->last_index < 0 || current_status->starting_index < 0)
		return 0;
	
	if (current_status->last_index >= current_status->starting_index)
		return current_status->last_index - current_status->starting_index + 1;
	
	else if (current_status->last_index < current_status->starting_index)
		return current_status->queue_size - (current_status->starting_index - current_status->last_index - 1);
	
//		return (current_status->queue_size - current_status->starting_index) + (current_status->last_index + 1)
}

