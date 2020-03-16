#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>

/*
通常使用GNU C提供的函数getopt、getopt_long、getopt_long_only函数来解析命令行参数。
一、关于命令行参数
     命令行参数可以分为两类:
	 1> 短选项，短选项在参数前加一杠"-"
	 2> 长选项，长选项在参数前连续加两杠"--"
   如下表（ls 命令参数）所示，其中-a，-A,-b都表示短选项，
   --all,--almost-all, --author都表示长选项。
   他们两者后面都可选择性添加额外参数。比如--block-size=SIZE，SIZE便是额外的参数。

*/


/*
定义函数:
int getopt(int argc,char * const argv[ ],const char * optstring);
	getopt() 所设置的全局变量包括：
	extern char *optarg;
	extern int optind, opterr, optopt;

	optarg ——指向当前选项参数（如果有）的指针。 
	optind ——(while)再次调用 getopt() 时的下一个argv指针的索引。 
	optopt ——最后一个未知选项。

	optstring中的指定的内容的意义（例如getopt(argc, argv, "ab:c:de::");）
	1.单个字符，表示选项（如下例中的abcde各为一个选项）。
	2.单个字符后接一个冒号:  ,表示该选项后必须跟一个参数。参数紧跟在选项后或者以空格隔开。
	该参数的指针赋给optarg（如下例中的b:c:）。
	3 单个字符后跟两个冒号:: ,表示该选项后可以跟一个参数，也可以不跟。如果跟一个参数，
	参数必须紧跟在选项后不能以空格隔开。该参数的指针赋给optarg。
	(如上例中的e::，如果没有跟参数，则optarg = NULL)
*/
void getopt_usage1(void)
{
	printf( "usage :./test xxx \n"
			"   -i :  no paramet\n"
			"   -f :  <param> \n"		
			"   -l :  no paramet\n"
			"   -h :  get help\n"
			"   ex : ./test -i -f <param> -l \n"
			);
}

int getopt_demo(int argc, char * argv[])
{
	int opt;

	//getopt_usage1();

	if(argc < 2)
	{
		printf("usage :%s -i -f <param> -l \n", argv[0]);
		_exit(1);
	}

	printf("%s run ...\n", __func__);
	while((opt = getopt(argc, argv, "if:lrh")) != -1)
	{
		switch(opt)
		{

			case 'i':
				printf("i cmd test\n");
				break;
			case 'l':
				break;
			case 'r':
				printf("option: %c\n",opt );
			break;
			case 'f':
				printf("filename: %s\n", optarg);
			break;
			case ':':
				printf("option needs a value\n");
			break;

			case 'h':
				getopt_usage1();
				_exit(1);
			break;

			case '?':
				printf("unknown option :%c\n", optopt);
			break;
		}
	}
	
	for(optind=0; optind <argc; optind++)
		printf("argument:%s\n", argv[optind]);
	
	return 0;
}

/*
定义函数:
int atoi(const char *nptr);
	atoi(表示ascii to integer)是把字符串转换成整型数的一个函数，
	
	函数会扫描参数nptr字符串，会跳过前面的空白字符（例如空格，tab缩进）等。
	如果 nptr不能转换成int或者nptr为空字符串，那么将返回0。
	特别注意，该函数要求被转换的字符串是按十进制数理解的。
	atoi输入的字符串对应数字存在大小限制（与int类型大小有关），若其过大可能报错-1。	
*/

int atoi_demo(void)
{
	printf("%s run ...\n", __func__);
	int n;
	char *str = "12345.67";
	n = atoi(str);
	printf("n = %d\n", n);
	
	return 0;
}

int atoi_demo2(void)
{
	printf("%s run ...\n", __func__);	
	char a[] = "-100";
	char b[] = "123";
	int c;
	c = atoi(a) + atoi(b);
	printf("c = %d\n", c);
	
	return 0;
}

/*
demo desc:
应用程序tcp/udp为例，我们要解析三个选项,-m指定以tcp/udp连接，-s指定IP地址，
-p指定端口号，解析参数选项的代码如下。
*/
void getopt_usage2(void)
{
	printf( "usage :./test <mode> <ip_addr> <port> \n"
			"   -m :  tcp or udp, chose connect method\n"
			"   -s :  xxx.xxx.xxx.xxx, set ip_addr\n"		
			"   -p :  xxx, set port\n"
			"   -h :  get help\n"
			"   ex :  ./test tcp 192.168.70.111 8080 \n"
			);
}

