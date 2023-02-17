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

/* PUBLIC FUNCTION HEADERS */
extern int printf_mock(int, char*, ...);

/* PUBLIC DATA */
extern char* PRINTF_TEST_BUFFER;
extern char SCANF_TEST_BUFFER[MAX_COMMAND_LINE_LEN];

/* UTILITY MACROS */
#define CLEAR_BUFFER(buffer) if(buffer != nullptr) free(buffer); buffer = nullptr;
#define printf(format, ...) printf_mock(FALSE, format, ##__VA_ARGS__)
#define scanf(format, ...) sscanf(SCANF_TEST_BUFFER, format, ##__VA_ARGS__)

//Needed as printf has is redefined with macro
#define DBG_PRINTF(format, ...) fprintf(stdout, format, ##__VA_ARGS__)
#define GET_PARAM_X(arr, index) ((char*)arr + MAX_COMMAND_LINE_LEN * index)
#endif //_MOCK_FUNCTIONS_H

#endif //_UNIT_TEST
