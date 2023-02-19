/**
 * File: mock_functions.c
 * Author: Ryan Lanciloti
 * Date of Creation: 2/12/2023
 * Description: This file contains general test infrastructure to be used across multiple
 * 	unit tests.
*/
#ifdef _UNIT_TEST
#include <string.h>
#include <stdio.h>
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport_c.h"
#include "mock_functions.h"

/* GLOBAL VARIABLES */
char* PRINTF_TEST_BUFFER = nullptr;
char SCANF_TEST_BUFFER[MAX_COMMAND_LINE_LEN] = {0};
int EXIT_STATUS = -1;

char GET_DATETIME_MOCK_RETVAL[GETDATETIME_MOCK_RETVAL_SIZE];
struct sysinfo_c GET_SYSTEM_INFO_MOCK_RETVAL;
struct mem_usage_ret GET_MEMORY_UTILIZATION_MOCK_RETVAL;
double* GET_LOAD_PROCS_PER_MINUTE_MOCK_RETVAL;

/* PRIVATE FUNCTION HEADERS*/
void _add_to_buffer(char** dst, char* src);

/* MOCKING FUNCTIONS */
/**
 * FUNCTIONS: _add_to_buffer
 * 
 * @brief: Private function that takes a pointer to a global malloc buffer and
 * 	dynamically expands it to support an additional string.
 * 
 * @param dst - Pointer to the global buffer
 * @param src - Pointer to a char* buffer to be appended to the global buffer
*/
void _add_to_buffer(char** dst, char* src){
	int size;
	if(*dst == nullptr){
		size = strlen(src) + 1;
		*dst = (char*)malloc(size * sizeof(char));
		strcpy(*dst, src);
		return;
	}
	size = strlen(*dst) + strlen(src) + 1;
	char* tmp_buffer = (char*)malloc(size * sizeof(char));
	memset(tmp_buffer, 0, size);
	strcat(tmp_buffer, *dst);
	strcat(tmp_buffer, src);
	free(*dst);
	*dst = tmp_buffer;
}

/**
 * @brief: This function will act as a mock for printf which takes in a string and
 * 	will add it to a global buffer which can be checked in a test case. Optionally, it
 * 	can still print the incoming string.
 * 
 * @param print - Flag which determines if the incoming string should be printed
 * @param format - String format, same as printf
 * @param ... - Arguments for the printf function
 * 
 * @return Returns 0 if no error, else 1
*/
int printf_mock(int print, const char* format, ...){
	if(format == nullptr) return 1;
	char buffer[512] = {0};
	
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	_add_to_buffer(&PRINTF_TEST_BUFFER, buffer);
	
	if(print) DBG_PRINTF("%s", buffer);
	
	va_end(args);
	return 0;
}

/**
 * @brief Mocks the getdatetime function
 * 
 * @return Pointer to a mocked output 
 */
char** getdatetime_mock(CLIENT* clnt){
	CHECK_EQUAL_C_POINTER(clnt, (CLIENT*)1);
	static char* retval = GET_DATETIME_MOCK_RETVAL;
	return &retval;
}

/**
 * @brief Mocks the getdatetime function
 * 
 * @return Pointer to a mocked output 
 */
struct sysinfo_c* getsysteminfo_mock(CLIENT* clnt){
	CHECK_EQUAL_C_POINTER(clnt, (CLIENT*)1);
	return (struct sysinfo_c*)&GET_SYSTEM_INFO_MOCK_RETVAL;
}

/**
 * @brief Mocks the getmemoryutilization function
 * 
 * @return Pointer to a mocked output 
 */
struct mem_usage_ret* getmemoryutilization_mock(CLIENT* clnt){
	CHECK_EQUAL_C_POINTER(clnt, (CLIENT*)1);
	return (struct mem_usage_ret*)&GET_MEMORY_UTILIZATION_MOCK_RETVAL;
}

/**
 * @brief Mocks the getloadsperminute function
 * 
 * @return Pointer to a mocked output 
 */
double* getloadprocsperminute_mock(CLIENT* clnt){
	CHECK_EQUAL_C_POINTER(clnt, (CLIENT*)1);
	return (double*)GET_LOAD_PROCS_PER_MINUTE_MOCK_RETVAL;
}

/* CPPUTESTS */
TEST_GROUP_C_SETUP(mock_functions_tests){
	CLEAR_BUFFER(PRINTF_TEST_BUFFER)
	memset(SCANF_TEST_BUFFER, 0, sizeof(char)* MAX_COMMAND_LINE_LEN);
}

TEST_GROUP_C_TEARDOWN(mock_functions_tests){
	mock_c()->checkExpectations();
	mock_c()->clear();
}

/* printf_mock tests */
TEST_C(mock_functions_tests, printf_mock_test_noargs){
	printf("Hello World!");
	CHECK_EQUAL_C_STRING(PRINTF_TEST_BUFFER, "Hello World!");
}

TEST_C(mock_functions_tests, printf_mock_test_args_multiple_lines){
	const char* expected_string = "Name: John Doe\nAge: 55 | FavNum: 0xB00B\n";
	printf("Name: %s\n", "John Doe");
	printf("Age: %d | FavNum: 0x%X\n", 55, 0xB00B);
	CHECK_EQUAL_C_STRING(PRINTF_TEST_BUFFER, expected_string);
}

/* scanf macro test */
TEST_C(mock_functions_tests, scanf_one_argument){
	const char* test_string = "Ryan";
	strcpy(SCANF_TEST_BUFFER, test_string);
	char name[10];
	scanf("%s", name);
	CHECK_EQUAL_C_STRING(name, test_string);
}

TEST_C(mock_functions_tests, scanf_one_multiple_arguments){
	const char* test_string = "Ryan Lanciloti 24 ";
	strcpy(SCANF_TEST_BUFFER, test_string);
	char first[10];
	char last[10];
	int age;
	scanf("%s %s %d", first, last, &age);
	CHECK_EQUAL_C_STRING(first, "Ryan");
	CHECK_EQUAL_C_STRING(last, "Lanciloti");
	CHECK_EQUAL_C_INT(age, 24);
}
#endif //_UNIT_TEST