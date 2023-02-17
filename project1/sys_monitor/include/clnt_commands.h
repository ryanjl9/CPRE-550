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
extern uint8_t CURRENT_SERVER_INDEX;

/* server_list */
extern const char* LIST_NO_SERVERS;

/* server_add */
extern const char* ADD_BAD_ARGS;
extern const char* SVR_CAP_MAX;

/* server_remove */
extern const char* RMV_BAD_ARGS;
extern const char* RMV_NO_SERVERS;
extern const char* RMV_INVALID_INDEX;

/* server_select */
extern const char* SEL_BAD_ARGS;
extern const char* SEL_INVALID_INDEX;
#endif //_UNIT_TEST

/* PUBLIC FUNCTIONS */
extern void server_list(char* args[MAX_COMMAND_LINE_LEN], int acnt);
extern void server_add(char* args[MAX_COMMAND_LINE_LEN], int acnt);
extern void server_remove(char* args[MAX_COMMAND_LINE_LEN], int acnt);
extern void server_select(char* args[MAX_COMMAND_LINE_LEN], int acnt);

#endif //_CLNT__COMMANDS_H