int getopt_demo2(int argc, char *argv[])
{

	//getopt_usage2();
	#define MODE_MAX_LELN	4
	#define IP_MAX_LEN		16	

	int i;
	int ch;
	char mode[MODE_MAX_LELN] = {0};
	char ip_addr[IP_MAX_LEN] = {0};
	int port = 0;

	if(argc < 2)
	{
		printf("usage :%s <mode> <ip_addr> <port> \n", argv[0]);
		_exit(1);
	}

	printf("%s run ...\n", __func__);
	while((ch = getopt(argc, argv, "m:s:p:h")) != -1)
	{
		switch(ch)
		{
			case 'm':
				snprintf(mode, "%s", optarg, strlen(optarg));
				printf("mode: %s\n", mode);
			break;

			case 's':
				snprintf(ip_addr, "%s", optarg, strlen(optarg));
				printf("ip_addr: %s\n", ip_addr);
			break;

			case 'p':
			//atoi(表示ascii to integer)是把字符串转换成整型数的一个函数，
				port = atoi(optarg);
				printf("port number: %d\n", port);
			break;

			case 'h':
				getopt_usage2();
				_exit(1);
			break;
		}
	}

	for(optind = 0; optind <argc; optind++)
	printf("argument list:%s \n", argv[optind]);
	
	return 0;
}

/*
函数说明:(getopt--解析命令行选项参数,getopt_long--支持长选项的命令行解析)

int getopt_long(int argc, char * const argv[],const char *optstring, 
				const struct option *longopts,int *longindex);
参数说明
	1. argc和argv通常直接从main()的两个参数传递而来。
	2. optsting是选项参数组成的字符串,字符串optstring可以有下列元素:
		1)单个字符，表示选项.
		2)单个字符后接一个冒号：表示该选项后必须跟一个参数。参数紧跟在选项后或者以空格隔开。
		该参数的指针赋给optarg。
		3)单个字符后跟两个冒号，表示该选项后可以有参数也可以没有参数。如果有参数，参数必须紧跟
		在选项后**不能以空格隔开。该参数的指针赋给optarg。
	
	例如，"a:b:cd"，表示可以接受的参数是a,b,c,d，其中，a和b参数后面跟有更多的参数值。
	(例如：-a host -b name)

	3. 参数longopts，是一个结构的实例：
	struct option {
	const char *name; //name表示的是长参数名
	int has_arg； //has_arg有3个值可选!
			1) no_argument(或者是0)，表示该参数后面不跟参数值
			2) required_argument(或者是1),表示该参数后面一定要跟个参数值
			3) optional_argument(或者是2),表示该参数后面可以跟，也可以不跟参数值
	int *flag;//用来决定getopt_long()的返回值到底是什么。
			1) 如果flag是null(通常情况)，则函数会返回与该项option匹配的val值;
			2) 如果flag不是NULL，则将val值赋予flag所指向的内存，并且返回值设置为0。
	int val; //和flag联合决定返回值
	}
注意:
	required_argument(或者是1)时，参数输入格式为：--参数 值 或者 --参数=值。
	optional_argument(或者是2)时，参数输入格式只能为：--参数=值。

例子：
	struct option long_options[] = {
	{"a123", required_argument, 0, 'a'},
	{"c123", no_argument, 0, 'c'},
	}
	现在，如果命令行的参数是-a 123，那么调用getopt_long()将返回字符'a',
	并且将字符串123由optarg返回(注意注意！字符串123由optarg带回！optarg不需要定义,在getopt.h中已经有定义)，
	那么，如果命令行参数是-c，那么调用getopt_long()将返回字符'c',而此时，optarg是null。
	最后，当getopt_long()将命令行所有参数全部解析完成后，返回-1。

	4. 参数longindex，表示当前长参数在longopts中的索引值。

全局变量：
	（1）optarg：表示当前选项对应的参数值。
	（2）optind：表示的是下一个将被处理到的参数在argv中的下标值。
	（3）opterr：如果opterr = 0，在getopt、getopt_long、getopt_long_only遇到错误
	 将不会输出错误信息到标准输出流。opterr在非0时，向屏幕输出错误。
	（4）optopt：表示没有被未标识的选项。

返回值：
    （1）如果短选项找到，那么将返回短选项对应的字符。
    （2）如果长选项找到，如果flag为NULL，返回val。如果flag不为空，返回0.
    （3）如果遇到一个选项没有在短字符、长字符里面。或者在长字符里面存在二义性的，返回“？”
    （4）如果解析完所有字符没有找到（一般是输入命令参数格式错误，eg： 连斜杠都没有加的选项），返回“-1”
    （5）如果选项需要参数，忘了添加参数。返回值取决于optstring，如果其第一个字符是“：”，则返回“：”，否则返回“？”。
注意：
	（1）longopts的最后一个元素必须是全0填充，否则会报段错误
	（2）短选项中每个选项都是唯一的。而长选项如果简写，也需要保持唯一性。
*/
void getoptlong_usage0(void)
{
	printf( "usage : ./demo nbl:h\n"
			"   -n :  no_argument\n"
			"   -b :  no_argument\n"		
			"   -l :  love\n"
			"   -h :  get help\n"
			"   ex :  ./demo love 1314 \n"
			);
}

