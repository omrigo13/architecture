#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv){

	char* entryPoint;
	int fd = open(argv[1], O_RDONLY);

	int fileSize = 0;
	fileSize = lseek(fd, 0, SEEK_END);
	
	lseek(fd, 0, SEEK_SET);
	char buff[fileSize];
	int ret = read(fd, buff, fileSize);

	if(ret == -1)
  {
		perror("read");
		exit(1);
	}
	if(fd == -1)
  {
		perror("open");
		exit(1);
	}

  printf("%s's entry point: ", argv[1]);

	entryPoint = buff + 24;
	printf("0x");
	int i;
	for(i=3; i >= 0; i--)
		printf("%02x", (unsigned char) entryPoint[i]);
	printf("\n");
	close(fd);
	return 0;
}