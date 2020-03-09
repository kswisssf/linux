
/*
1. 在C中，当我们无法列出传递函数的所有实参的类型和数目时,可以用省略号指定参数表
void foo(...);
void foo(parm_list,...);
这是C中一种传参的形式。

2. 函数参数的传递原理
   函数参数是以数据结构:栈的形式存取,从右至左入栈。
   首先是参数的内存存放格式：参数存放在内存的堆栈段中，在执行函数的时候，从最后一个开始入栈。
   因此栈底高地址，栈顶低地址，举个例子如下：
   void func(int 参数1(x), float 参数2(y), char z);
　　那么，调用函数的时候，实参char z 先进栈，然后是float y，最后是int x，因此在内存中变量的
   存放次序是 x->y->z，因此，从理论上说，我们只要探测到任意一个变量的地址，并且知道其他变量的类型，
   通过指针移位运算，则总可以顺藤摸瓜找到其他的输入变量。
　　
PS:栈是由低地址向上增长，就像是水桶，有一个桶底（低地址起始），加水从底下往上增长.
(注意 桶是倒者放的，桶口朝下，桶底在上，数据 进栈就是从栈顶（桶口）进入，向栈底增长，地址从小到大生长)
>>堆栈图解_看图王.png
		 |
		/|\	高地址 栈底方向	--------------------------高
		 |_____________________ 
		 |  参数n(n)			|
		 |	...				| 
		 |	...				|	
		 |  参数3(z)			函数参数入栈
		 |  参数2(y)			|
		 |  参数1(x) 		|
		 |__________________|__
		 |
		\|/ 低地址 栈顶方向    --------------------------低
		 |
通俗理解不定长参数原理就是:
	ap指向了上面的参数1，然后根据类型大小，挨个偏移获取参数2，参数3，直到没有参数为止！

下面是 <stdarg.h> 里面重要的几个宏定义如下：
	typedef char* va_list;
	void va_start ( va_list ap, prev_param ); // ANSI version 
	type va_arg ( va_list ap, type ); 
	void va_end ( va_list ap ); 
va_list 是一个字符指针，可以理解为指向当前参数的一个指针，取参必须通过这个指针进行。
<Step 1> 在调用参数表之前，定义一个 va_list 类型的变量，(假设va_list 类型变量被定义为ap)；
<Step 2> 然后应该对ap 进行初始化，让它指向可变参数表里面的第一个参数，这是通过 va_start 来实现的，
		 第一个参数是 ap 本身，第二个参数是在变参表前面紧挨着的一个变量,即“...”之前的那个参数；
<Step 3> 然后是获取参数，调用va_arg，它的第一个参数是ap，第二个参数是要获取的参数的指定类型，
		 然后返回这个指定类型的值，并且把 ap 的位置指向变参表的下一个变量位置；
<Step 4> 获取所有的参数之后，我们有必要将这个ap 指针关掉，以免发生危险，方法是调用 va_end，
         他是输入的参数ap 置为NULL，应该养成获取完参数表之后关闭指针的习惯。
         说白了，就是让我们的程序具有健壮性。通常va_start和va_end是成对出现。

在VC++6.0的include有一个stdarg.h头文件,有如下几个宏定义:
	#define _INTSIZEOF(n) ((sizeof(n)+sizeof(int)-1)&~(sizeof(int) - 1) )
	#define va_start(ap,v) ( ap = (va_list)&v + _INTSIZEOF(v) ) //第一个可选参数地址
	#define va_arg(ap,t) ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) ) //下一个参数的值
	#define va_end(ap) ( ap = (va_list)0 ) // 将指针置为无效

*/
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h> 

/*DEMO DESC:
 int max(int n, ...);其函数内部应该如此实现：
*/
void pri_arg(int num, ...) 
{
	int *temp = &num;
	int i;
	printf("num addr:%p val:%d\n", temp, num);
	
	temp++;
	for (i = 0; i < num; ++i)
	{
		printf("member addr:%p val: %d \n", temp, *temp);
		temp++;
	}

}

//在函数体中声明一个va_list，然后用va_start函数来获取参数列表中的参数，使用完毕后调用va_end()结束.
void _vsnprintf_demo(char* pszDest, int DestLen, const char* pszFormat, ...) 
{ 
	va_list args; 
	va_start(args, pszFormat); //一定要“...”之前的那个参数
	//int _vsnprintf(char* str, size_t size, const char* format, va_list ap);
	//_vsnprintf(pszDest, DestLen, pszFormat, args); 
	va_end(args); 
}


//同类型参数,地址递增寻址方法求和	
int max(int num, ...)
{
	int *tmp = &num;
	int i;
	int sum = 0;
	// 地址递增寻址法求和	
	printf("member num: %d\n", *tmp);
	for(i = 0; i < num; i++)
	{
		sum += *(++tmp);
		printf("tmp val:%d\n", *tmp);
	}
	
	return sum;	 	
}

/*ANSI标准形式的声明方式，括号内的省略号表示可选参数*/ 
int va_demo(char *msg, ...) 
{ 
    /*定义保存函数参数的结构*/
    va_list argp; 
    int argcnt = 0; 
    char *para = NULL;
    /*argp指向传入的第一个可选参数，msg是最后一个确定的参数*/
    va_start(argp, msg);

    while (1)
    {
    	para = va_arg(argp, char *);
    	if ( strcmp(para, " ") == 0 )
    		break;
    	printf("Parameter #%d is: %s\n", argcnt, para);
    	argcnt++;
    }
    
    va_end(argp);
    /*将argp置为NULL*/
    argp = NULL;

    return 0; 
}


#define NN 	 4

int main(int argc, char *argv[])
{
	int res;
	int tmp[NN]={1, 2, 3, 4};

	pri_arg(NN, tmp[0], tmp[1], tmp[2], tmp[3]);
	res = max(NN, tmp[0], tmp[1], tmp[2], tmp[3]);
	printf("max res: %d\n", res);

	if(argc < 2 )
	{
		printf("usage: %s <descrption>\n" 
			   "   ex: %s demo is a va_arg test case\n"
			   , argv[0], argv[0]);
		_exit(1);
	}

	va_demo(argv[0], argv[1], argv[2], argv[3], argv[4]);

	return 0;
}