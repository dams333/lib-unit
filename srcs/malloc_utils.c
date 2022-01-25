/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 14:51:23 by dhubleur          #+#    #+#             */
/*   Updated: 2022/01/25 16:14:59 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libunit.h"

int	g_log_fd;

char const *get_func_name(void *addr)
{
	Dl_info info;
	if (dladdr(addr, &info) && info.dli_sname)
		return (info.dli_sname);
	else
		return ("<static_function>");
}

void get_backtrace(void *trace[])
{
	void *array[103];
	size_t size;

	size = backtrace(array, 103);
	
	size_t i;
	for (i = 0; i < size; i++)
	{
		if (strcmp(get_func_name(array[i]), "main") == 0)
			break;
	}

	if (i == size)
		exit(EXIT_FAILURE);
	size_t main_pos = size - i - 1;

	size_t y = 0;
	for (size_t i = 2; i < size - main_pos; i++)
		trace[y++] = array[i];
	trace[y] = NULL;
}

void	print_trace(void *trace[])
{
	int first = 1;
	for(int i = 0; trace[i]; i++)
	{
		if(strcmp(get_func_name(trace[i]), "<static_function>") != 0)
		{
			if(first)
				ft_printf(g_log_fd, "%s", get_func_name(trace[i]));
			else
				ft_printf(g_log_fd, " <-- %s", get_func_name(trace[i]));
			first = 0;
		}
	}
	ft_printf(g_log_fd, "\n");
}

int	count_no_free()
{
	if(alloc_list == NULL)
		return 0;
	int count = 0;
	t_alloc_list *elem = alloc_list;
	while(elem != NULL)
	{
		if(elem->ptr != NULL)
			count++;
		elem = elem->next;
	}
	return (count);
}

void	print_leak_summary()
{
	int leaks = count_no_free();
	if(leaks != 0 && g_print_leaks)
	{
		ft_printf(g_log_fd, "\n  ===== Leaks Summary =====\n");
		t_alloc_list *elem = alloc_list;
		while(elem != NULL)
		{
			if(elem->ptr != NULL)
			{
				ft_printf(g_log_fd, "    Leak from: ");
				print_trace(elem->backtrace);
			}
			elem = elem->next;
		}
		g_leaks = 1;
	}
	alloc_list = NULL;
}

void	start_malloc_catcher()
{
	g_malloc_hook_active = 1;
}

void	stop_malloc_catcher()
{
	g_malloc_hook_active = 0;
}

void	stop_malloc_catcher_and_print_leaks()
{
	stop_malloc_catcher();
	print_leak_summary();
}

void	start_malloc_breaker()
{
	g_malloc_crash_active = 1;
}

void	stop_malloc_breaker()
{
	g_malloc_crash_active = 0;
}