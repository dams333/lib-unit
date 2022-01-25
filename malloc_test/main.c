#include "malloc_test.h"

void *test()
{
	return malloc(100);
}

int main()
{
	malloc(10);
	test();
	test();
	free(test());
	test();
	free(test());
	free(test());
	printf("%i not freed\n", count_no_free());
}