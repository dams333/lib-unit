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

char	*time()
{
	while(1)
		;
}

char *test1()
{
	t_stdout *ft_stdout = init_stdout_test();
	write(1, "Salu\n", 6);
	end_stdout_test(ft_stdout);
	return ft_stdout->output;
}

char *test2()
{
	return "Salut\n";
}

int main()
{
	t_tester *tester = init_tester("Libft Tester");

	t_tests_list *list1 = init_tests_list("ft_strlen");
	add_test(list1, "ok", &ok, &ok);
	add_test(list1, "ko", &ok, &ko);
	add_test(list1, "segv", &test1, &test2);
	add_test_list(tester, list1);

	launch_test(tester);
}