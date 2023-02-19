#include <linux/kernel.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <malloc.h>
#include <stdio.h>
#include <time.h>
#include <pwd.h>
#include "proj_config.h"
#include "sys_monitor.h"

#define DT_MAX_LEN 50

char** getdatetime_1_svc(enum dt_ops_t* argp, struct svc_req * clnt){
	(void)clnt;
	static char* retval;
	static char tmpstr[DT_MAX_LEN];
	
	time_t cur_time = time(0);
	struct tm* cur_time_s = localtime(&cur_time);

	switch(*argp){
		case DATE:
			strftime(retval, DT_MAX_LEN, "%A, %B %d, %Y", cur_time_s);
			break;
		case TIME:
			strftime(retval, DT_MAX_LEN, "%T", cur_time_s);
			break;
		case DATE_TIME:
			strftime(retval, DT_MAX_LEN, "%A, %B %d, %Y - %T", cur_time_s);
			break;
	}
	
	retval = &tmpstr;
	return &retval;
}

struct sysinfo_c* getsysteminfo_1_svc(void* argp, struct svc_req* clnt){
	(void)argp; (void)clnt;
	static struct sysinfo info;
	static struct sysinfo_c retval;

	sysinfo(&info);
	memcpy(&retval, &info, sizeof(struct sysinfo_c));

	return &retval;
}

struct mem_usage_ret* getmemoryutilization_1_svc(void* argp, struct svc_req* clnt){
	(void)argp; (void)clnt;
	static struct mem_usage_ret retval;

	struct mallinfo minfo = mallinfo();
	memcpy(&(retval.dynamic_memory_usage), &minfo, sizeof(struct mem_usage_ret));
	retval.page_size = getpagesize();
	retval.phys_page_cnt = get_phys_pages();
	retval.available_phys_page_cnt = get_avphys_pages();

	return &retval;
}

double* getloadprocsperminute_1_svc(void* argp, struct svc_req* clnt){
	(void)argp; (void)clnt;
	static double retval[3];

	getloadavg(retval, 3);

	return (double*)retval;
}
