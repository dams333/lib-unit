#include "includes/libunit.h"

char	*ok()
{
	return "1";
}

char	*ko()
{
	return "12";
}

char	*segv()
{
	raise(SIGSEGV);
}

char *test1()
{
	t_stdout *ft_stdout = init_stdout_test();
	write(1, "Salu", 6);
	end_stdout_test(ft_stdout);
	return ft_stdout->output;
}

char *test2()
{
	return "Salut";
}

int main()
{
	t_tester *tester = init_tester("Libft Tester");

	t_tests_list *list1 = init_tests_list("ft_strlen");
	add_test(list1, "ok", NULL, &ok, &ok);
	add_test(list1, "ko", NULL, &ok, &ko);
	add_test(list1, "stdout", "write(1, \"Salu\", 6);", &test1, &test2);
	add_test_list(tester, list1);

	t_tests_list *list2 = init_tests_list("ft_atoi");
	add_test(list2, "ok", NULL, &ok, &ok);
	add_test(list2, "ko", NULL, &ok, &ko);
	add_test(list2, "segv", NULL, &segv, &test2);
	add_test_list(tester, list2);

	launch_test(tester);
}