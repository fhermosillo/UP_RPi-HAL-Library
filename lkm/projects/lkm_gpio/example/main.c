#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main ()
{
	int f = open("/proc/lkm-gpio", O_RDWR);
	
	char buf[128];
	ssize_t num_bytes_read = read(f, buf, 7);
	printf("Number of read byres: %d\n", num_bytes_read);
	printf("Kernel Module Write: %s\n",buf);	
	
	write(f, "21,1", 4);
	usleep(500000);
	write(f, "21,0", 4);
	usleep(500000);
	
	return 0;
}
