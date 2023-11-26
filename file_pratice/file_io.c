/*
 * 7장 파일 처리
 * 파일 이름: file_io.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFSIZE 256

int main()
{
	int n;
	char buf[BUFSIZE];

	fprintf(stderr, "Here is file start.\n");
	while((n = read(0,buf,BUFSIZE)) > 0)
		write(1,buf,n);
	fprintf(stderr, "Here is file end.\n");
	exit(0);
}
