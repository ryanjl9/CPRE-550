/**
 * File: console_helper.h
 * Author: Ryan Lanciloti
 * Date of Creation: 2/12/2023
 * Description: This header file will hold the external function and variables used by
 * the console_helper.c file.
*/
#ifndef _CONSOLE_HELPER_H
#define _CONSOLE_HELPER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "proj_config.h"

#ifdef _UNIT_TEST
#include "mock_functions.h"
#endif //_UNIT_TEST

#define INTRO_MSG_SIZE 1024
#define MAX_HELP_FORMAT_SIZE 15

/* STRUCTS */
typedef struct menu_item_t menu_item_t;

struct menu_item_t{
	const char* cmd_name;
	const char* cmd_desc;
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
	CH_INIT_ITEM_CNT_MENU_ITEMS_INTRO_MSG_NULL,
	CH_INIT_BAD_INTRO_MSG
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

typedef enum {
	CH_PH_NO_ERROR = 0,

} ch_print_help_errors;

/* MESSAGES */
#ifdef _UNIT_TEST
extern char INTRO_MSG[INTRO_MSG_SIZE]; 
extern menu_item_t* MENU_ITEMS;
extern uint8_t ITEM_CNT;

extern const char* NULL_INTRO_MESSAGE;
extern const char* OVERSIZED_INTRO_MESSAGE;
extern const char* NO_MENU_ITEMS;
extern const char* NO_SUB_COMMND;
extern const char* BAD_COMMAND;
#endif //_UNIT_TEST
extern const char* BAD_ARGS;

/* PUBLIC FUNCTION */
/**
 * @brief: This function will initialize the 
 * 
 * @param intro_msg Message to be displayed when the app starts.
 * @param menu_items Pointer to an array of menu_items which are the valid commands we
 * 	can execute.
 * @param item_cnt Number of commands in the menu_items struct
 * 
 * @return Consult the enum ch_init_errors for decoding the output.
*/
extern uint8_t ch_init(char*intro_msg, menu_item_t* menu_items, uint8_t item_cnt);

/**
 * @brief: This function will print the introduction message associated with the
 * 	Console Helper file.
 * 
 * @return 0 if successful, else 1
*/
extern uint8_t ch_print_introduction(void);

/**
 * @brief: This function will parse a string and will execute the command which it
 * 	maps to.
 * 
 * @param cmd - Command to be parsed
 * 
 * @return Consult the enum ch_execute_errors for decoding the output. 
*/
extern uint8_t ch_execute(char* cmd);

/**
 * @brief: This function will clear the state variables in console_helper
*/
extern void ch_reset(void);

/**
 * @brief Prints help menu describing each command
 * 
 * @param args Array containing arguments for the the function.
 * @param acnt Arguments count 
 * 
 * @example help
 * @example help <command>
 */
extern void ch_help(char* args[MAX_COMMAND_LINE_LEN], int acnt);

/**
 * @brief Exits the application
 * 
 * @param args Array containing arguments for the the function.
 * @param acnt Arguments count 
 * 
 * @example exit
 */
extern void ch_quit(char* args[MAX_COMMAND_LINE_LEN], int acnt);

#define GET_PARAM_X(arr, index) ((char*)arr + MAX_COMMAND_LINE_LEN * index)

#ifdef __cplusplus
}
#endif

#endif //_CONSOLE_HELPER_H