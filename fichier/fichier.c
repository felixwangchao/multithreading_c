#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main()
{
	char buf[30];
	int size;
	int desc;
	desc = open("./a.txt",O_RDONLY,NULL);
	size = read(desc,buf,sizeof(buf));
	printf("%s\n",buf);
 	while(size){
 	size = read(desc,buf,sizeof(buf));
 	printf("%s\n",buf);
	}
	return 0;
}
