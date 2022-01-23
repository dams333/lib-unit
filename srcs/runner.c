/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 11:44:43 by dhubleur          #+#    #+#             */
/*   Updated: 2022/01/23 17:56:41 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libunit.h"

static void	exit_timeout(int signal)
{
	(void)signal;
	exit(SIGALRM);
}

static void	exec_child(t_test *test)
{
	char		*(*ft_test)(void);
	char		*(*ft_check)(void);

	ft_test = test->test;
	ft_check = test->waited;
	signal(SIGALRM, &exit_timeout);
	alarm(TIMEOUT);
	if(strcmp(ft_test(), ft_check()) == 0)
		exit(0);
	exit(-1);
}

static void	exec_list(t_tests_list *lst, int *total_test, int *ok_test, size_t longest_name, int fd)
{
	int		wait_status;
	pid_t	pid;

	int space = longest_name - strlen(lst->list_name);
	ft_printf(1, "%s%s: %s", YELLOW, lst->list_name, RESET);
	ft_printf(fd, "%s:\n", lst->list_name);
	for(int i = 0; i < space; i++)
		ft_printf(1, " ");

	t_test *test = lst->first_test;
	while (test != NULL)
	{
		pid = fork();
		if (pid == 0)
			exec_child(test);
		else
		{	
			wait(&wait_status);
			print_result(wait_status, ok_test, test, fd);
			(*total_test)++;
		}
		test = test->next_test;
	}
}

void	launch_test(t_tester *tester)
{
	time_t t;
    time(&t);
	char *name = malloc(200);
	name[0] = '\0';
	strcat(name, tester->name);
	strcat(name, "_");
	strcat(name, ctime(&t));
	for(int i = 0; name[i]; i++)
	{
		if(name[i] == ' ')
			name[i] = '_';
		if(name[i] == ':')
			name[i] = '_';
		if(name[i] == '\n')
			name[i] = '\0';
	}
	strcat(name, ".log");
	int fd = open(name, O_WRONLY | O_CREAT);
	ft_printf(1, "%s     %s     %s\n", BLUE, tester->name, RESET);
	ft_printf(fd, "     %s     \n", tester->name);
	int space = strlen(tester->name) + 10;
	ft_printf(1, "%s", BLUE);
	for(int i = 0; i < space; i++)
	{
		ft_printf(1, "-");
		ft_printf(fd, "-");
	}
	ft_printf(1, "%s\n\n", RESET);
	ft_printf(fd, "\n\n");

	int total_test = 0;
	int	ok_test = 0;
	t_tests_list *list = tester->first_list;
	size_t longest = 0;
	while(list != NULL)
	{
		if(strlen(list->list_name) > longest)
			longest = strlen(list->list_name);
		list = list->next_tests;
	}
	list = tester->first_list;
	while(list != NULL)
	{
		exec_list(list, &total_test, &ok_test, longest, fd);
		ft_printf(1, "\n");
		ft_printf(fd, "\n");
		list = list->next_tests;
	}
	ft_printf(1, "\n%sGlobal result: %s(%i/%i)%s\n", CYAN, (total_test == ok_test) ? GREEN : RED, ok_test, total_test, RESET);
	ft_printf(fd, "\nGlobal result: (%i/%i)", ok_test, total_test);
	close(fd);
}
