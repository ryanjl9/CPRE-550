/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "sys_monitor.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

char **
getdatetime_1(enum dt_ops_t *argp, CLIENT *clnt)
{
	static char *clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, GetDateTime,
		(xdrproc_t) xdr_dt_ops_t, (caddr_t) argp,
		(xdrproc_t) xdr_wrapstring, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

struct sysinfo_c *
getsysteminfo_1(void *argp, CLIENT *clnt)
{
	static struct sysinfo_c clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, GetSystemInfo,
		(xdrproc_t) xdr_void, (caddr_t) argp,
		(xdrproc_t) xdr_sysinfo_c, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

struct mem_usage_ret *
getmemoryutilization_1(void *argp, CLIENT *clnt)
{
	static struct mem_usage_ret clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, GetMemoryUtilization,
		(xdrproc_t) xdr_void, (caddr_t) argp,
		(xdrproc_t) xdr_mem_usage_ret, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

double *
getloadprocsperminute_1(void *argp, CLIENT *clnt)
{
	static load_procs_ret clnt_res;

	memset((char *)clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, GetLoadProcsPerMinute,
		(xdrproc_t) xdr_void, (caddr_t) argp,
		(xdrproc_t) xdr_load_procs_ret, (caddr_t) clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (clnt_res);
}
