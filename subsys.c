#include "subsystem.h"
#include <string.h>

#define PWR_BIT 0  
#define DATA_BIT 1  
#define ACT_BIT 2  
#define ERR_BIT 3  
#define PERF_BIT 4 
#define RES_BIT 5


/*
    This file should contain the implementations for Subsystem specific functionality.
    Initialization, printing, setting and getting data and statuses.
*/

/*
Task: subsys_init [10 minutes to 20]
Initalizes a subsytem with name
   Parameters:
	- subsystem : Pointer to the Subsystem struct to initialize (output)
	- name : The name of the subsystem (input)
	- status : The initial status byte for the subsystem (input)
   Returns:
	- ERR_NULL_POINTER if subsystem or name is NULL.
	- ERR_SUCCESS on success.	
   
*/


int subsys_init(Subsystem *subsystem, const char *name, char status){
	if (subsystem == NULL || name == NULL) {
        	return ERR_NULL_POINTER;  
    	}

   	strncpy(subsystem->name, name, MAX_STR - 1);
   	subsystem->name[MAX_STR - 1] = '\0'; 
   	
	subsystem->status = (unsigned char)status;
	subsystem->data = 0;
	
	return ERR_SUCCESS;
}

/*
Task: subsys_find [10 minutes to 20]
Search subsytsem using name, in an collection of subsystems
   Parameters:
	- subsystems : Collection of subsystems to search  (input)
	- name : The name of the subsystem to search for (input)
     
   Returns:
	- Index of the subsystem if found.
	- ERR_SYS_NOT_FOUND if subsystem is not in the collection.
   
*/


int subsys_find(const SubsystemCollection *subsystems, const char *name){
	//loop through collections to find name == name    	
    	for (unsigned int i = 0; i < subsystems->size; i++) {
        	if (strcmp(subsystems->subsystems[i].name, name) == 0) {
            		return i;  
        	}
    	}
	return ERR_SYS_NOT_FOUND;
}


/*
Task: subsys_print [15 minutes to 25]
Prints to terminal 
    TO DO:
	- if its NULL then return error code
	- extract status bits 
	- print so it is like [Name: <subsystem_name>, Status:(PWR: <pwr>|DATA: <data>|ACT: <act>|ERR: <err>|PERF: <perf>|RES: <res>)]
    Parameters:
	- subsystem : The subsystem to print (input)
    Returns:
	- ERR_NULL_POINTER if subsystem is NULL
	- ERR_SUCCESS if success	
	
*/


int subsys_print(Subsystem *subsystem) {
	if (subsystem == NULL) {
        return ERR_NULL_POINTER; 
    	}
	//extract for indivual bits
	unsigned int status = subsystem->status;
	unsigned int pwr = (status >> PWR_BIT) & 1;
	unsigned int data = (status >> DATA_BIT) & 1;
	unsigned int act = (status >> ACT_BIT) & 1;
	unsigned int err = (status >> ERR_BIT) & 1;
	unsigned int perf = (status >> PERF_BIT) & 1;
	unsigned int res = (status >> RES_BIT) & 1;


	printf("[Name: %16s, Status:(PWR: %u|DATA: %u|ACT: %u|ERR: %u|PERF: %u|RES: %u)]\n",
	   subsystem->name, pwr, data, act, err, perf, res);

	return ERR_SUCCESS;
}

/*
Task: subsys_status_set []
Sets a status bit in a subsystem's status 
    TO DO:
	- validate the status and value 
	- update the status bits by use switch
	-return sucess
    Parameters:
	- subsystem : The subsystem to update (input/output).
	- status : The status bit to modify (input).
	- value : The new value for the specified status bit (input).
	
    Returns:
	- ERR_NULL_POINTER if subsystem is NULL
	- ERR_SUCCESS if success
	- ERR_INVALID_STATUS if status/value is not valid.
	
	
*/



