/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 14:43:16 by dhubleur          #+#    #+#             */
/*   Updated: 2022/01/23 16:37:59 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libunit.h"

void	print_signal_error(int exit_code, t_test *test, int fd)
{
	int	sig_code;

	ft_printf(1, "%s", RED);
	ft_printf(fd, "  > %s: ", test->test_desc);
	sig_code = WTERMSIG(exit_code);
	if (sig_code == SIGBUS)
	{
		ft_printf(1, "[SIGBUS]");
		ft_printf(fd, "[SIGBUS]");
	}
	else if (sig_code == SIGSEGV)
	{
		ft_printf(1, "[SIGSEGV]");
		ft_printf(fd, "[SIGSEGV]");
	}
	else if (sig_code == SIGFPE)
	{
		ft_printf(1, "[SIGFPE]");
		ft_printf(fd, "[SIGFPE]");
	}
	else if (sig_code == SIGPIPE)
	{
		ft_printf(1, "[SIGPIPE]");
		ft_printf(fd, "[SIGPIPE]");
	}
	else if (sig_code == SIGILL)
	{
		ft_printf(1, "[SIGILL]");
		ft_printf(fd, "[SIGILL]");
	}
	else if (sig_code == SIGABRT)
	{
		ft_printf(1, "[SIGABRT]");
		ft_printf(fd, "[SIGABRT]");
	}
	else
	{
		ft_printf(1, "[SIGNAL]");
		ft_printf(fd, "[SIGNAL]");
	}
	ft_printf(1, "%s ", RESET);
	ft_printf(fd, "\n");
}

void	print_result(int wait_status, int *ok_test, t_test *test, int fd)
{
	if (WIFEXITED(wait_status))
	{
		if (WEXITSTATUS(wait_status) == 0)
		{
			ft_printf(1, "%s✔%s ", GREEN, RESET);
			ft_printf(fd, "  > %s: OK\n", test->test_desc);
			(*ok_test)++;
		}
		else if (WEXITSTATUS(wait_status) == SIGALRM)
		{
			ft_printf(1, "%s[TIMEOUT]%s ", RED, RESET);
			ft_printf(fd, "  > %s: TIMEOUT\n", test->test_desc);
		}
		else
		{
			ft_printf(1, "%s✗%s ", RED, RESET);
			ft_printf(fd, "  > %s: KO\n", test->test_desc);
			ft_printf(fd, "       EXCEPTED: [%s]\n", test->waited());
			ft_printf(fd, "       OBTAINED: [%s]\n", test->test());
		}
	}
	else
	{
		print_signal_error(wait_status, test, fd);
	}
}