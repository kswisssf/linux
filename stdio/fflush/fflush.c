#include <stdio.h>
#include "color_printf.h"

/*
缓冲又分为两种，行缓冲和完全缓冲。
*/

/*
对于行缓冲来说，遇到一个换行符，则清空缓冲区，键盘输入是标准的行缓冲，
因此，按下换行键的时候才会清空缓冲区。
将要输出到标准输出的字符存放在缓冲区当中，由于一直没有遇到换行符，
因此一直不会输出到屏幕，等到程序结束后才输出到屏幕上。
*/
void  Row_cache_test()
{
	printf("row cache");	//没有换行符
	int i = 0;
	for(; i<10; i++)
	{
		putchar('.');
		sleep(1);
	}
}

/*
对于完全缓冲来说，缓冲区满时被清空(内容被发送到指定的目的地)。
fflush()函数的作用是刷新缓冲区，把缓冲区的内容输出到执行位置。 
那么这个程序的运行结果应该是：先输出hello，然后等待10s输出10个小数点
*/
void stdout_test()
{
	printf("full cache, stdout test");	//没有换行符
	fflush(stdout); //新添加的行，刷新输出缓冲区
	int i = 0;
	for(;i<10;i++)
	{
		putchar('.');
		sleep(1);
	}
}

/*
fprint函数将字符输出到标准错误输出上去，而不是标准输出。虽然stdout和stderr都是指向屏幕，
但是两者还是有区别，stderr是立即回显，不会将字符送入缓冲区。

因此，这个程序的输出结果应该是：先输出hello，再输出一个“.”然后每隔1s输出一个小数点，输出10个小数点后程序结束
*/

void stderr_test()
{
	fprintf(stderr, "stderr test");
	int i = 0;
	for(;i<10;i++)
	{
		fprintf(stderr, ".");
		sleep(1);
	}
}

void usage()
{
	printf("input ch :\n"                   
					 "1-Row_cache_test\n"	 
					 "2-stdout_test\n" 	 
					 "3-stderr_test\n"     
					 "4-quit\n");		
}

int main()
{
	int ch;
	usage();
	int run = 1;
	
	while(run)
	{
		usage();
		scanf("%d",&ch);

		switch(ch)
		{
		
		case 1:
			Row_cache_test();
		break;

		case 2:
			stdout_test();
		break;

		case 3:
			stderr_test();
		break;
		
		case 4:
			printf("quit\n");
			run = 0;
		break;
		}

		BLUE_PRI("fish,get cmd",NULL);
		ch = -1;
	}
	return 0;
}