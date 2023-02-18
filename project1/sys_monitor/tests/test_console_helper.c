/**
 * File: test_console_helper.c
 * Author: Ryan Lanciloti
 * Date of Creation: 2/12/2023
 * Description: This file will hold all of the unit tests for the console_helper.c file.
*/
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport_c.h"
#include "proj_config.h"
#include "console_helper.h"

/* PRIVATE FUNCTION HEADERS*/
void dummy_func1(char* args[MAX_COMMAND_LINE_LEN], int acnt);
void dummy_func2(char* args[MAX_COMMAND_LINE_LEN], int acnt);
void dummy_func3(char* args[MAX_COMMAND_LINE_LEN], int acnt);

int DUMMY_FUNC1_STATE = 0;
int DUMMY_FUNC2_STATE = 0;
int DUMMY_FUNC3_STATE = 0;

/**
 * @brief: Sets a state variable to a known value
 * 
 * PARAMS: args - Array containing arguments for the the function
 * PARAMS: acnt - Arguments count 
*/
#define DUMMY_FUNC1_TEST_VAL 14
void dummy_func1(char* args[MAX_COMMAND_LINE_LEN], int acnt){
	(void)args;
	(void)acnt;
	DUMMY_FUNC1_STATE = DUMMY_FUNC1_TEST_VAL;
}

/**
 * @brief: Sets a state variable to a passed in parameter
 * 
 * PARAMS: args - Array containing arguments for the the function
 * PARAMS: acnt - Arguments count
 * 
 * EXAMPLE: dummy_func2 7' sets DUMMY_FUNC2_STATE = 7
*/
void dummy_func2(char* args[MAX_COMMAND_LINE_LEN], int acnt){
	if(acnt < 1) return;
	int val = atoi((char*)args);
	DUMMY_FUNC2_STATE = val;
}

/**
 * @brief: Sets a state variable to the sum off all parameters
 * 
 * PARAMS: args - Array containing arguments for the the function
 * PARAMS: acnt - Arguments count 
 * 
 * EXAMPLE: dummy_func3 1 2 3' sets DUMMY_FUNC3_STATE = 6
*/
void dummy_func3(char* args[MAX_COMMAND_LINE_LEN], int acnt){
	if(acnt < 1) return;
	
	int i;
	for(i = 0; i < acnt; i++){
		DUMMY_FUNC3_STATE += atoi(GET_PARAM_X(args, i));
	}
}

typedef enum {
	testing_subcmds_subentry1 = 0,
	testing_subcmds_subentry2,
	testing_subcmds_cnt
} testing_subcmds_t;

typedef enum {
	testing_cmds_entry1 = 0,
	testing_cmds_entry2,
	testing_cmds_entry3,
	testing_cmds_add,
	_testing_cmds_cnt
} testing_cmds_t;

menu_item_t _SUBMENU[testing_subcmds_cnt] = {
	{"subentry1", "Subentry Command 1", (void*)&dummy_func1, nullptr, 0},
	{"subentry2", "Subentry Command 2", (void*)&dummy_func2, nullptr, 0}
};

menu_item_t _TESTING_ITEMS[_testing_cmds_cnt] = {
	{"entry1", "Test Command 1", nullptr, nullptr, 0},
	{"entry2", "Test Command 2", nullptr, _SUBMENU, testing_subcmds_cnt},
	{"entry3", "Test Command 3", (void*)&dummy_func2, nullptr, 0},
	{"add", "Adds n numbers together `add 4 5`", (void*)&dummy_func3, nullptr, 0}
};

/* CPPUTESTS */
TEST_GROUP_C_SETUP(console_helper_tests){
	ch_reset();
	DUMMY_FUNC1_STATE = 0;
	DUMMY_FUNC2_STATE = 0;
	DUMMY_FUNC3_STATE = 0;
	EXIT_STATUS = -1;
	CLEAR_BUFFER(PRINTF_TEST_BUFFER)
}

TEST_GROUP_C_TEARDOWN(console_helper_tests){
	mock_c()->checkExpectations();
	mock_c()->clear();
}

/* ch_init tests */
TEST_C(console_helper_tests, console_init_null_params){
	menu_item_t test_menu_item = {"NAME", "HELP", nullptr, nullptr, 0};
	uint8_t retval = ch_init(nullptr, nullptr, 0);
	CHECK_EQUAL_C_INT(retval, CH_INIT_ITEM_CNT_MENU_ITEMS_INTRO_MSG_NULL);
	ch_reset();

	char name[] = "TEST";
	retval = ch_init(name, nullptr, 0);
	CHECK_EQUAL_C_INT(retval, CH_INIT_ITEM_CNT_MENU_ITEMS_ITEMS_NULL);
	ch_reset();

	retval = ch_init(nullptr, &test_menu_item, _testing_cmds_cnt);
	CHECK_EQUAL_C_INT(retval, CH_INIT_INTRO_MSG_NULL);
	ch_reset();

	retval = ch_init(name, &test_menu_item, _testing_cmds_cnt);
	CHECK_EQUAL_C_INT(retval, CH_INIT_NO_ERROR);
	ch_reset();
}

