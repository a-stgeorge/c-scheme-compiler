#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	
	char *testString = malloc(sizeof(char) * 5);
	testString[0] = '1';
	testString[1] = '2';
	testString[2] = '3';
	testString[3] = '4';
	testString[4] = '\0';

	char *str2 = "a";
	char *str3 = malloc(sizeof(char) * 6);
	str3 = strcat(testString, str2);

	printf("%s: %lu\n", str3, strlen(str3));

	return 0;
}
