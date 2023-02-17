/**
 * File: config.h
 * Date of Creation: 2/12/2023
 * Description: This file will be used to hold macros that can be used across the
 * 	different source files.
*/
#ifndef _CONFIG_H
#define _CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

/* DEFINES */
#define nullptr 0
#ifndef FALSE
	#define FALSE 0
#endif //FALSE

#ifndef TRUE 
	#define TRUE 1
#endif //TRUE

#define MAX_COMMAND_ARGS 25
#define MAX_COMMAND_LINE_LEN 50
#define MAX_SERVER_NAME_LEN 50
#define MAX_SERVER_CNT 10

#ifdef __cplusplus
}
#endif

#endif //_CONFIG_H