/**
 * File: console_helper.h
 * Author: Ryan Lanciloti
 * Date of Creation: 2/12/2023
 * Description: This header file will hold the external function and variables used by
 * the console_helper.c file.
*/
#ifndef _CONSOLE_HELPER_H
#define _CONSOLE_HELPER_H
#include "proj_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* DEFINES */
#define MAX_COMMAND_ARGS 25
#define MAX_COMMAND_LINE_LEN 25
#define GET_PARAM_X(arr, index) ((char*)arr + MAX_COMMAND_LINE_LEN * index)

/* STRUCTS */
typedef struct menu_item_t menu_item_t;

struct menu_item_t{
	char* cmd_name;
	char* cmd_desc;
	void (*cback_func)(char(*)[MAX_COMMAND_LINE_LEN], int);
	menu_item_t* sub_cmds;
	uint8_t sub_cmd_cnt;
};

/* ENUMS */
typedef enum {
	CH_INIT_NO_ERROR = 0,
	CH_INIT_INTRO_MSG_NULL,
	CH_INIT_MENU_ITEMS_NULL,
	CH_INIT_MENU_ITEMS_INTRO_MSG_NULL,
	CH_INIT_ITEM_CNT_ZERO,
	CH_INIT_ITEM_CNT_INTRO_MSG_NULL,
	CH_INIT_ITEM_CNT_MENU_ITEMS_ITEMS_NULL,
	CH_INIT_ITEM_CNT_MENU_ITEMS_INTRO_MSG_NULL
} ch_init_errors;

typedef enum {
	CH_PRINT_INTRO_NO_ERROR = 0,
	CH_PRINT_INTRO_MSG_NULL
} ch_print_introduction_errors;

typedef enum {
	CH_EXECUTE_NO_ERROR = 0,
	CH_EXECUTE_CMD_NULLPTR,
	CH_EXECUTE_MENU_NULLPTR,
	CH_EXECUTE_CMD_DOESNT_EXIST
} ch_execute_erros;

/* MESSAGES */
#ifdef _UNIT_TEST
extern char* INTRO_MSG; 
extern menu_item_t* MENU_ITEMS;
extern uint8_t ITEM_CNT;
#endif //_UNIT_TEST

/* PUBLIC FUNCTION */
extern uint8_t ch_init(char*, menu_item_t*, uint8_t);
extern uint8_t ch_print_introduction();
extern uint8_t ch_execute(char* cmd);
extern void ch_reset();

#ifdef __cplusplus
}
#endif

#endif //_CONSOLE_HELPER_H