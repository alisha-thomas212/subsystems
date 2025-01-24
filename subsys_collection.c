#include "subsystem.h"
#include <string.h>

// constants for bit positions 
#define PWR_BIT 0  
#define DATA_BIT 1  
#define ACT_BIT 2  
#define ERR_BIT 3  
#define PERF_BIT 4 
#define RES_BIT 5
/*
    This file should contain your documentation and implementations for SubsystemCollection 
    specific functionality; initialization, appending, removal, printing, finding, and filtering.
*/

/*
Task: subsys_collection_init [5 minutes to 10]
Initalizes a subsytemcollection and sets its size to 0
   Parameters:
 	- subsystems : Pointer to the collection to initialize (output).
   Returns:
	- ERR_NULL_POINTER if subsystems is NULL.
	- ERR_SUCCESS on success.	
   
*/

int subsys_collection_init(SubsystemCollection *subsystems){
	if (subsystems == NULL){
		return ERR_NULL_POINTER;  
	}
	
	subsystems->size = 0;
	return ERR_SUCCESS;
}

/*
Task: subsys_append [10 minutes to 20]
Appends a subsystem to the subsystemcollection
   Parameters:
	- subsystems : Pointer to the collection to add to (input/output)
	- subsystem : Pointer to the subsystem to add (input)
   Returns:
	- ERR_NULL_POINTER if subsystems or subsystem is NULL
	- ERR_MAX_CAPACITY if it reached its maximum capacity
	- ERR_SUCCESS 
   
*/

	
int subsys_append(SubsystemCollection *subsystems, const Subsystem *subsystem){
	if (subsystem == NULL || subsystems == NULL) {
        	return ERR_NULL_POINTER;  
    	}
    	
    	if (subsystems->size >= MAX_ARR) {
        	return ERR_MAX_CAPACITY; 
    	}
    	
    	subsystems->subsystems[subsystems->size] = *subsystem;//append the subsystems to collections
    	subsystems->size++;
        return ERR_SUCCESS;
}


/*
Task: subsys_collection_print [15 minutes to 20]
Prints all subsystems 
   Parameters:
	- subsystems : The collection to print (input)
   Returns:
	- ERR_No_DATA if collection is NULL/empty
	- ERR_SUCCESS 
   
*/


int subsys_collection_print(SubsystemCollection *subsystems) {
	if (subsystems == NULL || subsystems->size == 0) {
        	return ERR_NO_DATA; 
    	}

	for (int i = 0; i < subsystems->size; i++) {
		Subsystem *subsystem = &subsystems->subsystems[i];
		//extract individual bits 
		unsigned int status = subsystem->status;
		unsigned int pwr = (status >> PWR_BIT) & 1;
		unsigned int data = (status >> DATA_BIT) & 1;
		unsigned int act = (status >> ACT_BIT) & 1;
		unsigned int err = (status >> ERR_BIT) & 1;
		unsigned int perf = (status >> PERF_BIT) & 1;
		unsigned int res = (status >> RES_BIT) & 1;

		printf("[Name: %16s, Status:(PWR: %u|DATA: %u|ACT: %u|ERR: %u|PERF: %u|RES: %u)]\n",
               		subsystem->name, pwr, data, act, err, perf, res);
    }

    return ERR_SUCCESS;
}

/*
Task: subsys_remove []
Removes a subsystem 
   Parameters:
	- subsystems : The collection to edit (input/output)
	- index: indez of subsystem (input)
	
   Returns:
	- -1 if subsystems is NULL or index is out of range
	- -2 if the collection is empty
	- 0 on successful removal
   
*/

int subsys_remove(SubsystemCollection *subsystems, int index) {
	if (subsystems == NULL) {
		return -1; 
	}

	if (subsystems->size == 0) {
		return -2;
	}

	if (index < 0 || index >= subsystems->size) {
		return -1; 
	}
	//shift so subsystems can be removed
	for (int i = index; i < subsystems->size - 1; i++) {
		subsystems->subsystems[i] = subsystems->subsystems[i + 1];
	}

	subsystems->size--; 
		return 0;
	}


/*
Task: subsys_filter [30 minutes to 40]
Filters subsystems based on a status
    Steps:
	- null pointer edge case
	- inistalize collection 
	- make a filter mask, bitwise OR and invert it 
	- use a for loop to filter the subsystems 
	- return success
   Parameters:
	- src : Source collection to filter (input)
	- dest : store matching subsystems (output)
	- filter : Filter mask  (input)	
   Returns:
	- ERR_NULL_POINTER if any parametes is NULL
	- ERR_SUCCESS 	
*/


int subsys_filter(const SubsystemCollection *src, SubsystemCollection *dest, const unsigned char *filter) {
	if (!src || !dest || !filter) {
		return ERR_NULL_POINTER;
	}
	
	dest->size = 0;
	
	unsigned char filter_mask = 0;
	unsigned char wildcard_mask = 0;

	for (int i = 0; i < 8; i++) {
		if (filter[i] == '1') {
		    	filter_mask |= (1 << (7 - i)); //set bit for filter
		} else if (filter[i] == '*') {
		    	wildcard_mask |= (1 << (7 - i)); //set bit for wildcard
		}
	}

	filter_mask = ~filter_mask;
	
	//for loop to loop through collection
	for (unsigned int i = 0; i < src->size; i++) {
		unsigned char status = src->subsystems[i].status;
		unsigned char result = (filter_mask ^ status) | wildcard_mask;

		if (result == 0xFF) {
		    dest->subsystems[dest->size] = src->subsystems[i];
		    dest->size++;
		}
	}

	return ERR_SUCCESS;
}




























