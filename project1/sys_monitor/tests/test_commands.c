/**
 * File test_commands.c
 * Author: Ryan Lanciloti
 * Date of Creation: 2/14/2023
 * Description: This file will hold all of the unit tests for the commands.c file.
*/
#ifndef _UNIT_TEST
	#define _UNIT_TEST
#endif //_UNIT_TEST
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport_c.h"
#include "clnt_commands.h"
#include "sys_monitor.h"

CLIENT* CLNT_CREATE_RET_VAL;

CLIENT *clnt_create(const char *__host, const u_long __prog, const u_long __vers, const char *__prot){
	mock_c()->actualCall("clnt_create")
			->withStringParameters("__host", __host)
			->withUnsignedLongIntParameters("__prog", __prog)
			->withUnsignedLongIntParameters("__vers", __vers)
			->withStringParameters("__prot", __prot);
	return mock_c()->returnValue().value.pointerValue;
}

void _setup_servers(uint8_t count){
	if(!count) return;
	if(count > MAX_SERVER_CNT) count = MAX_SERVER_CNT;
	int i;
	for(i = 0; i < count; i++){
		memset(SERVERS[i].name, 0, sizeof(char)*MAX_SERVER_NAME_LEN);
		sprintf(SERVERS[i].name, "Server %d", i+1);
		SERVERS[i].clnt = (CLIENT*) 1;
	}
	SERVER_CNT = count;
	CURRENT_SERVER_INDEX = 1;
}

/* CPPUTESTS */
TEST_GROUP_C_SETUP(clnt_commands_tests){
	CLNT_CREATE_RET_VAL = nullptr;
	SERVER_CNT = 0;
	CURRENT_SERVER_INDEX = 0;
	CLEAR_BUFFER(PRINTF_TEST_BUFFER)
}

TEST_GROUP_C_TEARDOWN(clnt_commands_tests){
	mock_c()->checkExpectations();
	mock_c()->clear();
}

/* server_list tests*/
TEST_C(clnt_commands_tests, server_list_no_items){
	server_list(nullptr, 0);
	CHECK_EQUAL_C_STRING(LIST_NO_SERVERS, PRINTF_TEST_BUFFER);
}

TEST_C(clnt_commands_tests, server_list_multiple_items){
	_setup_servers(4);
	char* expected_output = "Current Servers:\n\t1 - Server 1*\n\t2 - Server 2\n"
							"\t3 - Server 3\n\t4 - Server 4\n\n";
	server_list(nullptr, 0);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
}

/* server_add tests*/
TEST_C(clnt_commands_tests, server_add_incorrect_number_of_parameters){
	server_add(nullptr, 0);
	CHECK_EQUAL_C_STRING(ADD_BAD_ARGS, PRINTF_TEST_BUFFER);

	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	server_add(nullptr, 2);
	CHECK_EQUAL_C_STRING(ADD_BAD_ARGS, PRINTF_TEST_BUFFER);
	CHECK_EQUAL_C_INT(SERVER_CNT, 0);
}

TEST_C(clnt_commands_tests, server_add_max_capacity){
	_setup_servers(MAX_SERVER_CNT);
	char cmd_param[1][MAX_COMMAND_LINE_LEN] = {"localhost"}; 
	
	server_add((char**)cmd_param[0], 1);
	CHECK_EQUAL_C_STRING(SVR_CAP_MAX, PRINTF_TEST_BUFFER);
	CHECK_EQUAL_C_INT(SERVER_CNT, 10);
}

TEST_C(clnt_commands_tests, server_add_server_doesnt_exists){
	char* expected_output = "Provided server \'localhost\' does not exist.\n";
	char cmd_param[1][MAX_COMMAND_LINE_LEN] = {"localhost"}; 

	mock_c()->expectOneCall("clnt_create")
			->withStringParameters("__host", "localhost")
			->withUnsignedLongIntParameters("__prog", SYS_MONITOR)
			->withUnsignedLongIntParameters("__vers", SYS_MONITOR_VERSION)
			->withStringParameters("__prot", "udp")
			->andReturnPointerValue(NULL);

	server_add((char**)cmd_param[0], 1);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
	CHECK_EQUAL_C_INT(SERVER_CNT, 0);
}

