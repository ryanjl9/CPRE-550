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

TEST_GROUP_C_WRAPPER(mock_functions_tests){
	TEST_GROUP_C_SETUP_WRAPPER(mock_functions_tests);
	TEST_GROUP_C_TEARDOWN_WRAPPER(mock_functions_tests);
};

TEST_GROUP_C_WRAPPER(clnt_commands_tests){
	TEST_GROUP_C_SETUP_WRAPPER(clnt_commands_tests);
	TEST_GROUP_C_TEARDOWN_WRAPPER(clnt_commands_tests);
};

/* console_helper_tests */
TEST_C_WRAPPER(console_helper_tests, console_init_null_params);
TEST_C_WRAPPER(console_helper_tests, ch_init_oversized_msg);

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
TEST_C_WRAPPER(console_helper_tests, ch_help_no_cmds)
TEST_C_WRAPPER(console_helper_tests, ch_help_no_args)
TEST_C_WRAPPER(console_helper_tests, ch_help_show_subcommands)
TEST_C_WRAPPER(console_helper_tests, ch_help_show_subcommands_no_subcommand)
TEST_C_WRAPPER(console_helper_tests, ch_help_bad_command)
TEST_C_WRAPPER(console_helper_tests, ch_quit_bad_args)
TEST_C_WRAPPER(console_helper_tests, ch_quit_valid)

/* mock_function_tests */
TEST_C_WRAPPER(mock_functions_tests, scanf_one_argument)
TEST_C_WRAPPER(mock_functions_tests, scanf_one_multiple_arguments)

TEST_C_WRAPPER(mock_functions_tests, printf_mock_test_noargs)
TEST_C_WRAPPER(mock_functions_tests, printf_mock_test_args_multiple_lines)

/* clnt_commands_tests */
TEST_C_WRAPPER(clnt_commands_tests, server_list_no_items)
TEST_C_WRAPPER(clnt_commands_tests, server_list_multiple_items)

TEST_C_WRAPPER(clnt_commands_tests, server_add_incorrect_number_of_parameters)
TEST_C_WRAPPER(clnt_commands_tests, server_add_max_capacity)
TEST_C_WRAPPER(clnt_commands_tests, server_add_server_doesnt_exists)
TEST_C_WRAPPER(clnt_commands_tests, server_add_server_exists)

TEST_C_WRAPPER(clnt_commands_tests, server_remove_incorrect_number_of_parameters)
TEST_C_WRAPPER(clnt_commands_tests, server_remove_no_servers)
TEST_C_WRAPPER(clnt_commands_tests, server_remove_invalid_index)
TEST_C_WRAPPER(clnt_commands_tests, server_remove_valid_entry_single)
TEST_C_WRAPPER(clnt_commands_tests, server_remove_selected_server1)
TEST_C_WRAPPER(clnt_commands_tests, server_remove_selected_server2)
TEST_C_WRAPPER(clnt_commands_tests, server_remove_selected_server3)

TEST_C_WRAPPER(clnt_commands_tests, server_select_incorrect_number_of_parameters)
TEST_C_WRAPPER(clnt_commands_tests, server_select_bad_index)
TEST_C_WRAPPER(clnt_commands_tests, server_select_valid_index)

TEST_C_WRAPPER(clnt_commands_tests, get_datetime_no_params)
TEST_C_WRAPPER(clnt_commands_tests, get_datetime_no_server_selected)
TEST_C_WRAPPER(clnt_commands_tests, get_datetime_invalid_arg)
TEST_C_WRAPPER(clnt_commands_tests, get_datetime_date)

TEST_C_WRAPPER(clnt_commands_tests, get_systeminfo_bad_params)
TEST_C_WRAPPER(clnt_commands_tests, get_systeminfo_no_server_selected)
TEST_C_WRAPPER(clnt_commands_tests, get_systeminfo_valid)
TEST_C_WRAPPER(clnt_commands_tests, get_memoryutilization_no_params)
TEST_C_WRAPPER(clnt_commands_tests, get_memoryutilization_no_server_selected)
TEST_C_WRAPPER(clnt_commands_tests, get_memoryutilization_valid)

TEST_C_WRAPPER(clnt_commands_tests, get_loadinfo_bad_param)
TEST_C_WRAPPER(clnt_commands_tests, get_loadinfo_no_server)
TEST_C_WRAPPER(clnt_commands_tests, get_loadinfo_valid)

TEST_C_WRAPPER(clnt_commands_tests, get_usernames_bad_param)
TEST_C_WRAPPER(clnt_commands_tests, get_usernames_no_server)
TEST_C_WRAPPER(clnt_commands_tests, get_usernames_valid)

int main(int argc, char** argv){
	return RUN_ALL_TESTS(argc, argv);
}
