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

/* GLOBAL STATE VARIABLES */
char INTRO_MSG[INTRO_MSG_SIZE] = {0};
menu_item_t* MENU_ITEMS = nullptr;
uint8_t ITEM_CNT = 0;

/* ERROR MESSAGES */
const char* NULL_INTRO_MESSAGE = \
	"Introduction message not specified. Make sure to call ch_init().\n";
const char* OVERSIZED_INTRO_MESSAGE = \
	"Introduction message provided is too large.\n";
const char* NO_MENU_ITEMS = \
	"There are no commands registered.\n";
const char* NO_SUB_COMMND = \
	"Provided command does not have any subcommands.\n";
const char* BAD_COMMAND = \
	"The provided command could not be found.\n";
const char* BAD_ARGS = \
	"The number of arguments provided are incorrect. Please see the "
	" help menu for more info.\n";

/* PRIVATE FUNCTION HEADERS */
/**
 * @brief: This is a helper function which can be used to traverse the command menu
 * 	recursively
 * 
 * @param cmd_name_lst - Command list
 * @param args_cnt - Number of arguments in the command list
 * @param cur_menu - Current menu that we are checking the command to exist in
 * @param menu_size - The number of entries in the current menu
 * 
 * @return Consult the enum ch_execute_errors for decoding the output. 
*/
uint8_t _ch_execute_helper(
	char cmd_name_lst[MAX_COMMAND_ARGS][MAX_COMMAND_LINE_LEN], 
	int args_cnt, 
	menu_item_t* cur_menu, 
	uint8_t menu_size
);

/* FUNCTION IMPLEMENTATIONS */

uint8_t ch_init(char* intro_msg, menu_item_t* menu_items, uint8_t item_cnt){
	if(intro_msg == nullptr || menu_items == nullptr){
		int retval = 0;
		retval |= (intro_msg == nullptr) ? CH_INIT_INTRO_MSG_NULL : CH_INIT_NO_ERROR;
		retval |= (menu_items == nullptr) ? CH_INIT_MENU_ITEMS_NULL : CH_INIT_NO_ERROR;
		retval |= (!item_cnt) ? CH_INIT_ITEM_CNT_ZERO : CH_INIT_NO_ERROR;
		return retval;
	}
	if(strlen(intro_msg) >= INTRO_MSG_SIZE){
		printf("%s", OVERSIZED_INTRO_MESSAGE);
		return CH_INIT_BAD_INTRO_MSG;
	}
	strcpy(INTRO_MSG, intro_msg);
	MENU_ITEMS = menu_items;
	ITEM_CNT = item_cnt;
	return CH_INIT_NO_ERROR;
}

uint8_t ch_print_introduction(void){
	if(!INTRO_MSG[0]){
		printf("%s", NULL_INTRO_MESSAGE);
		return CH_PRINT_INTRO_MSG_NULL;
	}
	printf("%s", INTRO_MSG);
	return CH_PRINT_INTRO_NO_ERROR;
}

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
				strtok(NULL, " ");
				return _ch_execute_helper(
					&(cmd_name_lst[1]), 
					args_cnt - 1, 
					cmd->sub_cmds, 
					cmd->sub_cmd_cnt
				);
			}
			if(cur_menu[menu_entry].cback_func != nullptr) {
				if(args_cnt >= 1) (*(cmd->cback_func))(&(cmd_name_lst[1]), args_cnt - 1);
				else (*(cmd->cback_func))(nullptr, args_cnt - 1);
			}
			return CH_EXECUTE_NO_ERROR;
		}
	}
	return CH_EXECUTE_CMD_DOESNT_EXIST;
}

void ch_reset(void){
	memset(INTRO_MSG, 0, sizeof(INTRO_MSG));
	MENU_ITEMS = nullptr;
	ITEM_CNT = 0;
}

void ch_help(char* args[MAX_COMMAND_LINE_LEN], int acnt){
	if(MENU_ITEMS == nullptr){
		printf("%s", NO_MENU_ITEMS);
		return;
	}
	
	menu_item_t* current_menu = MENU_ITEMS;
	int menu_size = ITEM_CNT;
	int index, cmd_depth;
	char params[MAX_COMMAND_ARGS][MAX_COMMAND_LINE_LEN];
	for(index = 0; index < acnt; index++){
		strcpy(params[index], GET_PARAM_X(args, index));
	}

	for(cmd_depth = 0; cmd_depth < acnt; cmd_depth++){
		for(index = 0; index < menu_size; index++){
			if(!strcmp(current_menu[index].cmd_name, params[cmd_depth])){
				if(current_menu[index].sub_cmds == nullptr){
					printf("%s", NO_SUB_COMMND);
					return;
				}
				menu_size = current_menu[index].sub_cmd_cnt;
				current_menu = current_menu[index].sub_cmds;
				break;
			}
		}
		if(index == menu_size){
			printf("%s", BAD_COMMAND);
			return;
		}
	}

	for(index=0; index < menu_size; index++){
		if(current_menu[index].sub_cmds != nullptr){ 
			printf(
				"%s* - %s\n", current_menu[index].cmd_name, current_menu[index].cmd_desc
			);
		}else{ 
		printf(
			"%s - %s\n", current_menu[index].cmd_name, current_menu[index].cmd_desc
		);
		}
	}
	printf("\n");
}

void ch_quit(char* args[MAX_COMMAND_LINE_LEN], int acnt){
	(void)args;
	if(acnt != 0){
		printf("%s", BAD_ARGS);
	}
	exit(0);
}
