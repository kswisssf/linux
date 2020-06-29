#include <stdio.h>

/*
一、printf原型为：
int printf(const char *format, ...);

二、规定符
%d 十进制有符号整数
%u 十进制无符号整数
%f 浮点数
%s 字符串
%c 单个字符
%p 指针的值
%e 指数形式的浮点数
%x, %X 无符号以十六进制表示的整数
%o 无符号以八进制表示的整数
%g 把输出的值按照%e或者%f类型中输出长度较小的方式输出
%p 输出地址符
%lu 32位无符号整数
%llu 64位无符号整数

三、说明
(1).可以在“%”和字母之间插进数字表示最大场宽。
	例如：%3d表示输出3位整型数，不够3位右对齐。
		 %9.2f表示输出场宽为9的浮点数，其中小数位为2，整数位为6，小数点占一位，不够9位右对齐。
		 %8s表示输出8个字符的字符串，不够8个字符右对齐。

(2).可以在“%”和字母之间加小写字母l，表示输出的是长型数。
	例如：%ld表示输出long整数
		 %lf表示输出double浮点数

(3).可以控制输出左对齐或右对齐，即在"%"和字母之间加入一个"-"号可
	说明输出为左对齐, 否则为右对齐。
	例如：%-7d 表示输出7位整数左对齐
		 %-10s 表示输出10个字符左对齐

四、一些特殊规定字符:

\n 换行
\f 清屏并换页
\r 回车
\t Tab符
\xhh 表示一个ASCII码用16进表示,
   	 其中hh是1到2个16进制数
*/

/*
函数声明
	int fprintf (FILE* stream, const char*format, [argument])
参数
	stream-- 这是指向FILE对象的指针,该FILE对象标识了流。
	format-- 这是C字符串，包含了要被写入到流stream中的文本.它可以包含嵌入的format标签,format标签可被随后的附加参数中指定的值替换,并按需求进行格式化。
	format 标签属性是%[flags][width][.precision][length]specifier
	[argument]：附加参数列表 
功能
	fprintf()函数根据指定的格式(format)，向输出流(stream)写入数据(argument)。
函数说明
	fprintf( )会根据参数format 字符串来转换并格式化数据，然后将结果输出到参数stream 指定的文件中，直到出现字符串结束('\0')为止。
*/

#define SIZE	1024

int  fprintf_demo_1(void)
{
	FILE *fp;
	char *path = "demo1.txt";
	char strbuf[SIZE] = "fprintf_demo_1";

	fp = fopen(path, "wr+");
	fprintf(fp, "%s\n", strbuf);

	fclose(fp);
	
	return 0;
}

int fprintf_demo_2(void)
{
	FILE *stream;

	int i = 10;
	double fp = 1.5;
	char s[] = "fprintf_demo_2";
	char c = '\n';	

	char *path = "demo2.txt";
	stream = fopen(path, "wr+");

	fprintf(stream, "%s%c\n", s, c);
	fprintf(stream, "%d\n", i);
	fprintf(stream, "%f\n", fp);
	
	fclose(stream);
	
	return 0;
}

int fprintf_demo_3(void)
{
    FILE *in,*out;

    printf("fprintf_demo_3\n");
    
	in = fopen("AUTOEXEC.BAT", "rt");
    if(in == NULL)
    {
        fprintf(stderr, "Can not open inputfile.\n");
        //return 1;
    }
    out = fopen("AUTOEXEC.BAT", "wt");
    if(out == NULL)
    {
        fprintf(stderr, "Can not open outputfile.\n");
        //return 1;
    }
   
   if(in!=NULL && out!=NULL) 
    {
    while(!feof(in))
        fputc(fgetc(in), out);
   
    fclose(in);
    fclose(out);
	}

    return 0;
}

