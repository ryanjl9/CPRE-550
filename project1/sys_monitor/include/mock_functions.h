/**
 * File: mock_functions.h
 * Author: Ryan Lanciloti
 * Date of Creation: 2/12/2023
 * Description: This header file will hold the external function and variables used by
 * the mock_functions.c file.
*/
#ifdef _UNIT_TEST

#ifndef _MOCK_FUNCTIONS_H
#define _MOCK_FUNCTIONS_H
#include "proj_config.h"
#include "sys_monitor.h"

/* DEFINES */
#define GETDATETIME_MOCK_RETVAL_SIZE 50

/* PUBLIC FUNCTION HEADERS */
extern int printf_mock(int, const char*, ...);
extern char** getdatetime_mock(CLIENT* clnt);
struct sysinfo_c* getsysteminfo_mock(CLIENT* clnt);
struct mem_usage_ret* getmemoryutilization_mock(CLIENT* clnt);
double* getloadprocsperminute_mock(CLIENT* clnt);

/* PUBLIC DATA */
extern char* PRINTF_TEST_BUFFER;
extern char SCANF_TEST_BUFFER[MAX_COMMAND_LINE_LEN];
extern int EXIT_STATUS;

extern char GET_DATETIME_MOCK_RETVAL[GETDATETIME_MOCK_RETVAL_SIZE];
extern struct sysinfo_c GET_SYSTEM_INFO_MOCK_RETVAL;
extern struct mem_usage_ret GET_MEMORY_UTILIZATION_MOCK_RETVAL;
extern double* GET_LOAD_PROCS_PER_MINUTE_MOCK_RETVAL;

/* UTILITY MACROS */
#define CLEAR_BUFFER(buffer) if(buffer != nullptr) free(buffer); buffer = nullptr;
#define printf(format, ...) printf_mock(FALSE, format, ##__VA_ARGS__)
#define scanf(format, ...) sscanf(SCANF_TEST_BUFFER, format, ##__VA_ARGS__)
#define getdatetime(argp, clnt) getdatetime_mock(clnt)
#define getsysteminfo(argp, clnt) getsysteminfo_mock(clnt)
#define getmemoryutilization(argp, clnt) getmemoryutilization_mock(clnt)
#define getloadprocsperminute(argp, clnt) getloadprocsperminute_mock(clnt)
#define exit(status) EXIT_STATUS = status

//Needed as printf has is redefined with macro
#define DBG_PRINTF(format, ...) fprintf(stdout, format, ##__VA_ARGS__)
#endif //_MOCK_FUNCTIONS_H

#endif //_UNIT_TEST
