/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libunit.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 09:32:14 by dhubleur          #+#    #+#             */
/*   Updated: 2022/01/25 16:13:41 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBUNIT_BONUS_H
# define LIBUNIT_BONUS_H

# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <stdio.h>
# include <signal.h>
# include <string.h>
# include <time.h>
# include <fcntl.h>
# include <string.h>
# include <stdarg.h>
# include <execinfo.h>
# include <errno.h>
# include <dlfcn.h>

# define TIMEOUT 1

# define RED "\033[31m" 
# define GREEN "\033[32m" 
# define YELLOW "\033[33m" 
# define BLUE "\033[34m" 
# define MAGENTA "\033[35m" 
# define CYAN "\033[36m" 
# define RESET "\033[0m" 

int		ft_printf(int fd, const char *str, ...);

/*
 * 	Structur
 */

typedef enum e_testtype
{
	INT_VALUE,
	STR_VALUE,
	INT_COMPARE,
	STR_COMPARE
}	t_testtype;
typedef enum e_bool
{
	TRUE = 1,
	FALSE = 0
}	t_bool;

typedef struct s_test
{
	char			*test_name;
	char			*test_code;
	t_testtype		type;
	void			*test;
	void			*compare;
	t_bool			accept_crash;
	struct s_test	*next_test;
}	t_test;

typedef struct s_tests_list
{
	char	*list_name;
	t_test	*first_test;
	struct s_tests_list *next_tests;
}	t_tests_list;

typedef	struct s_tester
{
	char	*name;
	t_tests_list *first_list;
}	t_tester;

typedef struct s_stdout
{
	char		*output;
	int			*fd;
	int			stdout;
}				t_stdout;

/*
 *	unit_test
 */
t_tester		*init_tester(char *tester_name);
void	add_test_list(t_tester *tester, t_tests_list *list);

t_tests_list	*init_tests_list(char *list_name);
void	add_test(t_tests_list *test_list, char *name, char *code, t_bool accept_crash, t_testtype type, void *test, ...);

void	launch_test(t_tester *tester);
void	print_result(int wait_status, int *ok_test, t_test *test, int fd);
void	print_header();

t_stdout	*init_stdout_test();
void	end_stdout_test(t_stdout *output);

typedef struct
{
	const char *dli_fname;
	void *dli_fbase;
	const char *dli_sname;
	void *dli_saddr;
} Dl_info;

typedef struct s_alloc_list
{
	void	*ptr;
	struct s_alloc_list *next;
	void 	**backtrace;
}	t_alloc_list;

extern int g_malloc_hook_active;
extern int	g_malloc_crash_active;
extern t_alloc_list	*alloc_list;
extern int	g_log_fd;
extern int	g_leaks;
extern int	g_print_leaks;

extern  void *__libc_malloc(size_t size);
extern  void *__libc_free(void *ptr);
int 		dladdr(void *address, Dl_info *dlip);

void *malloc(size_t size);
void free(void *ptr);

void	start_malloc_catcher();
void	stop_malloc_catcher();
void	stop_malloc_catcher_and_print_leaks();
void	start_malloc_breaker();
void	stop_malloc_breaker();

void get_backtrace(void *trace[]);


#endif