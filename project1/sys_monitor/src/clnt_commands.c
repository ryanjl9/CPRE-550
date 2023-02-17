/**
 * File: clnt_commands.c
 * Author: Ryan Lanciloti
 * Date of Creation: 2/14/2023
 * Description: This file contains functions that will be callback functions for the
 * 	console helper.
*/
#include <stdio.h>
#include "clnt_commands.h"
#include "sys_monitor.h"

/* GLOBAL VARIABLES */
server_info_t SERVERS[MAX_SERVER_CNT];
uint8_t SERVER_CNT = 0;
uint8_t CURRENT_SERVER_INDEX = 0;

/* CONSTANTS */
/* server_list */
const char* LIST_NO_SERVERS = "There are currently no servers available. Please add a "
							  "server.\n";

/* server_add */
const char* ADD_BAD_ARGS = "The number of arguments provided to server_add are "
						   "incorrect. Please see the help menu for more info.\n";
const char* SVR_CAP_MAX = "The maximum number of Servers (%d) has been reached.\n";

/* server_remove */
const char* RMV_BAD_ARGS = "The number of arguments provided to server_remove are "
						   "incorrect. Please see the help menu for more info.\n";
const char* RMV_NO_SERVERS = "There are no servers to remove from the list.\n";
const char* RMV_INVALID_INDEX = "The provided index is not valid.\n";

/* server_select */
const char* SEL_BAD_ARGS = "The number of arguments provided to server_select are "
						   "incorrect. Please see the help menu for more info.\n";
const char* SEL_INVALID_INDEX = "The provided index is not valid.\n";

/* SERVER COMMANDS */
/**
 * FUNCTION: server_list
 * 
 * DESCRIPTION: Prints the name of all of the 
 * 
 * PARAMS: args - Array containing arguments for the the function.
 * PARAMS: acnt - Arguments count 
 * 
 * EXAMPLE: server list
*/
void server_list(char* args[MAX_COMMAND_LINE_LEN], int acnt){
	if(SERVER_CNT == 0){
		printf("%s", LIST_NO_SERVERS);
		return;
	}
	printf("Current Servers:\n");
	int index;
	for(index = 0; index < SERVER_CNT; index++){
		if(index + 1 == CURRENT_SERVER_INDEX){
			printf("\t%d - %s*\n", index + 1, SERVERS[index].name);
		}else{
			printf("\t%d - %s\n", index + 1, SERVERS[index].name);
		}
	}
	printf("\n");
}

/**
 * @brief Adds a server to the list of servers
 * 
 * @param args Array containing arguments for the the function. If there are no
 * 	servers in the list, then if one is added, it will set as the currently selected
 * 	server.
 * @param acnt Arguments count 
 * 
 * @example server add localhost
*/
void server_add(char* args[MAX_COMMAND_LINE_LEN], int acnt){
	if(acnt != 1){
		printf("%s", ADD_BAD_ARGS);
		return;
	}
	if(SERVER_CNT == MAX_SERVER_CNT){
		printf("%s", SVR_CAP_MAX);
		return;
	}
	char hostname[MAX_COMMAND_LINE_LEN];
	strcpy(hostname, GET_PARAM_X(args, 0));

	CLIENT* clnt = clnt_create(hostname, SYS_MONITOR, SYS_MONITOR_VERSION, "udp");
	if(clnt == NULL){
		printf("Provided server \'%s\' does not exist.\n", hostname);
		return;
	}

	memcpy(&SERVERS[SERVER_CNT], hostname, sizeof(char)*strlen(hostname));
	SERVERS[SERVER_CNT].clnt = clnt;
	SERVER_CNT++;
	if(SERVER_CNT == 1) CURRENT_SERVER_INDEX = 1;
}

/**
 * FUNCTION: server_remove
 * 
 * DESCRIPTION: Removes a server from the list of servers by index. If you remove the
 * 	currently selected server, it will set your selected server back to 0. 
 * 
 * PARAMS: args - Array containing arguments for the the function
 * PARAMS: acnt - Arguments count 
 * 
 * EXAMPLE: server remove 5
*/
void server_remove(char* args[MAX_COMMAND_LINE_LEN], int acnt){
	if(acnt != 1){
		printf("%s", RMV_BAD_ARGS);
		return;
	}
	if(SERVER_CNT == 0){
		printf("%s", RMV_NO_SERVERS);
		return;
	}
	char* s_index = GET_PARAM_X(args, 0);
	int index = atoi(s_index) - 1;

	if(index < 0 || index > SERVER_CNT){
		printf("%s", RMV_INVALID_INDEX);
		return;
	}

	if(index == CURRENT_SERVER_INDEX-1) CURRENT_SERVER_INDEX = 0;

	memset(SERVERS[index].name, 0, sizeof(char) * MAX_SERVER_NAME_LEN);
	clnt_destroy(SERVERS[index].clnt);

	for(index; index < SERVER_CNT-1; index++){
		SERVERS[index] = SERVERS[index + 1];
	}
	SERVER_CNT--;
}

/**
 * FUNCTION: server_select
 * 
 * DESCRIPTION: Sets the server that RPC commands should be sent to.
 * 
 * PARAMS: args - Array containing arguments for the the function
 * PARAMS: acnt - Arguments count 
 * 
 * EXAMPLE: server select 4
*/
void server_select(char* args[MAX_COMMAND_LINE_LEN], int acnt){
	if(acnt != 1){
		printf("%s", SEL_BAD_ARGS);
		return;
	}
	char* s_index = GET_PARAM_X(args, 0);
	int index = atoi(s_index) - 1;
	if(index < 0 || index > SERVER_CNT){
		printf("%s", SEL_INVALID_INDEX);
		return;
	}
	CURRENT_SERVER_INDEX = index + 1;
}
