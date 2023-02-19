/**
 * File test_commands.c
 * Author: Ryan Lanciloti
 * Date of Creation: 2/14/2023
 * Description: This file will hold all of the unit tests for the commands.c file.
*/
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport_c.h"
#include "console_helper.h"
#include "clnt_commands.h"
#include "sys_monitor.h"

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
	CUR_SVR_INDEX = 1;
}

/* CPPUTESTS */
TEST_GROUP_C_SETUP(clnt_commands_tests){
	SERVER_CNT = 0;
	CUR_SVR_INDEX = 0;
	memset(GET_DATETIME_MOCK_RETVAL, 0, sizeof(char) * GETDATETIME_MOCK_RETVAL_SIZE);
	memset(&GET_SYSTEM_INFO_MOCK_RETVAL, 0, sizeof(struct sysinfo_c));
	memset(&GET_MEMORY_UTILIZATION_MOCK_RETVAL, 0, sizeof(struct mem_usage_ret));
	memset(&GET_LOAD_PROCS_PER_MINUTE_MOCK_RETVAL, 0, sizeof(double));
	CLEAR_BUFFER(PRINTF_TEST_BUFFER)
}

TEST_GROUP_C_TEARDOWN(clnt_commands_tests){
	mock_c()->checkExpectations();
	mock_c()->clear();
	mock_c()->removeAllComparatorsAndCopiers();
}

/* server_list tests*/
TEST_C(clnt_commands_tests, server_list_no_items){
	server_list(nullptr, 0);
	CHECK_EQUAL_C_STRING(LIST_NO_SERVERS, PRINTF_TEST_BUFFER);
}

TEST_C(clnt_commands_tests, server_list_multiple_items){
	_setup_servers(4);
	const char* expected_output = "Current Servers:\n\t1 - Server 1*\n\t2 - Server 2\n"
								  "\t3 - Server 3\n\t4 - Server 4\n\n";
	server_list(nullptr, 0);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
}

/* server_add tests*/
TEST_C(clnt_commands_tests, server_add_incorrect_number_of_parameters){
	server_add(nullptr, 0);
	CHECK_EQUAL_C_STRING(BAD_ARGS, PRINTF_TEST_BUFFER);

	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	server_add(nullptr, 2);
	CHECK_EQUAL_C_STRING(BAD_ARGS, PRINTF_TEST_BUFFER);
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
	const char* expected_output = "Provided server \'localhost\' does not exist.\n";
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
	const char* expected_output = "Current Servers:\n\t1 - localhost*\n\n";
	char cmd_param[1][MAX_COMMAND_LINE_LEN] = {"localhost"}; 

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
	const char* expected_output = "Current Servers:\n\t1 - localhost*\n\t2 - localhost\n"
								  "\t3 - localhost\n\t4 - localhost\n\n";
	char cmd_param[1][MAX_COMMAND_LINE_LEN] = {"localhost"}; 

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
	CHECK_EQUAL_C_STRING(BAD_ARGS, PRINTF_TEST_BUFFER);

	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	server_remove(nullptr, 2);
	CHECK_EQUAL_C_STRING(BAD_ARGS, PRINTF_TEST_BUFFER);
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
	CHECK_EQUAL_C_STRING(INVALID_INDEX, PRINTF_TEST_BUFFER);
	CHECK_EQUAL_C_INT(SERVER_CNT, 5);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	strcpy(cmd_param[0], "7"); 
	server_remove((char**)(cmd_param[0]), 1);
	CHECK_EQUAL_C_STRING(INVALID_INDEX, PRINTF_TEST_BUFFER);
	CHECK_EQUAL_C_INT(SERVER_CNT, 5);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	strcpy(cmd_param[0], "-4");
	server_remove((char**)(cmd_param[0]), 1);
	CHECK_EQUAL_C_STRING(INVALID_INDEX, PRINTF_TEST_BUFFER);
	CHECK_EQUAL_C_INT(SERVER_CNT, 5);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	strcpy(cmd_param[0], "wr");
	server_remove((char**)(cmd_param[0]), 1);
	CHECK_EQUAL_C_STRING(INVALID_INDEX, PRINTF_TEST_BUFFER);
	CHECK_EQUAL_C_INT(SERVER_CNT, 5);
}

TEST_C(clnt_commands_tests, server_remove_valid_entry_single){
	_setup_servers(1);
	const char* expected_output = "Current Servers:\n\t1 - Server 1*\n\n";
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
	CHECK_EQUAL_C_INT(CUR_SVR_INDEX, 1);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	char cmd_param[1][MAX_COMMAND_LINE_LEN] = {"1"};
	strcpy(expected_output, "Current Servers:\n\t1 - Server 2\n\n");
	server_remove((char**)(cmd_param[0]), 1);
	server_list(nullptr, 0);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
	CHECK_EQUAL_C_INT(SERVER_CNT, 1);
	CHECK_EQUAL_C_INT(CUR_SVR_INDEX, 0);
}

