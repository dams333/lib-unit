#include "malloc_test.h"

int main()
{
	start_malloc_catcher();
	char *test = malloc(10);
	start_malloc_breaker();
	char *str = malloc(10);
	if(str)
		str[0] = 'c';
	stop_malloc_breaker();
	stop_malloc_catcher_and_print_leaks();
}