TEST_C(clnt_commands_tests, server_add_server_exists){
	char* expected_output = "Current Servers:\n\t1 - localhost*\n\n";
	char cmd_param[1][MAX_COMMAND_LINE_LEN] = {"localhost"}; 
	CLNT_CREATE_RET_VAL = 0;

	mock_c()->expectOneCall("clnt_create")
			->withStringParameters("__host", "localhost")
			->withUnsignedLongIntParameters("__prog", SYS_MONITOR)
			->withUnsignedLongIntParameters("__vers", SYS_MONITOR_VERSION)
			->withStringParameters("__prot", "udp")
			->andReturnPointerValue((CLIENT*) 1);

	server_add((char**)cmd_param[0], 1);
	server_list(nullptr, 0);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
	CHECK_EQUAL_C_INT(SERVER_CNT, 1);
}

TEST_C(clnt_commands_tests, server_add_server_exists_multiple){
	char* expected_output = "Current Servers:\n\t1 - localhost*\n\t2 - localhost\n"
							"\t3 - localhost\n\t4 - localhost\n\n";
	char cmd_param[1][MAX_COMMAND_LINE_LEN] = {"localhost"}; 
	CLNT_CREATE_RET_VAL = 0;

	mock_c()->expectNCalls(4, "clnt_create")
			->withStringParameters("__host", "localhost")
			->withUnsignedLongIntParameters("__prog", SYS_MONITOR)
			->withUnsignedLongIntParameters("__vers", SYS_MONITOR_VERSION)
			->withStringParameters("__prot", "udp")
			->andReturnPointerValue((CLIENT*) 1);

	server_add((char**)cmd_param[0], 1);
	server_add((char**)cmd_param[0], 1);
	server_add((char**)cmd_param[0], 1);
	server_add((char**)cmd_param[0], 1);
	server_list(nullptr, 0);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
	CHECK_EQUAL_C_INT(SERVER_CNT, 4);
}

/* server_remove tests */
TEST_C(clnt_commands_tests, server_remove_incorrect_number_of_parameters){
	server_remove(nullptr, 0);
	CHECK_EQUAL_C_STRING(RMV_BAD_ARGS, PRINTF_TEST_BUFFER);

	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	server_remove(nullptr, 2);
	CHECK_EQUAL_C_STRING(RMV_BAD_ARGS, PRINTF_TEST_BUFFER);
}

TEST_C(clnt_commands_tests, server_remove_no_servers){
	server_remove(nullptr, 1);
	CHECK_EQUAL_C_STRING(RMV_NO_SERVERS, PRINTF_TEST_BUFFER);
	CHECK_EQUAL_C_INT(SERVER_CNT, 0);
}

TEST_C(clnt_commands_tests, server_remove_invalid_index){
	_setup_servers(5);

	char cmd_param[1][MAX_COMMAND_LINE_LEN] = {"0"}; 
	server_remove((char**)(cmd_param[0]), 1);
	CHECK_EQUAL_C_STRING(RMV_INVALID_INDEX, PRINTF_TEST_BUFFER);
	CHECK_EQUAL_C_INT(SERVER_CNT, 5);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	strcpy(cmd_param[0], "7"); 
	server_remove((char**)(cmd_param[0]), 1);
	CHECK_EQUAL_C_STRING(RMV_INVALID_INDEX, PRINTF_TEST_BUFFER);
	CHECK_EQUAL_C_INT(SERVER_CNT, 5);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	strcpy(cmd_param[0], "-4");
	server_remove((char**)(cmd_param[0]), 1);
	CHECK_EQUAL_C_STRING(RMV_INVALID_INDEX, PRINTF_TEST_BUFFER);
	CHECK_EQUAL_C_INT(SERVER_CNT, 5);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	strcpy(cmd_param[0], "wr");
	server_remove((char**)(cmd_param[0]), 1);
	CHECK_EQUAL_C_STRING(RMV_INVALID_INDEX, PRINTF_TEST_BUFFER);
	CHECK_EQUAL_C_INT(SERVER_CNT, 5);
}

