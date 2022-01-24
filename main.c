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
	add_test(list4, "with ok", "this_goona_help_you(3)", FALSE, INT_VALUE, &return_int_5, 5);
	add_test(list4, "with ko", "this_goona_help_you(3)", FALSE, INT_VALUE, &return_int_5, 7);
	add_test(list4, "with signal", "this_goona_help_you(3)", FALSE, INT_VALUE, &sig2, 7);
	add_test_list(tester, list4);

	t_tests_list *list5 = init_tests_list("Acceptable crash tests");
	add_test(list5, "not acceptable", NULL, FALSE, INT_VALUE, &sig1, 5);
	add_test(list5, "acceptable", NULL, TRUE, INT_VALUE, &sig1, 5);
	add_test_list(tester, list5);

	launch_test(tester);
}