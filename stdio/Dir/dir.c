#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

/*
注：一般情况下 chdir 函数 和 getcwd 函数配合在一起使用
一、chdir 函数
    1、作用：修改当前进程的路径
    2、函数原型：
	int chdir(const char *path);

二、getcwd 函数
    1、作用：获取当前进程工作目录
    2、函数原型：
	char *getcwd(char *buf, size_t size);
    参数说明：getcwd()会将当前工作目录的绝对路径复制到参数buffer所指的内存空间中,参数size为buffer的空间大小。
	返 回 值：成功则返回当前工作目录，失败返回 FALSE

    char *getwd(char *buf);

三、rmdir 函数
    1、作用：删除一个目录
    2、函数原型：
    int rmdir(const char *pathname);
	若成功则返回0，否则返回-1

	!!一个空目录。空目录是只包含.和..这两项的目录

四、mkdir 函数
    1、作用：创建一个目录
	2、函数原型：
	int mkdir(const char *pathname, mode_t mode);
	若目录创建成功，则返回0；否则返回-1，并将错误记录到全局变量errno中。
	mode方式：可多个权限相或，如0755表示S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH
	
	!!mkdir函数新创建的目录，并不会被后续的代码直接使用，需要使用chdir修改到新建目录下才能使用
	
	int access(const char *pathname,int mode);   
	判断有没有此文件或目录 --它区别不出这是文件还是目录
	
	int stat(const char *file_name,struct stat *buf); 
	判断该文件或目录是否否存在 ；得到st_mode，然后判断是不是目录文件。 
    stat()系统调用看是否成功，不成功就不存在，成功判断返回的st_mode是否是一个文件夹。
*/

#define SIZE     516

int dir_crtl(char *path)
{
	char pathname[SIZE] = {0};
	char ori_path[SIZE] = {0};
	int fd;
	int ret;

	//获取原始路径
	getcwd(ori_path, sizeof(ori_path));
	printf("orignal ori_path:%s\n", ori_path);

	//创建当前的目录下新目录
	ret = mkdir(path, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
	if(ret<0)
	{
		//perror("mkdir failed");
		fprintf(stderr, "mkdir failed:%d:\n",ret);
		exit(1);
	}
	printf("new path argv[1] = %s\n", path);

	//!!mkdir函数新创建的目录，并不会被后续的代码直接使用，需要使用chdir修改到新建目录下才能使用
	//修改当前的路径回到原始路径
	ret = chdir(path);
	if(ret == -1)
	{
		perror("chdir failed");
		exit(1);
	}

	//在新目录下创建一个新的文件，来证明目录已经改变
	fd = open("chdir.txt", O_CREAT|O_RDWR, 0644);
	if(fd == -1)
	{
		perror("open file failed\n");
		exit(1);
	}

	//获取新目录后的目录名
	getcwd(pathname, sizeof(pathname));
	printf("new pathname:%s\n", pathname);

	close(fd);
	return 0;
}

/*
五、opendir函数
    1、作用：打开一个目录
    2、函数原型：
    #include <dirent.h>
       DIR *opendir(const char *name);
       DIR *fdopendir(int fd);
    3、返回值：
       (1)、DIR结构体指针，该结构是一个内部结构，保存所打开的目录的信息，作用于类似于FILE结构。
       (2)、函数出错，返回NULL

六、readdir 函数
       1、作用：读目录
       2、函数原型：
           struct dirent *readdir(DIR *dirp);
       3、返回值：
             返回一个记录项  
         struct dirent {
               ino_t          d_ino;       // inode number                   // 目录进入点的 inode
               off_t          d_off;       // not an offset; see NOTES       // 目录文件头开始至此目录进入点的位移
               unsigned short d_reclen;    // length of this record          // d_name 长度
               unsigned char  d_type;      // type of file; not supported      // d_name 所指的文件夹 
                                              by all filesystem types 
               char           d_name[256]; // filename                       // 文件名
         	};
        d_tyep有8 种类型：   
            (1)、 DT_BLK      This is a block device.           块设备
            (2)、 DT_CHR      This is a character device.       字符设备
            (3)、 DT_DIR       This is a directory.              目录
            (4)、 DT_FIFO     This is a named pipe (FIFO).      管道
            (5)、 DT_LNK      This is a symbolic link.          软链接
            (6)、 DT_REG      This is a regular file.           普通文件
            (7)、 DT_SOCK     This is a UNIX domain socket.     套接字
            (8)、 DT_UNKNOWN     The file type is unknown.         未知类型

七、closedir 函数
       1、作用：关闭一个目录
	   2、函数原型：
       		#include <dirent.h>
       		int closedir(DIR *dirp);
       3、返回值：
            若函数执行成功，返回0；若失败，返回 -1.
*/

//opendir、readdir、closedir 三个函数 的综合运用：

#include <dirent.h>

struct content
{
	int files;
	int dirs;
};

// 获取/home/duobao/linux_base目录下的文件个数
int get_file_count(char *root, struct content *state)
{
	//open dir
	DIR * dir = NULL;
	char *content = root;

	dir = opendir(content);
	if(dir == NULL)
	{
		perror("opendir");
		exit(1);
	}

	//遍历当前打开的目录
	struct dirent * ptr = NULL;
	char path[1024] = {0};
	int total = 0;
	while((ptr = readdir(dir)) != NULL)
	{
		// 过滤掉 . 和 ..
		if(strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name,"..") == 0)
		{
			continue;
		}

		//如果是目录，递归读目录
		if(ptr->d_type == DT_DIR)
		{
			sprintf(path, "%s%s", root, ptr->d_name);
			get_file_count(path, state);
			state->dirs++;
		}
		//如果是普通文件
		if(ptr->d_type == DT_REG)
		{
			state->files++;
		}
	}
	// 关闭目录
	closedir(dir);

	return total;
}

int main(int argc, char *argv[])
{

	int contents;
	struct content ll = {0}; 

	if(argc!=2)
	{
		fprintf(stderr, "usage :%s pathname\n", argv[0]);
		exit(1);
	}
#if 0
	printf("dir_crtl\n");
	dir_crtl(argv[1]);
#else
	printf("get_file_count\n");
	contents = get_file_count(argv[1], &ll);

	printf("contents: %s include:%d dirs, %d files\n", argv[1], ll.dirs, ll.files);
#endif
	return 0;

}