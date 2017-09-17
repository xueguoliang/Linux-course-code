

void test()
{
	int fd = myConnect2(9988, "192.168.19.88");
	
	myWriteBuf("asdfsadfsdf");
	myWriteBuf("asdfsadfsdf");
	myWriteBuf("asdfsadfsdf");

	close(fd);

	exit(0);
}

int main()
{
	int n = 1500;
	for(int i=0; i<n; ++i)
	{
		pid_t pid = fork();
		if(pid > 0) 
			continue;
		else 
			test();
	}

	for(int i=0; i<n; ++i)
	{
		wait(NULL);
	}
}