int getopt_long_demo0(int argc, char *argv[])
{
	char *l_opt_arg;

	char* const short_options = "nbl:h";
	struct option long_options[] = {
		{ "name", 0, NULL, 'n' },
		{ "bf_name", 0, NULL, 'b' },
		{ "love", 1, NULL, 'l' },
		{ 0, 0, 0, 0},
	};
	printf("%s run ...\n", __func__);
	int ch;
	while((ch = getopt_long (argc, argv, short_options, long_options, NULL)) != -1)
	{
	switch (ch)
		{
			case 'n':
			printf("My name is XL.\n");
			break;

			case 'b':
			printf("His name is ST.\n");
			break;
			
			case 'l':
			l_opt_arg = optarg;
			printf("Our love is %s!\n", l_opt_arg);
			/*
			struct option {
				const char *name; //name表示的是长参数名
				从结构体来看，name 后面紧跟的param参数，是被optarg当做字符串一起访问。
				等于将"name param"当做一个字符串访问.
			*/
			//偏移len+1个字节长度（+1为了跳过空格），找到长选项love紧跟的参数
			printf("Our love is %s!\n", l_opt_arg+strlen(optarg)+1);
			break;
			
			case 'h':
			getoptlong_usage0();
			_exit(1);
			break;
		}
	}

	return 0;
}

int getopt_long_demo1(int argc, char **argv)
{
    int c;
    int digit_optind = 0;
 
   	while (1) {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        static struct option long_options[] = {
            {"add",     required_argument, 0,  0 },
            {"append",  no_argument,       0,  0 },
            {"delete",  required_argument, 0,  0 },
            {"verbose", no_argument,       0,  0 },
            {"create",  required_argument, 0, 'c'},
            {"file",    required_argument, 0,  0 },
            {0,         0,                 0,  0 }
        };
 
       c = getopt_long(argc, argv, "abc:d:012",
                 	   long_options, &option_index);
        if (c == -1)
            break;
 
       	switch (c) {
	        case 0:
	            printf("option %s", long_options[option_index].name);
	            if (optarg)
	                printf(" with arg %s", optarg);
	            printf("\n");
	            break;
 
	       	case '0':
	        case '1':
	        case '2':
	            if (digit_optind != 0 && digit_optind != this_option_optind)
	            printf("digits occur in two different argv-elements.\n");
	            digit_optind = this_option_optind;
	            printf("option %c\n", c);
	            break;
 
	       	case 'a':
	            printf("option a\n");
	            break;
	 
	       	case 'b':
	            printf("option b\n");
	            break;
	 
	       	case 'c':
	            printf("option c with value '%s'\n", optarg);
	            break;
	 
	       	case 'd':
	            printf("option d with value '%s'\n", optarg);
	            break;
	 
	       	case '?':
	            break;
	 
	       	default:
	            printf("?? getopt returned character code 0%o ??\n", c);
	            break;
        }
    }
 
   if (optind < argc) {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n");
    }
 
   _exit(0);
}

//对于extern "C"可以理解成在C++/C中的混合编程的编译指令。
#ifdef __cplusplus 	/* 如果采用了C++，如下代码使用C编译器 */
extern "C" { 	/* 如果没有采用C++，顺序预编译 */
#endif

/* 采用C编译器编译的C语言代码段 */
#ifdef __cplusplus 	/* 结束使用C编译器 */
} 
#endif 