int subsys_status_set(Subsystem *subsystem, unsigned char status, unsigned char value){
	if (subsystem == NULL) {
        return ERR_NULL_POINTER;
   	}
		//validate status and value
		switch (status) {
			case STATUS_POWER:
			case STATUS_DATA:
			case STATUS_ACTIVITY:
			case STATUS_ERROR:
			    if (value > 1) return ERR_INVALID_STATUS;
			    break;
			case STATUS_PERFORMANCE:
			case STATUS_RESOURCE:
			    if (value > 3) return ERR_INVALID_STATUS;
			    break;
			default:
			    return ERR_INVALID_STATUS;
		    	}
		//set/celar status bits
		switch (status) {
			case STATUS_POWER:
			    subsystem->status &= ~(1 << 7);
			    subsystem->status |= (value & 0x01) << 7;
			    break;

			case STATUS_DATA:
			    subsystem->status &= ~(1 << 6);
			    subsystem->status |= (value & 0x01) << 6;
			    break;

			case STATUS_ACTIVITY:
			    subsystem->status &= ~(1 << 5);
			    subsystem->status |= (value & 0x01) << 5;
			    break;

			case STATUS_ERROR:
			    subsystem->status &= ~(1 << 4);
			    subsystem->status |= (value & 0x01) << 4;
			    break;

			case STATUS_PERFORMANCE:
			    subsystem->status &= ~(0x03 << 2);
			    subsystem->status |= (value & 0x03) << 2;
			    break;

			case STATUS_RESOURCE:
			    subsystem->status &= ~(0x03);
			    subsystem->status |= (value & 0x03);
			    break;

			default:
			    return ERR_INVALID_STATUS;
		   	}

    return ERR_SUCCESS;

}

/*
Task: subsys_status_print [20 minutes to 25]
Prints to terminal 
    Parameters:
	- subsystem : The subsystem status to print (input/ouput)
	
    Returns:
	- ERR_NULL_POINTER if subsystem is NULL
	- ERR_SUCCESS if success	
	
*/


int subsys_status_print(const Subsystem *subsystem){
	 if (subsystem == NULL) {
		return ERR_NULL_POINTER;
	    }


	 printf("Power: %u, ", (subsystem->status >> 7) & 0x01);        
	 printf("Data: %u, ", (subsystem->status >> 6) & 0x01);          
	 printf("Activity: %u, ", (subsystem->status >> 5) & 0x01);      
	 printf("Error: %u, ", (subsystem->status >> 4) & 0x01);         
	 printf("Performance: %u, ", (subsystem->status >> 2) & 0x03);   
	 printf("Resource: %u", subsystem->status & 0x03);             

	 return ERR_SUCCESS;
}

/*
Task: subsys_data_get [20 minutes to 25]
Sets new data and returning the old data 
    Parameters:
	- subsystem : The subsystem's data to set (input/ouput)
	- new_data : The new data value (input)
	- old_data: pointer to store old data (output
    Returns:
	- ERR_NULL_POINTER if subsystem is NULL
	- ERR_SUCCESS if success	
	
*/


int subsys_data_set(Subsystem *subsystem, unsigned int new_data, unsigned int *old_data) {
	if (subsystem == NULL) {
		return ERR_NULL_POINTER;  
	}

	if (old_data != NULL) {
		*old_data = subsystem->data; 
	}

	subsystem->data = new_data;  
	subsystem->status |= (1 << STATUS_DATA); //set the data status

	return ERR_SUCCESS; 
}

/*
Task: subsys_data_get [20 minutes to 25]
Gets the data stored in subsystem
    Parameters:
	- subsystem : retrieve data (input/output).
	- data : Pointer to store the retrieved data value (output).
    Returns:
	- ERR_NULL_POINTER if data pointer is NULL
	- ERR_NO_DATA if there is no data 
	- ERR_SUCCESS if sucess	
*/



int subsys_data_get(Subsystem *subsystem, unsigned int *data) {
	if (subsystem == NULL || data == NULL) {
		return ERR_NULL_POINTER;
	}
	if ((subsystem->status & (1 << STATUS_DATA)) == 0) { 
		*data = 0;
		return ERR_NO_DATA;
	}   

	*data = subsystem->data; 
	subsystem->data = 0;
	subsystem->status &= ~(1 << STATUS_DATA); 

	return ERR_SUCCESS; 
}




