TEST_C(clnt_commands_tests, server_remove_selected_server2){
	_setup_servers(2);
	char expected_output[50] = "Current Servers:\n\t1 - Server 1*\n\t2 - Server 2\n\n";
	server_list(nullptr, 0);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
	CHECK_EQUAL_C_INT(SERVER_CNT, 2);
	CHECK_EQUAL_C_INT(CUR_SVR_INDEX, 1);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	char cmd_param[1][MAX_COMMAND_LINE_LEN] = {"2"};
	strcpy(expected_output, "Current Servers:\n\t1 - Server 1*\n\n");
	server_remove((char**)(cmd_param[0]), 1);
	server_list(nullptr, 0);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
	CHECK_EQUAL_C_INT(SERVER_CNT, 1);
	CHECK_EQUAL_C_INT(CUR_SVR_INDEX, 1);
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
	CHECK_EQUAL_C_INT(CUR_SVR_INDEX, 1);
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
	CHECK_EQUAL_C_INT(CUR_SVR_INDEX, 1);
}

/* server_select tests */
TEST_C(clnt_commands_tests, server_select_incorrect_number_of_parameters){
	server_select(nullptr, 0);
	CHECK_EQUAL_C_STRING(BAD_ARGS, PRINTF_TEST_BUFFER);

	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	server_select(nullptr, 2);
	CHECK_EQUAL_C_STRING(BAD_ARGS, PRINTF_TEST_BUFFER);
}

TEST_C(clnt_commands_tests, server_select_bad_index){
	_setup_servers(5);
	char cmd_param[1][MAX_COMMAND_LINE_LEN] = {"0"}; 
	server_select((char**)(cmd_param[0]), 1);
	CHECK_EQUAL_C_STRING(INVALID_INDEX, PRINTF_TEST_BUFFER);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	strcpy(cmd_param[0], "7"); 
	server_select((char**)(cmd_param[0]), 1);
	CHECK_EQUAL_C_STRING(INVALID_INDEX, PRINTF_TEST_BUFFER);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	strcpy(cmd_param[0], "-4");
	server_select((char**)(cmd_param[0]), 1);
	CHECK_EQUAL_C_STRING(INVALID_INDEX, PRINTF_TEST_BUFFER);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	strcpy(cmd_param[0], "wr");
	server_select((char**)(cmd_param[0]), 1);
	CHECK_EQUAL_C_STRING(INVALID_INDEX, PRINTF_TEST_BUFFER);
}

TEST_C(clnt_commands_tests, server_select_valid_index){
	_setup_servers(5);
	char expected_output[500] = "Current Servers:\n\t1 - Server 1*\n\t2 - Server 2\n"
							   "\t3 - Server 3\n\t4 - Server 4\n\t5 - Server 5\n\n";
	CHECK_EQUAL_C_INT(1, CUR_SVR_INDEX);
	server_list(nullptr, 0);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	char cmd_param[1][MAX_COMMAND_LINE_LEN] = {"2"}; 
	strcpy(expected_output, "Current Servers:\n\t1 - Server 1\n\t2 - Server 2*\n"
							   "\t3 - Server 3\n\t4 - Server 4\n\t5 - Server 5\n\n");
	server_select((char**)(cmd_param[0]), 1);
	CHECK_EQUAL_C_INT(2, CUR_SVR_INDEX);
	server_list(nullptr, 0);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	strcpy(cmd_param[0], "2"); 
	strcpy(expected_output, "Current Servers:\n\t1 - Server 1\n\t2 - Server 2*\n"
							   "\t3 - Server 3\n\t4 - Server 4\n\t5 - Server 5\n\n");
	server_select((char**)(cmd_param[0]), 1);
	CHECK_EQUAL_C_INT(2, CUR_SVR_INDEX);
	server_list(nullptr, 0);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	strcpy(cmd_param[0], "5"); 
	strcpy(expected_output, "Current Servers:\n\t1 - Server 1\n\t2 - Server 2\n"
							   "\t3 - Server 3\n\t4 - Server 4\n\t5 - Server 5*\n\n");
	server_select((char**)(cmd_param[0]), 1);
	CHECK_EQUAL_C_INT(5, CUR_SVR_INDEX);
	server_list(nullptr, 0);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);
}

/* get_datetime tests */
TEST_C(clnt_commands_tests, get_datetime_no_params){
	get_datetime(nullptr, 0);
	CHECK_EQUAL_C_STRING(BAD_ARGS, PRINTF_TEST_BUFFER);
	CLEAR_BUFFER(PRINTF_TEST_BUFFER);

	get_datetime(nullptr, 2);
	CHECK_EQUAL_C_STRING(BAD_ARGS, PRINTF_TEST_BUFFER);
}

TEST_C(clnt_commands_tests, get_datetime_no_server_selected){
	get_datetime(nullptr, 1);
	CHECK_EQUAL_C_STRING(NO_SERVER_SELECTED, PRINTF_TEST_BUFFER);
}

TEST_C(clnt_commands_tests, get_datetime_invalid_arg){
	_setup_servers(1);
	char cmd_param[1][MAX_COMMAND_LINE_LEN] = {"test"};
	get_datetime((char**)(&cmd_param[0]), 1);
	CHECK_EQUAL_C_STRING(DT_INVALID_ARG, PRINTF_TEST_BUFFER);
}