TEST_C(clnt_commands_tests, server_remove_valid_entry_single){
	_setup_servers(1);
	char* expected_output = "Current Servers:\n\t1 - Server 1*\n\n";
	server_list(nullptr, 0);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
	CHECK_EQUAL_C_INT(SERVER_CNT, 1);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	char cmd_param[1][MAX_COMMAND_LINE_LEN] = {"1"};
	server_remove((char**)(cmd_param[0]), 1);
	server_list(nullptr, 0);
	CHECK_EQUAL_C_STRING(LIST_NO_SERVERS, PRINTF_TEST_BUFFER);
	CHECK_EQUAL_C_INT(SERVER_CNT, 0);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	server_remove((char**)(cmd_param[0]), 1);
	CHECK_EQUAL_C_STRING(RMV_NO_SERVERS, PRINTF_TEST_BUFFER);
	CHECK_EQUAL_C_INT(SERVER_CNT, 0);
}

TEST_C(clnt_commands_tests, server_remove_selected_server1){
	_setup_servers(2);
	char expected_output[50] = "Current Servers:\n\t1 - Server 1*\n\t2 - Server 2\n\n";
	server_list(nullptr, 0);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
	CHECK_EQUAL_C_INT(SERVER_CNT, 2);
	CHECK_EQUAL_C_INT(CURRENT_SERVER_INDEX, 1);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	char cmd_param[1][MAX_COMMAND_LINE_LEN] = {"1"};
	strcpy(expected_output, "Current Servers:\n\t1 - Server 2\n\n");
	server_remove((char**)(cmd_param[0]), 1);
	server_list(nullptr, 0);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
	CHECK_EQUAL_C_INT(SERVER_CNT, 1);
	CHECK_EQUAL_C_INT(CURRENT_SERVER_INDEX, 0);
}

TEST_C(clnt_commands_tests, server_remove_selected_server2){
	_setup_servers(2);
	char expected_output[50] = "Current Servers:\n\t1 - Server 1*\n\t2 - Server 2\n\n";
	server_list(nullptr, 0);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
	CHECK_EQUAL_C_INT(SERVER_CNT, 2);
	CHECK_EQUAL_C_INT(CURRENT_SERVER_INDEX, 1);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	char cmd_param[1][MAX_COMMAND_LINE_LEN] = {"2"};
	strcpy(expected_output, "Current Servers:\n\t1 - Server 1*\n\n");
	server_remove((char**)(cmd_param[0]), 1);
	server_list(nullptr, 0);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
	CHECK_EQUAL_C_INT(SERVER_CNT, 1);
	CHECK_EQUAL_C_INT(CURRENT_SERVER_INDEX, 1);
}

TEST_C(clnt_commands_tests, server_remove_selected_server3){
	_setup_servers(10);
	char expected_output[500] = "Current Servers:\n\t1 - Server 1*\n\t2 - Server 2\n"
							   "\t3 - Server 3\n\t4 - Server 4\n\t5 - Server 5\n"
							   "\t6 - Server 6\n\t7 - Server 7\n\t8 - Server 8\n"
							   "\t9 - Server 9\n\t10 - Server 10\n\n";
	server_list(nullptr, 0);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
	CHECK_EQUAL_C_INT(SERVER_CNT, 10);
	CHECK_EQUAL_C_INT(CURRENT_SERVER_INDEX, 1);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	char cmd_param[1][MAX_COMMAND_LINE_LEN] = {"2"};
	strcpy(expected_output, "Current Servers:\n\t1 - Server 1*\n"
							   "\t2 - Server 3\n\t3 - Server 5\n"
							   "\t4 - Server 7\n\t5 - Server 9\n\n");
	server_remove((char**)(cmd_param[0]), 1);
	
	strcpy(cmd_param[0], "3");
	server_remove((char**)(cmd_param[0]), 1);
	
	strcpy(cmd_param[0], "4");
	server_remove((char**)(cmd_param[0]), 1);
	
	strcpy(cmd_param[0], "5");
	server_remove((char**)(cmd_param[0]), 1);

	strcpy(cmd_param[0], "6");
	server_remove((char**)(cmd_param[0]), 1);


	server_list(nullptr, 0);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
	CHECK_EQUAL_C_INT(SERVER_CNT, 5);
	CHECK_EQUAL_C_INT(CURRENT_SERVER_INDEX, 1);
}

