#include <stdio.h>
#include "fileaccess.h"

int main(int argc, char *argv[])

	
{
	printf("%d\n", argc);
	printf(*argv);
    if (is_readable(*(argv+1)) == FILE_NOTEXIST)
		printf("File does not exist");
	if (is_readable(*(argv+1)) == FILE_UNREADABLE)
		printf("File is unreadable");
	if (is_readable(*(argv+1)) == FILE_READABLE)
		printf("File is readable");
    return 0;
}
