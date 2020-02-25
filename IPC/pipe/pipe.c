#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

int pipe_fd[2];

int create_pipe(void)
{
	if(pipe(pipe_fd) < 0)
	{
		printf("create pipe failed \n");
		return -1;
	}else 
		{
			printf(" create sucessed!\n");
			printf(" pipe_fd:%d pipe_fd:%d\n", pipe_fd[0], pipe_fd[1]);
		}

	return 0;
}

void close_pipe()
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

int pipe_talk(const char *pipe_buf, int pipe_len)
{
	pid_t pid;
	int rd_len;
	int len = pipe_len;
	char *tmp_buf = (char *)malloc(len);
	
	if((pid=fork()) == 0)
	{
		printf(" child pid\n");
		close(pipe_fd[1]);
		sleep(1);		//get out cpu

		if((rd_len=read(pipe_fd[0], tmp_buf, len)) > 0)
			printf("read pipe_fd 0 sucessed len: %d :buf :%s\n",  rd_len, tmp_buf);
		close(pipe_fd[0]);
		exit(0);
	}else if(pid >0)
		{
			printf("father pid\n");
			close(pipe_fd[0]);
			if(write(pipe_fd[1], pipe_buf , len))
				printf("write pipe_fd 1 sucessed ,msg:%s\n", pipe_buf);
			close(pipe_fd[1]);
			exit(0);
		}

	free(tmp_buf);	
	return 0;
}

int main(int argc, char *argv[])
{
	char *test_pipe = "hello child";

	create_pipe();
	pipe_talk(test_pipe, strlen(test_pipe));

	return 0;
}