TEST_C(console_helper_tests, ch_init_oversized_msg){
	menu_item_t test_menu_item = {"NAME", "HELP", nullptr, nullptr, 0};
	char test_msg[1025];
	memset(test_msg, 0x41, 1024);
	int retval = ch_init(test_msg, &test_menu_item, _testing_cmds_cnt);
	CHECK_EQUAL_C_INT(retval, CH_INIT_BAD_INTRO_MSG);
}

/* ch_print_introduction tests */
TEST_C(console_helper_tests, print_intro_null){
	uint8_t retval = ch_print_introduction();
	CHECK_EQUAL_C_INT(retval, CH_PRINT_INTRO_MSG_NULL);
	CHECK_EQUAL_C_STRING(PRINTF_TEST_BUFFER, NULL_INTRO_MESSAGE);
}

TEST_C(console_helper_tests, print_intro_valid){
	strcpy(INTRO_MSG, "Test");
	uint8_t retval = ch_print_introduction();
	CHECK_EQUAL_C_INT(retval, CH_PRINT_INTRO_NO_ERROR);
	CHECK_EQUAL_C_STRING(
		PRINTF_TEST_BUFFER, 
		"Test"
	);
}

/* ch_execute tests*/
TEST_C(console_helper_tests, ch_execute_nullptr_arg){
	uint8_t retval = ch_execute(nullptr);
	CHECK_EQUAL_C_INT(retval, CH_EXECUTE_CMD_NULLPTR);
}

TEST_C(console_helper_tests, ch_execute_nullptr_menu_items){
	char cmd[] = "help";
	uint8_t retval = ch_execute(cmd);
	CHECK_EQUAL_C_INT(retval, CH_EXECUTE_MENU_NULLPTR);
}

TEST_C(console_helper_tests, ch_execute_cmd_doesnt_exist){
	char name[] = "TEST";
	uint8_t retval = ch_init(name, _TESTING_ITEMS, _testing_cmds_cnt);
	CHECK_EQUAL_C_INT(retval, CH_INIT_NO_ERROR);

	char cmd[] = "help";
	retval = ch_execute(cmd);
	CHECK_EQUAL_C_INT(retval, CH_EXECUTE_CMD_DOESNT_EXIST);
}

TEST_C(console_helper_tests, ch_execute_cmd_doesnt_do_anything){
	char name[] = "TEST";
	uint8_t retval = ch_init(name, _TESTING_ITEMS, _testing_cmds_cnt);
	CHECK_EQUAL_C_INT(retval, CH_INIT_NO_ERROR);

	char cmd[] = "entry1";
	retval = ch_execute(cmd);
	CHECK_EQUAL_C_INT(retval, CH_EXECUTE_NO_ERROR);
}

TEST_C(console_helper_tests, ch_execute_cmd_submenu_cmd_no_param){
	char name[] = "TEST";
	uint8_t retval = ch_init(name, _TESTING_ITEMS, _testing_cmds_cnt);
	CHECK_EQUAL_C_INT(retval, CH_INIT_NO_ERROR);

	char cmd[] = "entry2 subentry1";
	retval = ch_execute(cmd);
	CHECK_EQUAL_C_INT(retval, CH_EXECUTE_NO_ERROR);
	CHECK_EQUAL_C_INT(DUMMY_FUNC1_STATE, DUMMY_FUNC1_TEST_VAL);
}

TEST_C(console_helper_tests, ch_execute_cmd_submenu_cmd_with_param){
	char name[] = "TEST";
	uint8_t retval = ch_init(name, _TESTING_ITEMS, _testing_cmds_cnt);
	CHECK_EQUAL_C_INT(retval, CH_INIT_NO_ERROR);

	char cmd[] = "entry2 subentry2 42";
	retval = ch_execute(cmd);
	CHECK_EQUAL_C_INT(retval, CH_EXECUTE_NO_ERROR);
	CHECK_EQUAL_C_INT(DUMMY_FUNC2_STATE, 42);
}

TEST_C(console_helper_tests, ch_execute_cmd_action_no_submenu){
	char name[] = "TEST";
	uint8_t retval = ch_init(name, _TESTING_ITEMS, _testing_cmds_cnt);
	CHECK_EQUAL_C_INT(retval, CH_INIT_NO_ERROR);

	char cmd[] = "entry3 83";
	retval = ch_execute(cmd);
	CHECK_EQUAL_C_INT(retval, CH_EXECUTE_NO_ERROR);
	CHECK_EQUAL_C_INT(DUMMY_FUNC2_STATE, 83);
}

