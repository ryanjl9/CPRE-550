/**
 * File: clnt_commands.c
 * Author: Ryan Lanciloti
 * Date of Creation: 2/14/2023
 * Description: This file contains functions that will be callback functions for the
 * 	console helper.
*/
#include <stdio.h>
#include "clnt_commands.h"

#define DT_VALID_ARGS_CNT 3

/* GLOBAL VARIABLES */
server_info_t SERVERS[MAX_SERVER_CNT];
uint8_t SERVER_CNT = 0;
uint8_t CUR_SVR_INDEX = 0;

/* CONSTANTS */
/* general */
const char* NO_SERVER_SELECTED = "There is currently no server selected. Please selected"
								 " a server to make active.\n";
const char* INVALID_INDEX = "The provided index is not valid.\n";

/* server_list */
const char* LIST_NO_SERVERS = "There are currently no servers available. Please add a "
							  "server.\n";

/* server_add */
const char* SVR_CAP_MAX = "The maximum number of Servers (%d) has been reached.\n";

/* server_remove */
const char* RMV_NO_SERVERS = "There are no servers to remove from the list.\n";

/* get_datetime */
const char* DT_INVALID_ARG = "The provided argument is not valid. Please view help menu "
							 "for parameter options.\n";
const char* DT_VALID_ARGS[DT_VALID_ARGS_CNT] = {"date", "time", "both"};

