/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 14:43:16 by dhubleur          #+#    #+#             */
/*   Updated: 2022/01/23 12:41:28 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libunit.h"

void	print_signal_error(int exit_code)
{
	int	sig_code;

	ft_printf("%s", RED);
	sig_code = WTERMSIG(exit_code);
	if (sig_code == SIGBUS)
		ft_printf("[SIGBUS]");
	else if (sig_code == SIGSEGV)
		ft_printf("[SIGSEGV]");
	else if (sig_code == SIGFPE)
		ft_printf("[SIGFPE]");
	else if (sig_code == SIGPIPE)
		ft_printf("[SIGPIPE]");
	else if (sig_code == SIGILL)
		ft_printf("[SIGILL]");
	else if (sig_code == SIGABRT)
		ft_printf("[SIGABRT]");
	else
		ft_printf("[KILLED]");
	ft_printf("%s ", RESET);
}

void	print_result(int wait_status, int *ok_test)
{
	if (WIFEXITED(wait_status))
	{
		if (WEXITSTATUS(wait_status) == 0)
		{
			ft_printf("%s✅%s ", GREEN, RESET);
			(*ok_test)++;
		}
		else if (WEXITSTATUS(wait_status) == SIGALRM)
		{
			ft_printf("%s[TIMEOUT]%s ", RED, RESET);
		}
		else
		{
			ft_printf("%s❌%s ", RED, RESET);
		}
	}
	else
	{
		print_signal_error(wait_status);
	}
}