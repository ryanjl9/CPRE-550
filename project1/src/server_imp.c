#include <linux/kernel.h>
#include <linux/sys.h>
#include <pwd.h>

int main(int argc, char** argv){
	struct sysinfo info;
	int retval = sysinfo(&info);
	return 0;
}
