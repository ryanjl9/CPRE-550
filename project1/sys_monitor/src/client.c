/**
 * File client.c
 * Author: Ryan Lanciloti
 * Date of Creation: 2/14/2023
 * Description: This file will act as the user interface for the system_monitor client.
 * 	It will take commands from the user and send them to the server.
*/
#include <stdio.h>
#include "console_helper.h"
#include "clnt_commands.h"

enum svr_subcmds {
	SVR_SUBCMDS_LIST = 0,
	SVR_SUBCMDS_ADD,
	SVR_SUBCMDS_RMV,
	SVR_SUBCMDS_SEL,
	SVR_SUBCMDS_CNT
};

enum cmds {
	CMD_HELP = 0,
	CMD_SERVER,
	CMD_DT,
	CMD_SYSINFO,
	CMD_MEMUSG,
	CMD_LDINFO,
	CMD_USRINFO,
	CMD_QUIT,
	CMD_CNT
};

const char* intro_msg = "Welcome to Remote System Monitor - Type `help` for a list of "
						"available commands.\n";

menu_item_t server_subcmds[SVR_SUBCMDS_CNT] = {
	{
		"list",
		"Lists out all servers. Ex. server list 1",
		(void*)&server_list, nullptr, 0
	},
	{
		"add", 
		"Adds a server to the list of current server. Ex. server add localhost",
		(void*)&server_add, nullptr, 0
	},
	{
		"remove",
		"Removes a server from the list of current servers. Ex. server remove 2",
		(void*)&server_remove, nullptr, 0
	},
	{
		"select",
		"Sets the currently active server that commands will be sent to. Ex. server "
		"select 3.", (void*)&server_select, nullptr, 0
	}
};

menu_item_t commands[CMD_CNT] = {
	{"help", "Prints this menu. Try `help server`.", (void*)&ch_help, nullptr, 0},
	{
		"server", 
		"Commands which modify server info. View subcommands to see available options.",
		nullptr, (menu_item_t*)&server_subcmds, SVR_SUBCMDS_CNT
	},
	{
		"datetime", 
		"Prints the remote server's date or time. Valid parameters: <date/time/both>. "
		"Ex. datetime both",
		(void*)&get_datetime, nullptr, 0
	},
	{
		"system_info",
		"Prints some information and statistics about the remote PC's CPU. Ex. "
		"system_info",
		(void*)&get_systeminfo, nullptr, 0
	},
	{
		"memory_utilization",
		"Prints some informatino and statistics about the remote PC's memory. "
		"Ex. memory_utilization",
		(void*)&get_memoryutilization, nullptr, 0
	},
	{
		"load_info",
		"Prints the 1, 5, and 15 minute load averages on the remote PC. Ex. load_info",
		(void*)&get_loadinfo, nullptr, 0
	},
	{
		"user_info",
		"Prints a list of all users on the remote PC. Ex. user_info",
		(void*)&get_usernames, nullptr, 0
	},
	{
		"quit", "Closes the currently running program Ex. quit", 
		(void*)ch_quit, nullptr, 0
	}
};

int main(int argc, char** argv){
	(void)argc; (void)argv;
	ch_init((char*)intro_msg, (menu_item_t*) &commands, CMD_CNT);
	ch_print_introduction();
	
	char user_input[MAX_COMMAND_ARGS * (MAX_COMMAND_LINE_LEN-1)] = {0};

	while(1){
		printf("> ");
		fgets(user_input, MAX_COMMAND_ARGS * (MAX_COMMAND_LINE_LEN-1), stdin);
		user_input[strcspn(user_input, "\n")] = 0;
		ch_execute(user_input);
	}

	return 0;
}
