/**
 * File client.c
 * Author: Ryan Lanciloti
 * Date of Creation: 2/14/2023
 * Description: This file will act as the user interface for the system_monitor client.
 * 	It will take commands from the user and send them to the server.
*/
#include <stdio.h>
#include "client.h"
#include "console_helper.h"
#include "sys_monitor.h"

const char* intro_msg = "Welcome to Remote System Monitor - Type `help` for a list of "
						"available commands.\n";

#ifndef _UNIT_TEST
int main(int argc, char** argv){
	return 0;
}
#endif //_UNIT_TEST
