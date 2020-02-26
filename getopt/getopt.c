#include <stdio.h>
#include <unistd.h>

int getopt(int argc, char *const argv[], const char *optstring);
extern char *optarg;
extern int optind, opterr, optopt;

int main(int argc, char * argv[])
{

	int opt;

	while((opt = getopt(argc, argv, "if:lr")) != -1)
	{
		switch(opt)
		{

			case 'i':
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
			case '?':
			printf("unknown option :%c\n", optopt);
			break;
		}
	}
	
	for(; optind <argc; optind++)
		printf("argument:%s\n", argv[optind]);

	exit(0);


}