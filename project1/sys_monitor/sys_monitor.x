/*
	File: sys_monitor.x
	Author: Ryan Lanciloti
	Date of Creation: 2/8/2023

	Description: 
		This file describes the RPC interface which will be used to do remote 
		monitoring of servers. This will monitor 5 main aspects of the server:
			- The date and time of the server
			- Various statistics regarding CPU utilization
			- Various statistics regarding memory usage
			- Information regarding the number of processes in the system run queue
*/

/* TYPEDEFS */
typedef double load_procs_ret[3]; /* GetLoadProcsPerMinute return value */ 

/* STRUCTS */
struct sysinfo_c {				/* Struct returned by sysinfo call */
	long uptime;              	/* Seconds since boot */
	unsigned long loads[3];   	/* 1, 5, and 15 minute load averages */
	unsigned long totalram;   	/* Total usable main memory size */
	unsigned long freeram;    	/* Available memory size */
	unsigned long sharedram;  	/* Amount of shared memory */
	unsigned long bufferram;  	/* Memory used by buffers */
	unsigned long totalswap;  	/* Total swap space size */
	unsigned long freeswap;   	/* swap space still available */
	unsigned short procs;     	/* Number of current processes */
	char _f[22];              	/* Pads structure to 64 bytes */
};

struct mallinfo_c {	/* Struct returned by mallinfo call */
  int arena;    	/* non-mmapped space allocated from system */
  int ordblks;  	/* number of free chunks */
  int smblks;   	/* number of fastbin blocks */
  int hblks;    	/* number of mmapped regions */
  int hblkhd;   	/* space in mmapped regions */
  int usmblks; 	 	/* always 0, preserved for backwards compatibility */
  int fsmblks;  	/* space available in freed fastbin blocks */
  int uordblks; 	/* total allocated space */
  int fordblks; 	/* total free space */
  int keepcost; 	/* top-most, releasable (via malloc_trim) space */
};

struct mem_usage_ret {						/* Struct returned by GetMemoryUtilization*/
	struct mallinfo_c dynamic_memory_usage;	/* Struct containing info about dynamic memory usage */
	int page_size;							/* Page size of the remote machine */
	long phys_page_cnt;						/* Number of physical pages on the remote machine */
	long available_phys_page_cnt;			/* Number of free physical pages on the remote machine */
};

/* ENUMS */
enum dt_ops_t{		/* Enum used for determining what GetDateTime should return */
	DATE = 0,		/* For getting only the date */
	TIME,			/* For getting only the time */
	DATE_TIME		/* For getting both the date and time together */
};

enum load_time_map_t{	/* Enum used to parse the average load times */
	ONE_MINUTE = 0,		/* Maps for the 1 minute entry */
	FIVE_MINUTE,		/* Maps for the 5 minute entry */
	FIFTEEN_MINUTE		/* Maps for the 15 minute entry */
};

program SYS_MONITOR {
	version SYS_MONITOR_VERSION {
		string GetDateTime(enum dt_ops_t) = 1;
		struct sysinfo_c GetSystemInfo(void) = 2;
		struct mem_usage_ret GetMemoryUtilization(void) = 3;
		load_procs_ret GetLoadProcsPerMinute(void) = 4;
	} = 1;
} = 0x20159398;
