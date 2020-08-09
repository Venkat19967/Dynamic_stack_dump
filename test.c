#include <stdio.h>
#include <linux/kernel.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <errno.h>
#include <pthread.h>


#ifndef GRADING
#define TEST_SYMBOL1 "sys_hello"
#define TEST_SYMBOL2 "sys_hello"
#define TEST_SYMBOL3 "sys_hello"
#define TEST_SYMBOL4 "sys_hello"
#define TEST_SYMBOL5 "sys_open"
#define TEST_DRIVERS "<Your stub driver name, /dev/some>"
#endif

#define INSDUMP_SYSCALL 359
#define RMDUMP_SYSCALL 360

struct dumpmode_t {
	unsigned int mode;
};





void* test(void* input){


	printf("\nInside the thread");

	sleep(5);
	syscall(361);
	/* int fd;
	   printf("OPENING THE FILE\n");
	   fd = open("/home/root/test", O_RDWR);
	   if(close(fd)<0){
	   printf("ERROR WHILE OPENING THE FILE\n");
	   }*/
	return 0;
}



int main()
{
//	int fd; 
	int d_id;
	pid_t p;
	char symbol_name[40];
	pthread_t thread1;
	struct dumpmode_t i;
	snprintf(symbol_name, sizeof(char)*40, "%s", TEST_SYMBOL2);



	//Dumpmode > 0  -- add and remove the kprobe from the same process
	printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	printf("\nDumpmode > 0 Testing the syscall\n");
	printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	i.mode=10;
	d_id = syscall(INSDUMP_SYSCALL, symbol_name,&i);

	syscall(361);
	/*fd = open("/dev/tty", O_RDWR);
	  if(close(fd)<0){
	  printf("ERROR WHILE OPENING THE FILE\n");
	  }*/
	printf("\nReturn value from rmdump:%ld", syscall(RMDUMP_SYSCALL,d_id));	

	printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	sleep(5);



	//Dumpmode > 0 -- add and remove from the same process but the dump stack works
	printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	printf("\nDumpmode>0 testing if the dumpstack is working a new process --- It will not!\n");
	printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	i.mode=10;
	d_id = syscall(INSDUMP_SYSCALL, symbol_name,&i);
	p = fork();

	if(p <= 0){
		printf("\nInside the child process");
		syscall(361);
		/*	fd = open("/dev/tty", O_RDWR);
			if(close(fd)<0){
			printf("ERROR WHILE OPENING THE FILE\n");
			}*/
		//printf("\nReturn value from rmdump:%ld", syscall(RMDUMP_SYSCALL,d_id));

		exit(0);
	}

	sleep(5);	
	//printf("\nReturn value from rmdump:%ld", syscall(RMDUMP_SYSCALL,d_id));

	printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	sleep(5);



	snprintf(symbol_name, sizeof(char)*40, "%s", TEST_SYMBOL1);



	//Dumpmode > 0 -- add and remove from the child process. But it does not work because the owner process 
	printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	printf("\nDumpmode>0  this test will show that rmdump cannot be removed from the child process and it is removed from the parent\n");
	printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	i.mode=10;
	d_id = syscall(INSDUMP_SYSCALL, symbol_name,&i);
	p = fork();

	if(p <= 0){
		printf("\nInside the child process");
		syscall(361);
		/*	fd = open("/dev/tty", O_RDWR);
			if(close(fd)<0){
			printf("ERROR WHILE OPENING THE FILE\n");
			}*/
		printf("\nReturn value from rmdump:%ld", syscall(RMDUMP_SYSCALL,d_id));

		printf("\n*********It does not get removed here hence exiting process will remove the kprobe ************\n");

		exit(0);
	}

	sleep(5);	
	printf("\nReturn value from rmdump:%ld", syscall(RMDUMP_SYSCALL,d_id));

	printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	sleep(5);


	snprintf(symbol_name, sizeof(char)*40, "%s", TEST_SYMBOL3);

	//Dumpmode = 0 -- The child process cannot do the dumpstack
	printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	printf("\nDumpmode=0  this test will show that a forked process cannot do dumpstack\n");
	printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	i.mode=0;
	d_id = syscall(INSDUMP_SYSCALL, symbol_name,&i);
	p = fork();

	if(p <= 0){
		printf("\nInside the child process");
		syscall(361);
		/*	fd = open("/dev/tty", O_RDWR);
			if(close(fd)<0){
			printf("ERROR WHILE OPENING THE FILE\n");
			}*/


		exit(0);
	}

	sleep(5);	
	//printf("\nReturn value from rmdump:%ld", syscall(RMDUMP_SYSCALL,d_id));

	printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	sleep(5);


	//Dumpmode = 1 -- add and remove from the child process. But it does not work because the owner process 
	printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	printf("\nDumpmode=1  this test will show that a forked process cannot do dumpstack\n");
	printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	i.mode=1;
	d_id = syscall(INSDUMP_SYSCALL, symbol_name,&i);


	pthread_create(&thread1,NULL, test, (void *) NULL);
	pthread_join(thread1,NULL);


	sleep(5);	
	printf("\nReturn value from rmdump:%ld", syscall(RMDUMP_SYSCALL,d_id));

	printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	sleep(5);

	
	snprintf(symbol_name, sizeof(char)*40, "%s", TEST_SYMBOL4);



	printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	d_id = syscall(INSDUMP_SYSCALL, symbol_name,&i);


	printf("\nThis is the final exiting process that will remove all the kprobes:\n");

	sleep(1);


	return 0;
}
