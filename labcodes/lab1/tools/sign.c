#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    struct stat st;	//����stat���ͱ���st
    if (argc != 3)	//���argv[]�е�Ԫ�ز���4��
	{
        fprintf(stderr, "Usage: <input filename> <output filename>\n");
        return -1;
    }
    if (stat(argv[1], &st) != 0) //ʹ��int stat(const char *file_name, struct stat *buf);�������ļ�argv[1]����Ϣ������st�У�����ɹ�����0��ʧ�ܷ���-1	
	{
        fprintf(stderr, "Error opening file '%s': %s\n", argv[1], strerror(errno));
        return -1;
    }
    printf("'%s' size: %lld bytes\n", argv[1], (long long)st.st_size);	//�������ļ��ֽ���
    if (st.st_size > 510) //����ֽ�������510�ֽ�
	{
        fprintf(stderr, "%lld >> 510!!\n", (long long)st.st_size);
        return -1;
    }
	//argv[1]�ļ�С�ڵ���510�ֽ�
    char buf[512];
    memset(buf, 0, sizeof(buf));
    FILE *ifp = fopen(argv[1], "rb");	//�Զ��ķ�ʽ�򿪶�����argv[1]�ļ����������ַ����ifp
    int size = fread(buf, 1, st.st_size, ifp);
    if (size != st.st_size) {
        fprintf(stderr, "read '%s' error, size is %d.\n", argv[1], size);
        return -1;
    }
    fclose(ifp);//�ر��ļ�
    buf[510] = 0x55;	//�����������ֽ�
    buf[511] = 0xAA;
    FILE *ofp = fopen(argv[2], "wb+");	//��д�ķ�ʽ�򿪶�����argv[2]�ļ����������ַ����ofp
    size = fwrite(buf, 1, 512, ofp);
    if (size != 512) {
        fprintf(stderr, "write '%s' error, size is %d.\n", argv[2], size);
        return -1;
    }
    fclose(ofp);
    printf("build 512 bytes boot sector: '%s' success!\n", argv[2]);
    return 0;
}