int fprintf_demo_4(void)
{
    FILE* fp;

    int i = 617;
    char* s = "fprintf_demo_4";
    fp = fopen("text.dat", "w");
/*
fputs是一个函数，具有的功能是向指定的文件写入一个字符串（不自动写入字符串结束标记符‘\0’）。
成功写入一个字符串后，文件的位置指针会自动后移，函数返回值为非负整数；否则返回EOF(符号常量，其值为-1)。
函数原型:
	int fputs(const char *str, FILE *stream);
返回值：该函数返回一个非负值，如果发生错误则返回 EOF(-1)。
   （1）str：这是一个数组，包含了要写入的以空字符终止的字符序列。
   （2）stream：指向 FILE 对象的指针，该 FILE 对象标识了要被写入字符串的流。
*/
    fputs("total", fp);
    fputs(":", fp);
  
    fprintf(fp, "%d\n", i);
    fprintf(fp, "%s", s);
    
    fclose(fp);
    
    return 0;
}

/*
函数声明:
	int sprintf(char *string, char *format [,argument,...]);
参数列表:
	string-- 这是指向一个字符数组的指针，该数组存储了 C 字符串。
	format-- 这是字符串，包含了要被写入到字符串 str 的文本。
	它可以包含嵌入的 format 标签，format 标签可被随后的附加参数中指定的值替换，并按需求进行格式化。
	format 标签属性是%[flags][width][.precision][length]specifier
	[argument]...：根据不同的 format 字符串，函数可能需要一系列的附加参数，每个参数包含了一个要被插入的值，
	替换了format参数中指定的每个 % 标签。参数的个数应与 % 标签的个数相同。
功能:
	把格式化的数据写入某个字符串缓冲区。
返回值:
	如果成功，则返回写入的字符总数，不包括字符串追加在字符串末尾的空字符。如果失败，则返回一个负数。
	sprintf 返回以format为格式argument为内容组成的结果被写入string的字节数，结束字符‘\0’不计入内。
	即，如果“Hello”被写入空间足够大的string后，函数sprintf 返回5。

*/
int sprintf_demo()
{
	char buffer[SIZE];
	char S[] = "sprintf";
	char c = '1';
	int i = 99;
	int offset = 0;
	float fd = 3.1415926;
	//格式化并打印各种数据到buffer	 
	offset  = sprintf(buffer, "string: %s\n", S);
	printf("Output:\n%s length count:%d\n\n", buffer, offset);

	offset += sprintf(buffer, "Character: %c\n", c);
	printf("Output:\n%s length count:%d\n\n", buffer, offset);

	//格式化数字字符串sprintf 最常见的应用之一莫过于把整数打印到字符串中
	offset += sprintf(buffer, "Integer: %d\n", i);
	printf("Output:\n%s length count:%d\n\n", buffer, offset);
	/*
	浮点数的打印和格式控制,格式符”%f”控制，默认保留小数点后6位数字.
	控制打印的宽度和小数位数，”%m.nf”格式，其中m 表示打印的宽度，n 表示小数点后的位数。比如：
	*/
	offset += sprintf(buffer, "float: %.5f", fd);
	printf("Output:\n%s length count:%d\n\n", buffer, offset);
	/*
	连接字符串
	sprintf 的格式控制串中既然可以插入各种东西，并最终把它们“连成一串”，
	自然也就能够连接字符串，从而在许多场合可以替代strcat，但sprintf 
	能够一次连接多个字符串（自然也可以同时在它们中间插入别的内容，总之非常灵活）。
	*/
	char *who = "I";
	char *whom = "CSDN";
	offset += sprintf(buffer," %slove%s.\n", who, whom);
	printf("Output:\n%s length count:%d\n\n", buffer, offset);
	/*
	打印地址信息
	有时调试程序时，看某些变量或者成员的地址，由于地址或者指针也不过是个32位的数，
	可以使用打印无符号整数的”%u”把他们打印出来：
	*/
	offset += sprintf(buffer,"i addr:%p\n", &i);

	/*
	利用sprintf 的返回值知道字符串的长度
	sprintf 返回了本次函数调用最终打印到字符缓冲区中的字符数目。
	也就是说每当一次sprinf 调用结束以后，你无须再调用一次strlen便已经知道了结果字符串的长度offset.
	*/
	printf("Output:\n%s length count:%d\n\n", buffer, offset);

	return 0;
}


int main(int argc, char *argv[])
{
	fprintf_demo_1();

	fprintf_demo_2();

	fprintf_demo_3();

	fprintf_demo_4();

	printf("sprintf demo\n");
	sprintf_demo();
	
	return 0;
}