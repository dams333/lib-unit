# Introduction
This library **is not** the Libunit rush. But an improved version to write unit tests simply.
## Version
The current version is relatively simple but new features will be added very soon
# Documentation
## Compilation
Compile the libunit with its Makefile and use the header (includes/libunit.h) in your tester code
## Use
### General
- First, initialize your tester with the `init_tester` function by specifying its name
- Initialize a test list for each of the features you will test with the `init_tests_list`
- To add a test to a list, use add_test. Give it the pointer to the list, the name of the test and 2 functions. These functions must return a string.
  - The first function is your test itself.
  - The second function must return what is expected by the first one in order to compare them
  - _(If you want to check the return value and the modification of a variable, concatenate these 2 values in a string)_
  - The test is considered valid if the 2 strings are identical (the comparison is done with `strcmp`)
- When your test list is finished, add it to your test with `add_test_list`
- You can now start your tester with `launch_test`
---
### STDOUT test
You can test what is written on STDOUT. For this:
- In your test initialise a `t_stdout` with `init_stdout_test`.
- Do what you want that writes on the STDOUT
- Use `end_stdout_test` to finish reading
- You can now retrieve what has been written using the `output` fields of your `t_stdout`.
## Example
This is what your main could look like:
```c
t_tester *tester = init_tester("Libft Tester");

t_tests_list *list1 = init_tests_list("ft_strlen");
add_test(list1, "ok", &test_strlen_ok, &compare_strlen_ok);
add_test(list1, "ko", &test_strlen_ko, &compare_strlen_ko);
add_test_list(tester, list1);

t_tests_list *list2 = init_tests_list("ft_atoi");
add_test(list2, "ok", &test_atoi_ok, &compare_atoi_ok);
add_test(list2, "ko", &test_atoi_ko, &compare_atoi_ko);
add_test_list(tester, list2);

launch_test(tester);
```
The result of such a test will look like this:
```
     Libft Tester     
----------------------

ft_strlen: ✅ ❌
ft_atoi: ✅ ❌

Global result: (2/4)
```