TEST_C(clnt_commands_tests, get_datetime_date){
	const char* expected_output = "Server 1 -> January 1, 2000\n";
	_setup_servers(1);

	strcpy(GET_DATETIME_MOCK_RETVAL, "January 1, 2000");

	char cmd_param[1][MAX_COMMAND_LINE_LEN] = {"date"};
	get_datetime((char**)(&cmd_param[0]), 1);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
}

/* get_systeminfo tests */
TEST_C(clnt_commands_tests, get_systeminfo_bad_params){
	get_systeminfo(nullptr, 1);
	CHECK_EQUAL_C_STRING(BAD_ARGS, PRINTF_TEST_BUFFER);
}

TEST_C(clnt_commands_tests, get_systeminfo_no_server_selected){
	get_systeminfo(nullptr, 0);
	CHECK_EQUAL_C_STRING(NO_SERVER_SELECTED, PRINTF_TEST_BUFFER);
}

TEST_C(clnt_commands_tests, get_systeminfo_valid){
	_setup_servers(1);
	const char* expected_output = "Server 1 -> System Info:\n"
								  "\tUptime (seconds): 9999\n"
								  "\tNumber of Processes: 100\n\n";
	GET_SYSTEM_INFO_MOCK_RETVAL.uptime = 9999;
	GET_SYSTEM_INFO_MOCK_RETVAL.procs = 100;

	get_systeminfo(nullptr, 0);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
}

/* get_memoryutilization tests */
TEST_C(clnt_commands_tests, get_memoryutilization_no_params){
	get_memoryutilization(nullptr, 1);
	CHECK_EQUAL_C_STRING(BAD_ARGS, PRINTF_TEST_BUFFER);
}

TEST_C(clnt_commands_tests, get_memoryutilization_no_server_selected){
	get_memoryutilization(nullptr, 0);
	CHECK_EQUAL_C_STRING(NO_SERVER_SELECTED, PRINTF_TEST_BUFFER);
}

TEST_C(clnt_commands_tests, get_memoryutilization_valid){
	_setup_servers(1);
	const char* expected_output = \
		"Server 1 -> Memory Utilization:\n"
		"\tNumber of Free Chunks: 8\n"
		"\tNumber of fastbin blocks: 13\n"
		"\tNumber of Mapped Regions: 18\n"
		"\tSpace in mapped regions: 16\n"
		"\tSpace in fastbin blocks: 37\n"
		"\tPage size: 1000\n"
		"\tPhysical page count: 4\n"
		"\tAvailable physical page count: 2\n"
		"\tTotal allocated space: 22\n"
		"\tTotal free space: 114\n\n";

	GET_MEMORY_UTILIZATION_MOCK_RETVAL.dynamic_memory_usage.ordblks = 8;
	GET_MEMORY_UTILIZATION_MOCK_RETVAL.dynamic_memory_usage.smblks = 13;
	GET_MEMORY_UTILIZATION_MOCK_RETVAL.dynamic_memory_usage.hblks = 18;
	GET_MEMORY_UTILIZATION_MOCK_RETVAL.dynamic_memory_usage.hblkhd = 16;
	GET_MEMORY_UTILIZATION_MOCK_RETVAL.dynamic_memory_usage.fsmblks = 37;
	GET_MEMORY_UTILIZATION_MOCK_RETVAL.page_size = 1000;
	GET_MEMORY_UTILIZATION_MOCK_RETVAL.phys_page_cnt = 4;
	GET_MEMORY_UTILIZATION_MOCK_RETVAL.available_phys_page_cnt = 2;
	GET_MEMORY_UTILIZATION_MOCK_RETVAL.dynamic_memory_usage.uordblks = 22;
	GET_MEMORY_UTILIZATION_MOCK_RETVAL.dynamic_memory_usage.fordblks = 114;

	get_memoryutilization(nullptr, 0);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
}

/* get_loadinfo */
TEST_C(clnt_commands_tests, get_loadinfo_bad_param){
	get_loadinfo(nullptr, 1);
	CHECK_EQUAL_C_STRING(BAD_ARGS, PRINTF_TEST_BUFFER);
}

TEST_C(clnt_commands_tests, get_loadinfo_no_server){
	get_loadinfo(nullptr, 1);
	CHECK_EQUAL_C_STRING(BAD_ARGS, PRINTF_TEST_BUFFER);
}

TEST_C(clnt_commands_tests, get_loadinfo_valid){
	_setup_servers(1);
	const char* expected_output = \
		"Server 1 -> Loaded Processes Per Minute:\n"
		"\t1 Minute: 17.500\n"
		"\t5 Minutes: 85.123\n"
		"\t15 Minutes: 150.776\n\n";

	double retval[3] = {17.5, 85.123, 150.7756};
	GET_LOAD_PROCS_PER_MINUTE_MOCK_RETVAL = retval;

	get_loadinfo(nullptr, 0);
	CHECK_EQUAL_C_STRING(expected_output, PRINTF_TEST_BUFFER);
}
