#include <stdio.h>
#include "color_printf.h"

#define  COMMAND		"COMMAND"
#define  CONST_NAME1	"CONST_NAME1"
#define	 CONST_NAME2	"CONST_NAME2"

#line	14		"line.c"
#if 0
#if defined(GCC1.0)
	#pragma message("TEST VERSION 1....")
	#define GCCVERSION "GCC 1.0"
#elif defined(GCC2.0)
	#pragma message("TEST VERSION 2....")
	#define GCCVERSION "GCC 2.0"
#elif defined(GCC3.0)
	#pragma message("TEST VERSION 3....")
	#define GCCVERSION "GCC 3.0"
#else
	#error No defined constant system COMMAND
#endif
#endif

//#号运算符，用于在预编译期将宏参数转换为字符串
#define CONVER(x)		#x
#define CALL(f,p)		(printf("call fuction %s\n", #f),f(p))

int square(int x)
{
	return x*x;
}

int f(int x)
{
	return x;
}

//##(两个)运算符用于在预编译期粘连两个符号
#define  NAME(n)		name##n

//传统结构体定义方法
typedef struct _tag_stu
{
	int i;
	char c;
	short num;
}_STU;
_STU student1;

//用##号定义 结构体
#define  STTU(type)		typedef struct _tag##type type;struct _tag##type 

STTU(_STTU)	//typedef struct _tag_STTU _STTU;struct _tag_STTU 
{
	int i;
	char c;
	short num;
};

int main(int argc, char *argv[])
{
	#ifndef	COMMAND
	#warning Compilation will be stoped
	#error No defined constant system COMMAND
	#endif

	printf("%s\n", COMMAND);
	printf("%s\n", CONST_NAME1);
	printf("%s\n", CONST_NAME2);
	//BLUE_PRI("lalala","hh");

	YELLOW_PRI("Sdk use gcc version:","GCCVERSION");

	printf("%s\n", CONVER(hello world!));
	printf("%s\n", CONVER(100));
	printf("%s\n", CONVER(while));
	printf("%s\n", CONVER(return));

//#号运算符，用于在预编译期将宏参数转换为字符串
	printf("%d\n", CALL(square, 4));
	printf("%d\n", CALL(f, 2));

//##(两个)运算符用于在预编译期粘连两个符号
	int NAME(1);
	int NAME(2);

	NAME(1) = 1;
	NAME(2) = 2;

	printf("%s: %d\n", CONVER(NAME(1)),NAME(1));
	printf("%s: %d\n", CONVER(NAME(2)),NAME(2));

//用##号定义 结构体
	_STTU student2;

	student2.i = 10;
	student2.c = 'a';
	student2.num = 100;

	printf("student2,--i:%d, --c:%c,--num:%d\n", student2.i, student2.c, student2.num);

	return 0;
}