/* server_select tests */
TEST_C(clnt_commands_tests, server_select_incorrect_number_of_parameters){
	server_select(nullptr, 0);
	CHECK_EQUAL_C_STRING(SEL_BAD_ARGS, PRINTF_TEST_BUFFER);

	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	server_select(nullptr, 2);
	CHECK_EQUAL_C_STRING(SEL_BAD_ARGS, PRINTF_TEST_BUFFER);
}

TEST_C(clnt_commands_tests, server_select_bad_index){
	_setup_servers(5);
	char cmd_param[1][MAX_COMMAND_LINE_LEN] = {"0"}; 
	server_select((char**)(cmd_param[0]), 1);
	CHECK_EQUAL_C_STRING(SEL_INVALID_INDEX, PRINTF_TEST_BUFFER);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	strcpy(cmd_param[0], "7"); 
	server_select((char**)(cmd_param[0]), 1);
	CHECK_EQUAL_C_STRING(SEL_INVALID_INDEX, PRINTF_TEST_BUFFER);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	strcpy(cmd_param[0], "-4");
	server_select((char**)(cmd_param[0]), 1);
	CHECK_EQUAL_C_STRING(SEL_INVALID_INDEX, PRINTF_TEST_BUFFER);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	strcpy(cmd_param[0], "wr");
	server_select((char**)(cmd_param[0]), 1);
	CHECK_EQUAL_C_STRING(SEL_INVALID_INDEX, PRINTF_TEST_BUFFER);
}

TEST_C(clnt_commands_tests, server_select_valid_index){
	_setup_servers(5);
	char expected_output[500] = "Current Servers:\n\t1 - Server 1*\n\t2 - Server 2\n"
							   "\t3 - Server 3\n\t4 - Server 4\n\t5 - Server 5\n\n";
	CHECK_EQUAL_C_INT(1, CURRENT_SERVER_INDEX);
	server_list(nullptr, 0);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	char cmd_param[1][MAX_COMMAND_LINE_LEN] = {"2"}; 
	strcpy(expected_output, "Current Servers:\n\t1 - Server 1\n\t2 - Server 2*\n"
							   "\t3 - Server 3\n\t4 - Server 4\n\t5 - Server 5\n\n");
	server_select((char**)(cmd_param[0]), 1);
	CHECK_EQUAL_C_INT(2, CURRENT_SERVER_INDEX);
	server_list(nullptr, 0);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	strcpy(cmd_param[0], "2"); 
	strcpy(expected_output, "Current Servers:\n\t1 - Server 1\n\t2 - Server 2*\n"
							   "\t3 - Server 3\n\t4 - Server 4\n\t5 - Server 5\n\n");
	server_select((char**)(cmd_param[0]), 1);
	CHECK_EQUAL_C_INT(2, CURRENT_SERVER_INDEX);
	server_list(nullptr, 0);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	strcpy(cmd_param[0], "5"); 
	strcpy(expected_output, "Current Servers:\n\t1 - Server 1\n\t2 - Server 2\n"
							   "\t3 - Server 3\n\t4 - Server 4\n\t5 - Server 5*\n\n");
	server_select((char**)(cmd_param[0]), 1);
	CHECK_EQUAL_C_INT(5, CURRENT_SERVER_INDEX);
	server_list(nullptr, 0);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);
}
