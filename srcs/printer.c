/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 14:43:16 by dhubleur          #+#    #+#             */
/*   Updated: 2022/01/24 14:23:19 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libunit.h"

static int	count_chars(int n)
{
	long int	nb;
	int			chars;

	nb = n;
	chars = 1;
	if (n < 0)
	{
		chars++;
		nb *= -1;
	}
	while (nb >= 10)
	{
		nb /= 10;
		chars++;
	}
	return (chars);
}

static char	*itoa(int n)
{
	long int	nb;
	char		*str;
	int			i;

	str = malloc(sizeof(char) + (count_chars(n) + 1));
	if (!str)
		return (NULL);
	str[count_chars(n)] = '\0';
	i = count_chars(n) - 1;
	nb = n;
	if (n < 0)
	{
		str[0] = '-';
		nb *= -1;
	}
	while (nb >= 10)
	{
		str[i] = (nb % 10) + '0';
		nb /= 10;
		i--;
	}
	str[i] = nb + '0';
	return (str);
}

void	print_signal_error(int exit_code, t_test *test, int fd)
{
	int	sig_code;

	ft_printf(1, "%s", RED);
	ft_printf(fd, "  > %s: ", test->test_name);
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
	if(test->test_code != NULL)
				ft_printf(fd, "       Created by: [%s]\n", test->test_code);
	ft_printf(fd, "\n");
}

char	*replace_chars(char *str)
{
	int count = 0;
	for(int i = 0; str[i]; i++)
	{
		if(str[i] == '\a' || str[i] == '\b' || str[i] == '\n' || str[i] == '\v' || str[i] == '\f' || str[i] == '\r' || str[i] == '\t')
			count++;
	}
	char *new_str = malloc(sizeof(char) * (strlen(str) + count));
	int	new_i = 0;
	for(int i = 0; str[i]; i++)
	{
		if(str[i] == '\a')
		{
			new_str[new_i++] = '\\';
			new_str[new_i] = 'a';
		}
		else if(str[i] == '\b')
		{
			new_str[new_i++] = '\\';
			new_str[new_i] = 'b';
		}
		else if(str[i] == '\n')
		{
			new_str[new_i++] = '\\';
			new_str[new_i] = 'n';
		}
		else if(str[i] == '\v')
		{
			new_str[new_i++] = '\\';
			new_str[new_i] = 'v';
		}
		else if(str[i] == '\f')
		{
			new_str[new_i++] = '\\';
			new_str[new_i] = 'f';
		}
		else if(str[i] == '\r')
		{
			new_str[new_i++] = '\\';
			new_str[new_i] = 'r';
		}
		else if(str[i] == '\t')
		{
			new_str[new_i++] = '\\';
			new_str[new_i] = 't';
		}
		else if(str[i] < ' ' || str[i] > '~')
			new_str[i] = '.';
		else
			new_str[new_i] = str[i];
		new_i++;
	}
	new_str[new_i] = '\0';
	return new_str;
}

void	print_result(int wait_status, int *ok_test, t_test *test, int fd)
{
	if (WIFEXITED(wait_status))
	{
		if (WEXITSTATUS(wait_status) == 0)
		{
			ft_printf(1, "%s✔%s ", GREEN, RESET);
			ft_printf(fd, "  > %s: OK\n", test->test_name);
			(*ok_test)++;
		}
		else if (WEXITSTATUS(wait_status) == SIGALRM)
		{
			ft_printf(1, "%s[TIMEOUT]%s ", RED, RESET);
			ft_printf(fd, "  > %s: TIMEOUT\n", test->test_name);
			if(test->test_code != NULL)
				ft_printf(fd, "       Created by: [%s]\n", test->test_code);
		}
		else
		{
			ft_printf(1, "%s✗%s ", RED, RESET);
			ft_printf(fd, "  > %s: KO\n", test->test_name);
			char *str1;
			char *str2;
			if(test->type == INT_VALUE)
			{
				str1 = strdup(itoa(*((int *)test->compare)));
				int (*fct)(void);
				fct = test->test;
				str2 = strdup(itoa(fct()));
			}
			if(test->type == STR_VALUE)
			{
				str1 = strdup(((char *)test->compare));
				char *(*fct)(void);
				fct = test->test;
				str2 = strdup(fct());
			}
			if(test->type == INT_COMPARE)
			{
				int (*fct2)(void);
				fct2 = test->compare;
				str1 = strdup(itoa(fct2()));
				int (*fct)(void);
				fct = test->test;
				str2 = strdup(itoa(fct()));
			}
			if(test->type == STR_COMPARE)
			{
				char *(*fct2)(void);
				fct2 = test->compare;
				str1 = strdup(fct2());
				char *(*fct)(void);
				fct = test->test;
				str2 = strdup(fct());
			}
			ft_printf(fd, "       EXCEPTED: [%s]\n", replace_chars(str1));
			ft_printf(fd, "       OBTAINED: [%s]\n", replace_chars(str2));
			if(test->test_code != NULL)
				ft_printf(fd, "       Created by: [%s]\n", test->test_code);
		}
	}
	else
	{
		print_signal_error(wait_status, test, fd);
	}
}

void	print_header()
{
	ft_printf(1, "%s", MAGENTA);
	ft_printf(1, "Created with the help of the\n");
	ft_printf(1, "  _      _ _     _    _       _ _   \n");
	ft_printf(1, " | |    (_) |   | |  | |     (_) |  \n");
	ft_printf(1, " | |     _| |__ | |  | |_ __  _| |_ \n");
	ft_printf(1, " | |    | | '_ \\| |  | | '_ \\| | __|\n");
	ft_printf(1, " | |____| | |_) | |__| | | | | | |_ \n");
	ft_printf(1, " |______|_|_.__/ \\____/|_| |_|_|\\__|\n");
	ft_printf(1, "                          by dhubleur\n");
	ft_printf(1, "\n%s", RESET);
}