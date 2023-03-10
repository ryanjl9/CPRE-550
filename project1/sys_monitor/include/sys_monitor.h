/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _SYS_MONITOR_H_RPCGEN
#define _SYS_MONITOR_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef double load_procs_ret[3];

struct sysinfo_c {
	long uptime;
	u_long loads[3];
	u_long totalram;
	u_long freeram;
	u_long sharedram;
	u_long bufferram;
	u_long totalswap;
	u_long freeswap;
	u_short procs;
	char _f[22];
};
typedef struct sysinfo_c sysinfo_c;

struct mallinfo_c {
	int arena;
	int ordblks;
	int smblks;
	int hblks;
	int hblkhd;
	int usmblks;
	int fsmblks;
	int uordblks;
	int fordblks;
	int keepcost;
};
typedef struct mallinfo_c mallinfo_c;

struct mem_usage_ret {
	struct mallinfo_c dynamic_memory_usage;
	int page_size;
	long phys_page_cnt;
	long available_phys_page_cnt;
};
typedef struct mem_usage_ret mem_usage_ret;

enum dt_ops_t {
	DATE = 0,
	TIME = 0 + 1,
	DATE_TIME = 0 + 2,
};
typedef enum dt_ops_t dt_ops_t;

enum load_time_map_t {
	ONE_MINUTE = 0,
	FIVE_MINUTE = 0 + 1,
	FIFTEEN_MINUTE = 0 + 2,
};
typedef enum load_time_map_t load_time_map_t;

#define SYS_MONITOR 0x20159398
#define SYS_MONITOR_VERSION 1

#if defined(__STDC__) || defined(__cplusplus)
#define GetDateTime 1
extern  char ** getdatetime_1(enum dt_ops_t *, CLIENT *);
extern  char ** getdatetime_1_svc(enum dt_ops_t *, struct svc_req *);
#define GetSystemInfo 2
extern  struct sysinfo_c * getsysteminfo_1(void *, CLIENT *);
extern  struct sysinfo_c * getsysteminfo_1_svc(void *, struct svc_req *);
#define GetMemoryUtilization 3
extern  struct mem_usage_ret * getmemoryutilization_1(void *, CLIENT *);
extern  struct mem_usage_ret * getmemoryutilization_1_svc(void *, struct svc_req *);
#define GetLoadProcsPerMinute 4
extern  double * getloadprocsperminute_1(void *, CLIENT *);
extern  double * getloadprocsperminute_1_svc(void *, struct svc_req *);
extern int sys_monitor_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define GetDateTime 1
extern  char ** getdatetime_1();
extern  char ** getdatetime_1_svc();
#define GetSystemInfo 2
extern  struct sysinfo_c * getsysteminfo_1();
extern  struct sysinfo_c * getsysteminfo_1_svc();
#define GetMemoryUtilization 3
extern  struct mem_usage_ret * getmemoryutilization_1();
extern  struct mem_usage_ret * getmemoryutilization_1_svc();
#define GetLoadProcsPerMinute 4
extern  double * getloadprocsperminute_1();
extern  double * getloadprocsperminute_1_svc();
extern int sys_monitor_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_load_procs_ret (XDR *, load_procs_ret);
extern  bool_t xdr_sysinfo_c (XDR *, sysinfo_c*);
extern  bool_t xdr_mallinfo_c (XDR *, mallinfo_c*);
extern  bool_t xdr_mem_usage_ret (XDR *, mem_usage_ret*);
extern  bool_t xdr_dt_ops_t (XDR *, dt_ops_t*);
extern  bool_t xdr_load_time_map_t (XDR *, load_time_map_t*);

#else /* K&R C */
extern bool_t xdr_load_procs_ret ();
extern bool_t xdr_sysinfo_c ();
extern bool_t xdr_mallinfo_c ();
extern bool_t xdr_mem_usage_ret ();
extern bool_t xdr_dt_ops_t ();
extern bool_t xdr_load_time_map_t ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_SYS_MONITOR_H_RPCGEN */
