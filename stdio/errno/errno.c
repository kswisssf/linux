#include <stdio.h>
#include <string.h>
#include <errno.h>

/*
定义函数:
	void perror ( const char * str );
	用来将上一个函数发生错误的原因输出到标准设备(stderr)。
	参数str 所指的字符串会先打印出，后面再加上错误原因字符串。

	此错误原因依照全局变量errno的值来决定要输出的字符串。
    在库函数中有个errno变量，每个errno值对应着以字符串表示的错误类型。
    当你调用"某些"函数出错时，该函数已经重新设置了errno的值。
    perror函数只是将你输入的一些信息和errno所对应的错误一起输出。

perror() 函数显示您传给它的字符串,后跟一个冒号.一个空格和当前errno 值的文本表示形式。
*/


//让我们来模拟一种错误情况，尝试打开一个不存在的文件
int perror_demo(void)
{
	FILE *fp;
	fp = fopen("root/noexxxxx","r+");
	if(NULL == fp)
	{
		perror("root/noexxxxx");
		return 1;
	}

	close(fp);

	return 0;
}


/*
函数名:
	strerror, _strerror, _wcserror, __wcserror
	通过标准错误的标号，获得错误的描述字符串 ，将单纯的错误标号转为字符串描述，方便用户查找错误。
	
	函数原型编辑
	char*strerror(int errnum);
	char*_strerror(const char*strErrMsg);
	wchar_t*_wcserror(int errnum);
	wchar_t*__wcserror(const wchar_t*strErrMsg)
参数：
	errnum：错误标号，通常用errno(标准错误号，定义在errno.h中)
	Error number.
	strErrMsg
	User-supplied message.
返回：
	指向错误信息的指针（即：错误的描述字符串）。

函数作用:
	获取系统错误信息或打印用户程序错误信息
*/

int strerror_demo(void)
{

	int fd;
	int errno;
	char *mesg;

	if((fd = open("/dev/dsp",'wr+')) < 0)
	{
		perror("/dev/dsp");
		printf("errno=%d\n",errno);
		mesg = strerror(errno);
		printf("Mesg:%s\n",mesg);
	}

	return 0;
}


int main(int argc, char *argv[])
{
	printf("perror demo\n");
	perror_demo();

	printf("strerror demo\n");
	strerror_demo();
	
	return 0;
}