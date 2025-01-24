#include <stdio.h>
#include <stdlib.h>
#include "subsystem.h"

#define MENU_ADD 1
#define MENU_PRINT 2
#define MENU_PRINTALL 3
#define MENU_STATUS 4
#define MENU_REMOVE 5
#define MENU_FILTER 6
#define MENU_DATA 7
#define MENU_EXIT 0
#define MENU_MIN 0
#define MENU_MAX 7
#define FILTER_LENGTH 8

/*
Task: print_menu [an hour cumlative]
Display menu and get user's pick
    Steps
	- while loop display menu
	- swutch by what the user chooses 
	- edge case: if user doesnt type a valid option --> error
    Parameters:
   	- chocie: stores the user's pick
    Returns: 
    	- ERR_SUCCESS if printed right, else ERR_NULL_POINTER if choice is not valid	
   */

int print_menu(int *choice);

int main() {

SubsystemCollection collection;
SubsystemCollection filtered_collection;
char filter[FILTER_LENGTH + 1];
int choice;
char name[MAX_STR];
unsigned int status;
unsigned int data;
int result;
int index;
unsigned int value;
subsys_collection_init(&collection);
 subsys_collection_init(&filtered_collection);

while (1) {
    result = print_menu(&choice);
    if (result != ERR_SUCCESS) {
        printf("Error: Unable to print the menu.\n");
        continue;
    }

    switch (choice) {
        case MENU_ADD:
            printf("Enter subsystem name (no spaces) : ");
            scanf("%s", name);
            
            // when adding subsystem, we add it with defult values
            if (subsys_append(&collection, &(Subsystem){.name="", .status=status, .data=0}) == ERR_SUCCESS) {
                strncpy(collection.subsystems[collection.size - 1].name, name, MAX_STR - 1);
                collection.subsystems[collection.size - 1].name[MAX_STR - 1] = '\0';
                printf("Subsystem added successfully.\n");
            } else {
                printf("Error: Unable to add subsystem. Check for capacity.\n");
            }
            break;

        case MENU_PRINT:
            printf("Enter the name of the subsystem to print: ");
            scanf("%s", name);
            int index = subsys_find(&collection, name);
            if (index >= 0) {
                subsys_print(&collection.subsystems[index]);
            } else {
                printf("Error: Subsystem not found.\n");
            }
            break;

        case MENU_PRINTALL:
            result = subsys_collection_print(&collection);
            if (result == ERR_NO_DATA) {
                printf("No subsystems to print.\n");
            } else if (result != ERR_SUCCESS) {
                printf("Error: Unable to print subsystems.\n");
            }
            break;
            
    	case MENU_STATUS:

             printf("Enter the name of the subsystem to update: ");
                scanf("%s", name);
                index = subsys_find(&collection, name);
                if (index >= 0) {
                    printf("Enter the status bit to update (0-7): ");
                    scanf("%u", &status);
                    printf("Enter the value for the status bit (0-3): ");
                    scanf("%u", &value);
                    result = subsys_status_set(&collection.subsystems[index], status, value);
                    if (result == ERR_SUCCESS) {
                        printf("Status updated successfully.\n");
                    } else {
                        printf("Error: Invalid status or value.\n");
                    }
                } else {
                    printf("Error: Subsystem not found.\n");
                }
                break;
      
	 case MENU_REMOVE:
            printf("Enter the name of the subsystem to remove: ");
	    scanf("%s", name);
	    index = subsys_find(&collection, name); 
	    if (index >= 0) {
		subsys_remove(&collection, index); 
		printf("Subsystem '%s' removed successfully.\n", name);
	    } else {
		printf("Error: Subsystem not found.\n");
	    }
	    break;
	    
	case MENU_FILTER:
                printf("Enter a filter string of 8 characters (1, 0, *): ");
                scanf("%8s", filter);
                if (strlen(filter) != FILTER_LENGTH || strspn(filter, "01*") != FILTER_LENGTH) {
                    printf("Error: Invalid filter string. Please use only '1', '0', and '*'.\n");
                    break;
                }

                result = subsys_filter(&collection, &filtered_collection, (unsigned char *)filter);
                if (result == ERR_SUCCESS) {
                    printf("Filtered subsystems:\n");
                    result = subsys_collection_print(&filtered_collection);
                    if (result != ERR_SUCCESS) {
                        printf("Error: Unable to print filtered subsystems.\n");
                    }
                } else {
                    printf("Error: Filtering failed.\n");
                }
                break;
                
        case MENU_DATA:
                printf("Enter the name of the subsystem: ");
		    scanf("%s", name);
		    index = subsys_find(&collection, name);
		    if (index >= 0) {
			unsigned int new_data;
			printf("Enter the new hexadecimal value (in uppercase, without 0x): ");
			char hex_value[5];
			scanf("%4s", hex_value);
			
			//convert hex to int
			if (sscanf(hex_value, "%X", &new_data) == 1) {
			    collection.subsystems[index].data = new_data;
			    collection.subsystems[index].status |= STATUS_DATA; 
			    printf("Data updated successfully for %s.\n", name);
			} else {
			    printf("Error: Invalid hexadecimal value.\n");
			}
		    } else {
			printf("Error: Subsystem not found.\n");
		    }
		    break;
				
        case MENU_EXIT:
            printf("Exiting the program.\n");
            return 0;

        default:
            printf("Error: Invalid menu option selected.\n");
            break;
    }
}

   
    return 0;
}


int print_menu(int *choice) {
    if (choice == NULL) {
        return ERR_NULL_POINTER;
    }

    *choice = 0;

    do {
        printf("\n--- Subsystem Management Menu ---\n");
        printf("%d. Add Subsystem\n", MENU_ADD);
        printf("%d. Print Subsystem\n", MENU_PRINT);
        printf("%d. Print All Subsystems\n", MENU_PRINTALL);
        printf("%d. Set Subsystem Status\n", MENU_STATUS);
        printf("%d. Remove Subsystem\n", MENU_REMOVE);
        printf("%d. Filter Subsystems\n", MENU_FILTER);
        printf("%d. Set Subsystem Data\n", MENU_DATA);
        printf("%d. Exit\n", MENU_EXIT);
        printf("Select an option: ");
        scanf("%d", choice);
        while (getchar() != '\n');

        if (*choice < MENU_MIN || *choice > MENU_MAX) {
            printf("Error: Invalid choice. Please try again.\n");
        }
    } while (*choice < MENU_MIN || *choice > MENU_MAX);

    return ERR_SUCCESS;
}
