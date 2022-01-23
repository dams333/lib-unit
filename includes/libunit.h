/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libunit.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 09:32:14 by dhubleur          #+#    #+#             */
/*   Updated: 2022/01/23 16:43:06 by dhubleur         ###   ########.fr       */
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

# define TIMEOUT 10

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

typedef struct s_test
{
	char			*test_name;
	char			*test_code;
	char			*(*test)(void);
	char			*(*waited)(void);
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
void	add_test(t_tests_list *test_list, char *name, char *code, char *(*ft_test)(void), char *(*waited)(void));

void	launch_test(t_tester *tester);
void	print_result(int wait_status, int *ok_test, t_test *test, int fd);

t_stdout	*init_stdout_test();
void	end_stdout_test(t_stdout *output);

#endif