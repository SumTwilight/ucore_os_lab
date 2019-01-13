#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    struct stat st;	//定义stat类型变量st
    if (argc != 3)	//如果argv[]中的元素不是4个
	{
        fprintf(stderr, "Usage: <input filename> <output filename>\n");
        return -1;
    }
    if (stat(argv[1], &st) != 0) //使用int stat(const char *file_name, struct stat *buf);函数将文件argv[1]的信息保存在st中，如果成功返回0，失败返回-1	
	{
        fprintf(stderr, "Error opening file '%s': %s\n", argv[1], strerror(errno));
        return -1;
    }
    printf("'%s' size: %lld bytes\n", argv[1], (long long)st.st_size);	//输出这个文件字节数
    if (st.st_size > 510) //如果字节数大于510字节
	{
        fprintf(stderr, "%lld >> 510!!\n", (long long)st.st_size);
        return -1;
    }
	//argv[1]文件小于等于510字节
    char buf[512];
    memset(buf, 0, sizeof(buf));
    FILE *ifp = fopen(argv[1], "rb");	//以读的方式打开二进制argv[1]文件，并将其地址赋给ifp
    int size = fread(buf, 1, st.st_size, ifp);
    if (size != st.st_size) {
        fprintf(stderr, "read '%s' error, size is %d.\n", argv[1], size);
        return -1;
    }
    fclose(ifp);//关闭文件
    buf[510] = 0x55;	//填充最后两个字节
    buf[511] = 0xAA;
    FILE *ofp = fopen(argv[2], "wb+");	//以写的方式打开二进制argv[2]文件，并将其地址赋给ofp
    size = fwrite(buf, 1, 512, ofp);
    if (size != 512) {
        fprintf(stderr, "write '%s' error, size is %d.\n", argv[2], size);
        return -1;
    }
    fclose(ofp);
    printf("build 512 bytes boot sector: '%s' success!\n", argv[2]);
    return 0;
}

