/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_hook.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 14:49:14 by dhubleur          #+#    #+#             */
/*   Updated: 2022/01/25 14:51:00 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libunit.h"

int g_malloc_hook_active = 0;
int	g_malloc_crash_active = 0;
t_alloc_list	*alloc_list = NULL;

void	add_alloc_elem(void *ptr, void *backtrace[100])
{
	t_alloc_list *new_elem = malloc(sizeof(t_alloc_list));
	
	new_elem->ptr = ptr;
	new_elem->backtrace = backtrace;
	new_elem->next = NULL;
	if(alloc_list == NULL)
		alloc_list = new_elem;
	else
	{
		t_alloc_list *elem = alloc_list;
		while(elem->next != NULL)
			elem = elem->next;
		elem->next = new_elem;
	}
}

void	remove_alloc_elem(void *ptr)
{
	t_alloc_list *elem = alloc_list;
	if(elem->ptr == ptr)
		alloc_list = elem->next;
	else
	{
		while(elem->next->ptr != ptr)
			elem = elem->next;
		elem->next = elem->next->next;
	}
}

void *alloc_hook(size_t size)
{
	void *result;
	void *route[100];

	g_malloc_hook_active = 0;
	get_backtrace(route);
	if(g_malloc_crash_active)
		result = NULL;
	else
		result = malloc(size);
	add_alloc_elem(result, route);
	g_malloc_hook_active = 1;
	return result;
}

void *malloc(size_t size)
{
	if (g_malloc_hook_active)
		return (alloc_hook(size));
	return (__libc_malloc(size));
}

void free_hook(void *ptr)
{
	(void)ptr;
	g_malloc_hook_active = 0;
	if(ptr != NULL)
		remove_alloc_elem(ptr);
	g_malloc_hook_active = 1;
}

void free(void *ptr)
{
	if (g_malloc_hook_active)
		free_hook(ptr);
	__libc_free(ptr);
}