TEST_C(console_helper_tests, ch_execute_cmd_add_1){
	char name[] = "TEST";
	uint8_t retval = ch_init(name, _TESTING_ITEMS, _testing_cmds_cnt);
	CHECK_EQUAL_C_INT(retval, CH_INIT_NO_ERROR);

	char cmd[] = "add 1";
	retval = ch_execute(cmd);
	CHECK_EQUAL_C_INT(retval, CH_EXECUTE_NO_ERROR);
	CHECK_EQUAL_C_INT(DUMMY_FUNC3_STATE, 1);
}

TEST_C(console_helper_tests, ch_execute_cmd_add_2){
	char name[] = "TEST";
	uint8_t retval = ch_init(name, _TESTING_ITEMS, _testing_cmds_cnt);
	CHECK_EQUAL_C_INT(retval, CH_INIT_NO_ERROR);

	char cmd[] = "add 1 3";
	retval = ch_execute(cmd);
	CHECK_EQUAL_C_INT(retval, CH_EXECUTE_NO_ERROR);
	CHECK_EQUAL_C_INT(DUMMY_FUNC3_STATE, 4);
}

TEST_C(console_helper_tests, ch_execute_cmd_add_3){
	char name[] = "TEST";
	uint8_t retval = ch_init(name, _TESTING_ITEMS, _testing_cmds_cnt);
	CHECK_EQUAL_C_INT(retval, CH_INIT_NO_ERROR);

	char cmd[] = "add 1 3 5 7";
	retval = ch_execute(cmd);
	CHECK_EQUAL_C_INT(retval, CH_EXECUTE_NO_ERROR);
	CHECK_EQUAL_C_INT(DUMMY_FUNC3_STATE, 16);
}

/* ch_reset tests */
TEST_C(console_helper_tests, ch_reset_verify){
	strcpy(INTRO_MSG,"Test");
	MENU_ITEMS = _TESTING_ITEMS;

	CHECK_EQUAL_C_STRING(INTRO_MSG, "Test");
	CHECK_EQUAL_C_POINTER(MENU_ITEMS, _TESTING_ITEMS);
	ch_reset();

	CHECK_EQUAL_C_INT(INTRO_MSG[0], 0);
	CHECK_EQUAL_C_POINTER(MENU_ITEMS, nullptr);
}

/* ch_help tests*/
TEST_C(console_helper_tests, ch_help_no_cmds){
	ch_help(nullptr, 0);
	CHECK_EQUAL_C_STRING(NO_MENU_ITEMS, PRINTF_TEST_BUFFER);
}

TEST_C(console_helper_tests, ch_help_no_args){
	char name[] = "TEST";
	uint8_t retval = ch_init(name, _TESTING_ITEMS, _testing_cmds_cnt);
	const char* expected_output = "entry1 - Test Command 1\nentry2* - Test Command 2\n"
								  "entry3 - Test Command 3\n"
								  "add - Adds n numbers together `add 4 5`\n\n";
	MENU_ITEMS = _TESTING_ITEMS;
	ch_help(nullptr, 0);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
}

TEST_C(console_helper_tests, ch_help_show_subcommands){
	char name[] = "TEST";
	uint8_t retval = ch_init(name, _TESTING_ITEMS, _testing_cmds_cnt);
	
	const char* expected_output = "subentry1 - Subentry Command 1\n"
								  "subentry2 - Subentry Command 2\n\n";
	char cmd_param[1][MAX_COMMAND_LINE_LEN] = {"entry2"};
	
	ch_help((char**)(&cmd_param[0]), 1);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
}

TEST_C(console_helper_tests, ch_help_show_subcommands_no_subcommand){
	char name[] = "TEST";
	uint8_t retval = ch_init(name, _TESTING_ITEMS, _testing_cmds_cnt);
	
	char cmd_param[1][MAX_COMMAND_LINE_LEN] = {"entry1"};
	
	ch_help((char**)(&cmd_param[0]), 1);
	CHECK_EQUAL_C_STRING(NO_SUB_COMMND, PRINTF_TEST_BUFFER);
}

TEST_C(console_helper_tests, ch_help_bad_command){
	char name[] = "TEST";
	uint8_t retval = ch_init(name, _TESTING_ITEMS, _testing_cmds_cnt);
	
	char cmd_param[1][MAX_COMMAND_LINE_LEN] = {"entry4"};
	
	ch_help((char**)(&cmd_param[0]), 1);
	CHECK_EQUAL_C_STRING(BAD_COMMAND, PRINTF_TEST_BUFFER);
}

/* ch_quit tests*/
TEST_C(console_helper_tests, ch_quit_bad_args){
	ch_quit(nullptr, 1);
	CHECK_EQUAL_C_STRING(BAD_ARGS, PRINTF_TEST_BUFFER);
}

/* ch_quit tests*/
TEST_C(console_helper_tests, ch_quit_valid){
	ch_quit(nullptr, 0);
	CHECK_EQUAL_C_INT(0, EXIT_STATUS);
}
