# Introduction
This library **is not** the Libunit rush. But an improved version to write unit tests simply.
## RoadMap
- Add norm checker, non protected malloc checker, leaks checker.
- Add the possibility to create tests for undefined behaviors and for acceptable crash.
- Add int test comparison or multiple test comparison
# Documentation
## Compilation
Compile the libunit with its Makefile and use the header (includes/libunit.h) in your tester code
## Usage
### General
- First, initialize your tester with the `init_tester` function by specifying its name
- Initialize a test list for each of the features you will test with the `init_tests_list`
- To add a test to a list, use add_test. Give it the pointer to the list, the name of the test and 2 functions. These functions must return a string.
  - The first function is your test itself.
  - The second function must return what is expected by the first one in order to compare them
  - _(If you want to check the return value and the modification of a variable, concatenate these 2 values in a string)_
  - The test is considered valid if the 2 strings are identical (the comparison is done with `strcmp`)
- The field `code`, can be left at NULL. It will be displayed in the log file in case of KO to give information to the user about the part of his code that caused the error _(no multiple lines)_
- When your test list is finished, add it to your test with `add_test_list`
- You can now start your tester with `launch_test`
---
### STDOUT test
You can test what is written on STDOUT. For this:
- In your test initialise a `t_stdout` with `init_stdout_test`.
- Do what you want that writes on the STDOUT
- Use `end_stdout_test` to finish reading
- You can now retrieve what has been written using the `output` fields of your `t_stdout`.
# Examples
## Basic
This is what your main could look like:
```c
t_tester *tester = init_tester("Libft Tester");

t_tests_list *list1 = init_tests_list("ft_strlen");
add_test(list1, "ok", NULL &test_strlen_ok, &compare_strlen_ok);
add_test(list1, "ko", "ft_strlen(\"abcdefghiklm\")" &test_strlen_ko, &compare_strlen_ko);
add_test_list(tester, list1);

t_tests_list *list2 = init_tests_list("ft_atoi");
add_test(list2, "ok", NULL &test_atoi_ok, &compare_atoi_ok);
add_test(list2, "ko", NULL &test_atoi_ko, &compare_atoi_ko);
add_test_list(tester, list2);

launch_test(tester);
```
The result of such a test will look like this:
```
     Libft Tester     
----------------------

ft_strlen: ✔ ✗
ft_atoi:   ✔ ✗

Global result: (2/4)
```
Associated with a log file like this:
```
     Libft Tester     
----------------------

ft_strlen:
  > ok: OK
  > ko: KO
       EXCEPTED: [12]
       OBTAINED: [1]
	   Created by: [ft_strlen("abcdefghiklm")]

ft_atoi:
  > ok: OK
  > ko: KO
       EXCEPTED: [12]
       OBTAINED: [1]


Global result: (2/4)
```
## STDOUT
This main:
```c
#include "includes/libunit.h"

void ft_putendl(char *str)
{
	write(1, str, strlen(str));
}

char *test()
{
	t_stdout *ft_stdout = init_stdout_test();
	ft_putendl("Salut");
	end_stdout_test(ft_stdout);
	return ft_stdout->output;
}
char *result()
{
	return "Salut\n";
}

int main()
{
	t_tester *tester = init_tester("STDOUT Tester");

	t_tests_list *list1 = init_tests_list("STDOUT");
	add_test(list1, "test ", "ft_putendl(\"Salut\")", &test, &result);
	add_test_list(tester, list1);

	launch_test(tester);
}
```
Produce this output:
```
     STDOUT Tester     
-----------------------

STDOUT: ✗

Global result: (0/1)
```
And this log file:
```
     STDOUT Tester     
-----------------------

STDOUT:
  > test : KO
       EXCEPTED: [Salut\n]
       OBTAINED: [Salut]
       Created by: [ft_putendl("Salut")]


Global result: (0/1)
```