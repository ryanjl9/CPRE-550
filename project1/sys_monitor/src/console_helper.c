/**
 * File: console_helper.c
 * Author: Ryan Lanciloti
 * Date of Creation: 2/12/2023
 * Description: This file contains functions that abstract out console interaction with
 * the user.
*/
#include <stdio.h>
#include <string.h>
#include "console_helper.h"
#include "proj_config.h"

/* GLOBAL STATE VARIABLES */
char* INTRO_MSG = nullptr;
menu_item_t* MENU_ITEMS = nullptr;
uint8_t ITEM_CNT = 0;

/* ERROR MESSAGES */
const char* NULL_INTRO_MESSAGE = \
	"Introduction message not specified. Make sure to call ch_init().";

/* PRIVATE FUNCTION HEADERS */
uint8_t _ch_execute_helper(
	char cmd_name_lst[MAX_COMMAND_ARGS][MAX_COMMAND_LINE_LEN], 
	int args_cnt, 
	menu_item_t* cur_menu, 
	uint8_t menu_size
);

/* FUNCTION IMPLEMENTATIONS */
/**
 * FUNCTION: ch_init
 * 
 * DESCRIPTION: This function will initialize the 
 * 
 * PARAM: intro_msg - Message to be displayed when the app starts.
 * PARAM: menu_items - Pointer to an array of menu_items which are the valid commands we
 * 	can execute.
 * 
 * RETURNS: Consult the enum ch_init_errors for decoding the output.
*/
uint8_t ch_init(char* intro_msg, menu_item_t* menu_items, uint8_t item_cnt){
	if(intro_msg == nullptr || menu_items == nullptr){
		int retval = 0;
		retval |= (intro_msg == nullptr) ? CH_INIT_INTRO_MSG_NULL : CH_INIT_NO_ERROR;
		retval |= (menu_items == nullptr) ? CH_INIT_MENU_ITEMS_NULL : CH_INIT_NO_ERROR;
		retval |= (item_cnt == 0) ? CH_INIT_ITEM_CNT_ZERO : CH_INIT_NO_ERROR;
		return retval;
	}
	INTRO_MSG = intro_msg;
	MENU_ITEMS = menu_items;
	ITEM_CNT = item_cnt;
	return CH_INIT_NO_ERROR;
}

/**
 * FUNCTION: ch_print_introduction
 * 
 * DESCRIPTION: This function will print the introduction message associated with the
 * 	Console Helper file.
 * 
 * RETURNS: 0 if successful, else 1
*/
uint8_t ch_print_introduction(){
	if(INTRO_MSG == nullptr){
		printf("%s", NULL_INTRO_MESSAGE);
		return CH_PRINT_INTRO_MSG_NULL;
	}
	printf("%s", INTRO_MSG);
	return CH_PRINT_INTRO_NO_ERROR;
}

/**
 * FUNCTION: ch_execute
 * 
 * DESCRIPTION: This function will parse a string and will execute the command which it
 * 	maps to.
 * 
 * PARAM: cmd - Command to be parsed
 * 
 * RETURNS: Consult the enum ch_execute_errors for decoding the output. 
*/
uint8_t ch_execute(char* cmd){
	if(cmd == nullptr) return CH_EXECUTE_CMD_NULLPTR;
	if(MENU_ITEMS == nullptr) return CH_EXECUTE_MENU_NULLPTR;
	char split_command[MAX_COMMAND_ARGS][MAX_COMMAND_LINE_LEN];
	int num_cmd_args = 0;

	char* token = strtok(cmd, " ");
	while(token != NULL){
		strcpy(split_command[num_cmd_args], token);
		num_cmd_args++;
		token = strtok(NULL, " ");
	}
	
	return _ch_execute_helper(split_command, num_cmd_args, MENU_ITEMS, ITEM_CNT);
}

/**
 * FUNCTION: _ch_execute_helper
 * 
 * DESCRIPTION: This is a helper function which can be used to traverse the command menu
 * 	recursively
 * 
 * PARAM: cmd_name_lst - Command list
 * PARAM: args_cnt - Number of arguments in the command list
 * PARAM: cur_menu - Current menu that we are checking the command to exist in
 * PARAM: menu_size - The number of entries in the current menu
 * 
 * RETURNS: Consult the enum ch_execute_errors for decoding the output. 
*/
uint8_t _ch_execute_helper(
	char cmd_name_lst[MAX_COMMAND_ARGS][MAX_COMMAND_LINE_LEN], 
	int args_cnt, 
	menu_item_t* cur_menu, 
	uint8_t menu_size
){
	if(!args_cnt) return 0;
	
	int menu_entry;
	menu_item_t* cmd;
	for(menu_entry = 0; menu_entry < menu_size; menu_entry++){
		cmd = &cur_menu[menu_entry];
		if(!strcmp(cmd->cmd_name, cmd_name_lst[0])){
			if(cur_menu[menu_entry].sub_cmds != nullptr){
				char* token = strtok(NULL, " ");
				return _ch_execute_helper(
					&(cmd_name_lst[1]), 
					args_cnt-1, 
					cmd->sub_cmds, 
					cmd->sub_cmd_cnt
				);
			}
			if(cur_menu[menu_entry].cback_func != nullptr) {
				if(args_cnt >= 1) (*(cmd->cback_func))(&(cmd_name_lst[1]), args_cnt-1);
				else (*(cmd->cback_func))(nullptr, args_cnt-1);
			}
			return CH_EXECUTE_NO_ERROR;
		}
	}
	return CH_EXECUTE_CMD_DOESNT_EXIST;
}

/**
 * FUNCTION: ch_reset
 * 
 * DESCRIPTION: This function will clear the state variables in console_helper
*/
void ch_reset(){
	INTRO_MSG = nullptr;
	MENU_ITEMS = nullptr;
	ITEM_CNT = 0;
}
