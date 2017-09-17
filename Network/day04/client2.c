#include "../network.h"

void test()
{
	int fd = myConnect2(9988, "192.168.19.88");
	
	myWriteBuf(fd, "asdfsadfsdf");
	myWriteBuf(fd, "asdfsadfsdf");
	myWriteBuf(fd, "asdfsadfsdf");

	close(fd);

	exit(0);
}

int main()
{
	int n = 1500;
    int i;
	for(i=0; i<n; ++i)
	{
		pid_t pid = fork();
		if(pid > 0) 
			continue;
		else 
			test();
	}

	for(i=0; i<n; ++i)
	{
		wait(NULL);
	}
}
