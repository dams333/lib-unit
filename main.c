#include "includes/libunit.h"

int	return_int_5()
{
	return 5;
}
int	return_int_7()
{
	return 7;
}

char	*return_str_1()
{
	return "Salut, ca va\n";
}
char	*return_str_2()
{
	return "Salut, ca va";
}

int	sig1()
{
	raise(SIGBUS);
}
int	sig2()
{
	raise(SIGSEGV);
}
int	sig3()
{
	raise(SIGFPE);
}
int	sig4()
{
	raise(SIGPIPE);
}
int	sig5()
{
	raise(SIGILL);
}
int	sig6()
{
	raise(SIGABRT);
}

int	timeout()
{
	while(1)
		;
}

char *test_stdout()
{
	t_stdout *stdout = init_stdout_test();
	write(1, "Salut\n", 6);
	end_stdout_test(stdout);
	return stdout->output;
}

int	test_malloc()
{
	start_malloc_catcher();
	start_malloc_breaker();
	char *str = malloc(10);
	str[0] = 'c';
	stop_malloc_breaker();
	stop_malloc_catcher_and_print_leaks();
	return 2;
}

int	test_malloc2()
{
	start_malloc_catcher();
	char *str = malloc(10);
	stop_malloc_catcher_and_print_leaks();
	return 2;
}

int	test_malloc3()
{
	start_malloc_catcher();
	free(malloc(10));
	stop_malloc_catcher_and_print_leaks();
	return 2;
}

int main()
{
	t_tester *tester = init_tester("LibUnit Tester");

	t_tests_list *list1 = init_tests_list("Int tests");
	add_test(list1, "value, ok", NULL, FALSE, INT_VALUE, &return_int_5, 5);
	add_test(list1, "value, ko", NULL, FALSE, INT_VALUE, &return_int_5, 7);
	add_test(list1, "compare, ok", NULL, FALSE, INT_COMPARE, &return_int_5, &return_int_5);
	add_test(list1, "compare, ko", NULL, FALSE, INT_COMPARE, &return_int_5, &return_int_7);
	add_test_list(tester, list1);

	t_tests_list *list2 = init_tests_list("String tests");
	add_test(list2, "value, ok", NULL, FALSE, STR_VALUE, &return_str_1, "Salut, ca va\n");
	add_test(list2, "value, ko", NULL, FALSE, STR_VALUE, &return_str_1, "Salut, ca va");
	add_test(list2, "compare, ok", NULL, FALSE, STR_COMPARE, &return_str_1, &return_str_1);
	add_test(list2, "compare, ko", NULL, FALSE, STR_COMPARE, &return_str_1, &return_str_2);
	add_test_list(tester, list2);

	t_tests_list *list3 = init_tests_list("Error tests");
	add_test(list3, "bus", NULL, FALSE, INT_VALUE, &sig1, 5);
	add_test(list3, "segv", NULL, FALSE, INT_VALUE, &sig2, 5);
	add_test(list3, "fpe", NULL, FALSE, INT_VALUE, &sig3, 5);
	add_test(list3, "pipe", NULL, FALSE, INT_VALUE, &sig4, 5);
	add_test(list3, "ill", NULL, FALSE, INT_VALUE, &sig5, 5);
	add_test(list3, "abrt", NULL, FALSE, INT_VALUE, &sig6, 5);
	add_test(list3, "timeout", NULL, FALSE, INT_VALUE, &timeout, 5);
	add_test_list(tester, list3);

	t_tests_list *list4 = init_tests_list("Code precision tests");
	add_test(list4, "with ok", "this_gonna_help_you(3)", FALSE, INT_VALUE, &return_int_5, 5);
	add_test(list4, "with ko", "this_gonna_help_you(3)", FALSE, INT_VALUE, &return_int_5, 7);
	add_test(list4, "with signal", "this_gonna_help_you(3)", FALSE, INT_VALUE, &sig2, 7);
	add_test_list(tester, list4);

	t_tests_list *list5 = init_tests_list("Acceptable crash tests");
	add_test(list5, "not acceptable", NULL, FALSE, INT_VALUE, &sig1, 5);
	add_test(list5, "acceptable", NULL, TRUE, INT_VALUE, &sig1, 5);
	add_test_list(tester, list5);

	t_tests_list *list6 = init_tests_list("Stdout");
	add_test(list6, "ok", NULL, FALSE, STR_VALUE, &test_stdout, "Salut\n");
	add_test(list6, "ko", NULL, FALSE, STR_VALUE, &test_stdout, "Salut");
	add_test_list(tester, list6);

	t_tests_list *list7 = init_tests_list("Malloc");
	add_test(list7, "non protected", NULL, FALSE, INT_VALUE, &test_malloc, 1);
	add_test(list7, "leak ko", NULL, FALSE, INT_VALUE, &test_malloc2, 1);
	add_test(list7, "leak ok", NULL, FALSE, INT_VALUE, &test_malloc2, 2);
	add_test(list7, "no leak ko", NULL, FALSE, INT_VALUE, &test_malloc3, 1);
	add_test(list7, "no leak ok", NULL, FALSE, INT_VALUE, &test_malloc3, 2);
	add_test_list(tester, list7);

	launch_test(tester);
}