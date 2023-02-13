/**
 * File: test_main.cpp
 * Author: Ryan Lanciloti
 * Date of Creation: 2/12/2023
 * Description: This file will act as the glue between the C unit tests and the CPP 
 * 	CppUTest framework.
*/
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness_c.h"

TEST_GROUP_C_WRAPPER(console_helper_tests){
	TEST_GROUP_C_SETUP_WRAPPER(console_helper_tests);
	TEST_GROUP_C_TEARDOWN_WRAPPER(console_helper_tests);
};

TEST_C_WRAPPER(console_helper_tests, console_init_null_params);

TEST_C_WRAPPER(console_helper_tests, print_intro_null)
TEST_C_WRAPPER(console_helper_tests, print_intro_valid)

TEST_C_WRAPPER(console_helper_tests, ch_execute_nullptr_arg)
TEST_C_WRAPPER(console_helper_tests, ch_execute_nullptr_menu_items)
TEST_C_WRAPPER(console_helper_tests, ch_execute_cmd_doesnt_exist)
TEST_C_WRAPPER(console_helper_tests, ch_execute_cmd_doesnt_do_anything)
TEST_C_WRAPPER(console_helper_tests, ch_execute_cmd_submenu_cmd_no_param)
TEST_C_WRAPPER(console_helper_tests, ch_execute_cmd_submenu_cmd_with_param)
TEST_C_WRAPPER(console_helper_tests, ch_execute_cmd_action_no_submenu)
TEST_C_WRAPPER(console_helper_tests, ch_execute_cmd_add_1)
TEST_C_WRAPPER(console_helper_tests, ch_execute_cmd_add_2)
TEST_C_WRAPPER(console_helper_tests, ch_execute_cmd_add_3)

TEST_C_WRAPPER(console_helper_tests, ch_reset_verify)

int main(int argc, char** argv){
	return RUN_ALL_TESTS(argc, argv);
}