#if 1
void showUsage() {
  //cout << "Usage: " << PACKAGE_NAME << " [options] URL ..." << endl;
  cout << " Options:" << endl;
  cout << " -d, --dir=DIR              The directory to store downloaded file." << endl;
  cout << " -o, --out=FILE             The file name for downloaded file." << endl;
  cout << " -l, --log=LOG              The file path to store log. If '-' is specified," << endl;
  cout << "                            log is written to stdout." << endl;
  cout << " -D, --daemon               Run as daemon." << endl;
  cout << " -s, --split=N              Download a file using s connections. s must be" << endl;
  cout << "                            between 1 and 5. If this option is specified the" << endl;
  cout << "                            first URL is used, and the other URLs are ignored." << endl;
  cout << " --http-proxy=HOST:PORT     Use HTTP proxy server. This affects to all" << endl;
  cout << "                            URLs." << endl;
  cout << " --http-user=USER           Set HTTP user. This affects to all URLs." << endl;
  cout << " --http-passwd=PASSWD       Set HTTP password. This affects to all URLs." << endl;
  cout << " --http-proxy-user=USER     Set HTTP proxy user. This affects to all URLs" << endl;
  cout << " --http-proxy-passwd=PASSWD Set HTTP proxy password. This affects to all URLs." << endl;
  cout << " --http-auth-scheme=SCHEME  Set HTTP authentication scheme. Currently, BASIC" << endl;
  cout << "                            is the only supported scheme." << endl;
  cout << " -v, --version              Print the version number and exit." << endl;
  cout << " -h, --help                 Print this message and exit." << endl;
  cout << " URL:" << endl;
  cout << " You can specify multiple URLs. All URLs must point to the same file" << endl;
  cout << " or a download fails." << endl;
  cout << " Examples:" << endl;
  cout << " Download a file by 1 connection:" << endl;
  cout << " aria2c http://AAA.BBB.CCC/file.zip" << endl;
  cout << " Download a file by 2 connections:" << endl;
  cout << " aria2c -s 2 http://AAA.BBB.CCC/file.zip" << endl;
  cout << " Download a file by 2 connections, each connects to a different server." << endl;
  cout << " aria2c http://AAA.BBB.CCC/file.zip http://DDD.EEE.FFF/GGG/file.zip" << endl;
  cout << " Reports bugs to <tujikawa at rednoah dot com>" << endl;
}
 
int getopt_long_demo2(int argc, char* argv[]) {
  bool stdoutLog = false;
  char logfile[] = {0};
  char dir[] = {0};
  char ufilename[] = {0};
  int split = 0;
  bool daemonMode = false;
  int ch;
 
  while(1) {
    int optIndex = 0;
    int lopt;
    static struct option longOpts[] = {
	    { "daemon", no_argument, NULL, 'D' },
	    { "dir", required_argument, NULL, 'd' },
	    { "out", required_argument, NULL, 'o' },
	    { "log", required_argument, NULL, 'l' },
	    { "split", required_argument, NULL, 's' },
	    { "http-proxy", required_argument, &lopt, 1 },
	    { "http-user", required_argument, &lopt, 2 },
	    { "http-passwd", required_argument, &lopt, 3 },
	    { "http-proxy-user", required_argument, &lopt, 4 },
	    { "http-proxy-passwd", required_argument, &lopt, 5 },
	    { "http-auth-scheme", required_argument, &lopt, 6 },
	    { "version", no_argument, NULL, 'v' },
	    { "help", no_argument, NULL, 'h' },
	    { 0, 0, 0, 0 }
    };
    ch = getopt_long(argc, argv, "Dd:o:l:s:vh", longOpts, &optIndex);
    printf("返回值： %c\n",ch);
    if(ch == -1) {
        break;
    }
    switch(ch) {

    	case 0:{
	      	switch(lopt){

		      	case 1: 
					printf("1: %s\n",optarg);
				break;

		      	case 2:
					printf("2: %s\n",optarg);
				break;
		      	
		      	case 3:
					printf("3: %s\n",optarg);
				break;
		      
		      	case 4:
					printf("4: %s\n",optarg);
				break;
		      
		      	case 5: 
					printf("5: %s\n",optarg);
				break;
		      	
		      	case 6:
					printf("6: %s\n",optarg);
				break;
	        }

	        break;
    	}

	    case 'D':
 	       printf("D: %s\n",optarg);
	       break;

	    case 'd':
	       printf("d: %s\n",optarg);
	       break;

	    case 'o':
	       printf("o: %s\n",optarg);
	       break;

	    case 'l':
	       printf("l: %s\n",optarg);
	       break;

	    case 's':
	       printf("s: %s\n",optarg);
	       break;

	    case 'v':
	       printf("s: %s\n",optarg);
	       //showVersion();
	       exit(0);

	    case 'h':
	       showUsage();
	       exit(0);

	    default:
	       showUsage();
	       exit(1);
    	}
    }

  return 0;
}
#endif

/*注意：宏定义只能单个测试*/

//#define GETOPT   			
//#define GETOPT2   			
//#define GETOPT_LONG0		
//#define GETOPT_LONG1
#define GETOPT_LONG2

int main(int argc, char *argv[])
{

#ifdef GETOPT
	getopt_demo(argc, argv);
#endif

#ifdef GETOPT
	atoi_demo();
	atoi_demo2();
	getopt_demo2(argc, argv);
#endif

#ifdef GETOPT_LONG0
	getopt_long_demo0(argc, argv);	
#endif

#ifdef GETOPT_LONG1
	getopt_long_demo1(argc, argv);	
#endif

#ifdef GETOPT_LONG2
	getopt_long_demo2(argc, argv);	
#endif
	return 0;
}