/**
 * @brief: Prints the name of all of the 
 * 
 * PARAMS: args - Array containing arguments for the the function.
 * PARAMS: acnt - Arguments count 
 * 
 * EXAMPLE: server list
*/
void server_list(char* args[MAX_COMMAND_LINE_LEN], int acnt){
	(void)args;
	(void)acnt;
	if(!SERVER_CNT){
		printf("%s", LIST_NO_SERVERS);
		return;
	}
	printf("Current Servers:\n");
	int index;
	for(index = 0; index < SERVER_CNT; index++){
		if(index + 1 == CUR_SVR_INDEX){
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
		printf("%s", BAD_ARGS);
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
	if(SERVER_CNT == 1) CUR_SVR_INDEX = 1;
}

/**
 * @brief: Removes a server from the list of servers by index. If you remove the
 * 	currently selected server, it will set your selected server back to 0. 
 * 
 * PARAMS: args - Array containing arguments for the the function
 * PARAMS: acnt - Arguments count 
 * 
 * EXAMPLE: server remove 5
*/
void server_remove(char* args[MAX_COMMAND_LINE_LEN], int acnt){
	if(acnt != 1){
		printf("%s", BAD_ARGS);
		return;
	}
	if(!SERVER_CNT){
		printf("%s", RMV_NO_SERVERS);
		return;
	}
	char* s_index = GET_PARAM_X(args, 0);
	int index = atoi(s_index) - 1;

	if(index < 0 || index > SERVER_CNT){
		printf("%s", INVALID_INDEX);
		return;
	}

	if(index == CUR_SVR_INDEX-1) CUR_SVR_INDEX = 0;

	memset(SERVERS[index].name, 0, sizeof(char) * MAX_SERVER_NAME_LEN);
	clnt_destroy(SERVERS[index].clnt);

	for(; index < SERVER_CNT-1; index++){
		SERVERS[index] = SERVERS[index + 1];
	}
	SERVER_CNT--;
}

/**
 * @brief: Sets the server that RPC commands should be sent to.
 * 
 * PARAMS: args - Array containing arguments for the the function
 * PARAMS: acnt - Arguments count 
 * 
 * EXAMPLE: server select 4
*/
void server_select(char* args[MAX_COMMAND_LINE_LEN], int acnt){
	if(acnt != 1){
		printf("%s", BAD_ARGS);
		return;
	}
	char* s_index = GET_PARAM_X(args, 0);
	int index = atoi(s_index) - 1;
	if(index < 0 || index > SERVER_CNT){
		printf("%s", INVALID_INDEX);
		return;
	}
	CUR_SVR_INDEX = index + 1;
}

/**
 * @brief When called, this function will return either the date, time, or both from a
 * 	remote machine.
 * 
 * @param: args - Array containing arguments for the the function
 * @param: acnt - Arguments count 
 * 
 * @example datetime date -> February 6, 2023
 * @example datetime time -> 10:47:21
 * @example datetime both -> February 6, 2023 - 10:47:21
 */
void get_datetime(char* args[MAX_COMMAND_LINE_LEN], int acnt){
	if(acnt != 1){
		printf("%s", BAD_ARGS);
		return;
	}
	if(!CUR_SVR_INDEX){
		printf("%s", NO_SERVER_SELECTED);
		return;
	}

	int index;
	char* cmd = GET_PARAM_X(args, 0);

	for(index = 0; index < DT_VALID_ARGS_CNT; index++) 
		if(!strcmp(DT_VALID_ARGS[index], cmd)) break;
	
	if(index == DT_VALID_ARGS_CNT){
		printf("%s", DT_INVALID_ARG);
		return;
	}

	enum dt_ops_t dt_param_index = index;
	char** datetime_retval;
	datetime_retval = getdatetime(&dt_param_index, SERVERS[CUR_SVR_INDEX-1].clnt);
	printf("%s -> %s\n", SERVERS[CUR_SVR_INDEX-1].name, (char*)datetime_retval);
}

/**
 * @brief When called, this function will get various statistics about memory 
 * utilization on the remote computer.
 * 
 * @param: args - Array containing arguments for the the function
 * @param: acnt - Arguments count 
 * 
 * @example system_info
 * @returns A struct containing information about the remote CPU. Description for what is
 *  in the struct can be found above.
 */
void get_systeminfo(char* args[MAX_COMMAND_LINE_LEN], int acnt){
	(void)args;
	if(acnt != 0){
		printf("%s", BAD_ARGS);
		return;
	}
	if(!CUR_SVR_INDEX){
		printf("%s", NO_SERVER_SELECTED);
		return;
	}
	struct sysinfo* systeminfo = getsysteminfo(nullptr, SERVERS[CUR_SVR_INDEX-1].clnt);
	printf("%s -> System Info:\n\tUptime (seconds): %ld\n\tNumber of Processes: %hu\n\n", 
		SERVERS[CUR_SVR_INDEX-1].name, systeminfo->uptime, systeminfo->procs);
}

/**
 * @brief When called, this function will get various statistics about memory utilization
 *  on the remote computer.
 * 
 * @param args Array containing arguments for the the function
 * @param acnt Arguments count 
 * 
 * @example memory_utilization
 * @returns A struct containing information about the remote computer's memory 
 *  utilization. A full description of what is in the struct can be found above.
 */
void get_memoryutilization(char* args[MAX_COMMAND_LINE_LEN], int acnt){
	(void)args;
	if(acnt != 0){
		printf("%s", BAD_ARGS);
		return;
	}
	if(!CUR_SVR_INDEX){
		printf("%s", NO_SERVER_SELECTED);
		return;
	}
	struct mem_usage_ret* mem_usage;
	mem_usage = getmemoryutilization(nullptr, SERVERS[CUR_SVR_INDEX-1].clnt);
	printf(
		"%s -> Memory Utilization:\n"
		"\tNumber of Free Chunks: %d\n"
		"\tNumber of fastbin blocks: %d\n"
		"\tNumber of Mapped Regions: %d\n"
		"\tSpace in mapped regions: %d\n"
		"\tSpace in fastbin blocks: %d\n"
		"\tPage size: %d\n"
		"\tPhysical page count: %ld\n"
		"\tAvailable physical page count: %ld\n"
		"\tTotal allocated space: %d\n"
		"\tTotal free space: %d\n\n",
		SERVERS[CUR_SVR_INDEX-1].name, mem_usage->dynamic_memory_usage.ordblks,
		mem_usage->dynamic_memory_usage.smblks, mem_usage->dynamic_memory_usage.hblks,
		mem_usage->dynamic_memory_usage.hblkhd, mem_usage->dynamic_memory_usage.fsmblks,
		mem_usage->page_size, mem_usage->phys_page_cnt, 
		mem_usage->available_phys_page_cnt, mem_usage->dynamic_memory_usage.uordblks,
		mem_usage->dynamic_memory_usage.fordblks
	);
}

/**
 * @brief When called, this function will get the 1, 5, and 15 minute load averages of 
 * 	the remote system.
 * 
 * @param args Array containing arguments for the the function
 * @param acnt Arguments count 
 * 
 * @example load_info
 */
void get_loadinfo(char* args[MAX_COMMAND_LINE_LEN], int acnt){
	(void)args;
	if(acnt != 0){
		printf("%s", BAD_ARGS);
		return;
	}
	if(!CUR_SVR_INDEX){
		printf("%s", NO_SERVER_SELECTED);
		return;
	}
	double* retval = getloadprocsperminute(nullptr, SERVERS[CUR_SVR_INDEX-1].clnt);
	printf(
		"%s -> Loaded Processes Per Minute:\n"
		"\t1 Minute: %0.3lf\n"
		"\t5 Minutes: %0.3lf\n"
		"\t15 Minutes: %0.3lf\n\n", 
		SERVERS[CUR_SVR_INDEX-1].name, 
		retval[ONE_MINUTE], retval[FIVE_MINUTE], retval[FIFTEEN_MINUTE]
	);
}

/**
 * @brief When called, this function will get a struct containing a list of all username on the
 *  remote system.
 * 
 * @param args Array containing arguments for the the function
 * @param acnt Arguments count
 * 
 * @example user_info
 */
void get_usernames(char* args[MAX_COMMAND_LINE_LEN], int acnt){
	(void)args;
	if(acnt != 0){
		printf("%s", BAD_ARGS);
		return;
	}
	if(!CUR_SVR_INDEX){
		printf("%s", NO_SERVER_SELECTED);
		return;
	}
	int index;
	struct user_info* retval = getusernames(nullptr, SERVERS[CUR_SVR_INDEX-1].clnt);
	
	printf("%s -> Users on the remote machine:\n", SERVERS[CUR_SVR_INDEX-1].name);
	for(index = 0; index < retval->count; index++){
		printf("\t%s\n", retval->user_list[index]);
	}
	printf("\n");
}
