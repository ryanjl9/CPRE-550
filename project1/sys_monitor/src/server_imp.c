#include <linux/kernel.h>
#include <sys/sysinfo.h>
#include <pwd.h>
#include <malloc.h>

int main(int argc, char** argv){
	struct sysinfo info;
	struct mallinfo minfo = mallinfo();
	int retval = sysinfo(&info);
	return 0;
}
