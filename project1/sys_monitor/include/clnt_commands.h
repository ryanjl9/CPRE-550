/**
 * File: clnt_commands.h
 * Author: Ryan Lanciloti
 * Date of Creation: 2/12/2023
 * Description: This file contains functions that abstract out console interaction with
 * the user.
*/
#ifndef _CLNT__COMMANDS_H
#define _CLNT__COMMANDS_H
#include <rpc/rpc.h>
#include "proj_config.h"
#include "console_helper.h"
#include "sys_monitor.h"

#ifdef _UNIT_TEST
#include "mock_functions.h"
#endif //_UNIT_TEST

typedef struct {
	char name[MAX_SERVER_NAME_LEN];
	CLIENT* clnt;
} server_info_t;

#ifdef _UNIT_TEST
#ifdef clnt_destroy
	#undef clnt_destroy
	#define clnt_destroy(rh) DBG_PRINTF("Destroying Client: %p\n", rh);
#endif //clnt_destroy
extern server_info_t SERVERS[MAX_SERVER_CNT];
extern uint8_t SERVER_CNT;
extern uint8_t CUR_SVR_INDEX;

/* general */
extern const char* NO_SERVER_SELECTED;
extern const char* INVALID_INDEX;

/* server_list */
extern const char* LIST_NO_SERVERS;

/* server_add */
extern const char* SVR_CAP_MAX;

/* server_remove */
extern const char* RMV_NO_SERVERS;

/* get_datetime */
extern const char* DT_INVALID_ARG;

#endif //_UNIT_TEST

#ifndef _UNIT_TEST
	#define getdatetime(argp, clnt) getdatetime_1(argp, clnt)
	#define getsysteminfo(argp, clnt) getsysteminfo_1(argp, clnt)
	#define getmemoryutilization(argp, clnt) getmemoryutilization_1(argp, clnt)
	#define getloadprocsperminute(argp, clnt) getloadprocsperminute_1(argp, clnt)
#endif //_UNIT_TEST

/* PUBLIC FUNCTIONS */
extern void server_list(char* args[MAX_COMMAND_LINE_LEN], int acnt);
extern void server_add(char* args[MAX_COMMAND_LINE_LEN], int acnt);
extern void server_remove(char* args[MAX_COMMAND_LINE_LEN], int acnt);
extern void server_select(char* args[MAX_COMMAND_LINE_LEN], int acnt);

extern void get_datetime(char* args[MAX_COMMAND_LINE_LEN], int acnt);
extern void get_systeminfo(char* args[MAX_COMMAND_LINE_LEN], int acnt);
extern void get_memoryutilization(char* args[MAX_COMMAND_LINE_LEN], int acnt);
extern void get_loadinfo(char* args[MAX_COMMAND_LINE_LEN], int acnt);

#endif //_CLNT__COMMANDS_H
