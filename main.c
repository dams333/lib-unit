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
	sleep(2);
	return "3";
}

int main()
{
	t_tester *tester = init_tester("Libft Tester");

	t_tests_list *list1 = init_tests_list("ft_strlen");
	add_test(list1, "ok", &ok, &ok);
	add_test(list1, "ko", &ok, &ko);
	add_test(list1, "ko", &ok, &time);
	add_test(list1, "segv", &segv, &ok);
	add_test(list1, "ok", &ok, &ok);
	add_test(list1, "ok", &ok, &ok);
	add_test(list1, "ok", &ok, &ok);
	add_test_list(tester, list1);

	launch_test(tester);
}