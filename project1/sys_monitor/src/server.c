#include <linux/kernel.h>
#include <sys/sysinfo.h>
#include <malloc.h>
#include <stdio.h>
#include <time.h>
#include <pwd.h>
#include "proj_config.h"
#include "sys_monitor.h"

int main(int argc, char** argv){
	struct sysinfo info;
	struct mallinfo minfo = mallinfo();
	int retval = sysinfo(&info);
	
	FILE* passwd_file = fopen("/etc/passwd", "r");
	struct passwd* password_lists = fgetpwent(passwd_file);
	password_lists = fgetpwent(passwd_file);
	return 0;
}

char** getdatetime_1_svc(enum dt_ops_t* argp, struct svc_req * clnt){
	(void)clnt;
	static char retval[50] = {0};
	return nullptr;
}

struct sysinfo_c* getsysteminfo_1_svc(void* argp, struct svc_req* clnt){
	(void)argp; (void)clnt;
	return nullptr;
}

struct mem_usage_ret* getmemoryutilization_1_svc(void* argp, struct svc_req* clnt){
	(void)argp; (void)clnt;
	return nullptr;
}

double* getloadprocsperminute_1_svc(void* argp, struct svc_req* clnt){
	(void)argp; (void)clnt;
	return nullptr;
}

struct user_info* getusernames_1_svc(void* argp, struct svc_req* clnt){
	(void)argp; (void)clnt;
	return nullptr;
}
