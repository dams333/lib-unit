/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 11:28:58 by dhubleur          #+#    #+#             */
/*   Updated: 2022/01/23 11:49:54 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libunit.h"

t_tester	*init_tester(char *tester_name)
{	
	t_tester	*tester;

	tester = malloc(sizeof(tester));
	tester->name = tester_name;
	tester->first_list = NULL;
	return (tester);
}

t_tests_list	*init_tests_list(char *list_name)
{	
	t_tests_list	*test_list;

	test_list = malloc(sizeof(test_list));
	test_list->list_name = list_name;
	test_list->first_test = NULL;
	return (test_list);
}

static t_test	*create_test(char *description, char *(*ft_test)(void), char *(*waited)(void))
{
	t_test	*test_elem;

	test_elem = malloc(sizeof(t_test));
	test_elem->test_desc = description;
	test_elem->test = ft_test;
	test_elem->waited = waited;
	test_elem->next_test = NULL;
	return (test_elem);
}

void	add_test(t_tests_list *test_list, char *description, char *(*ft_test)(void), char *(*waited)(void))
{
	t_test	*elem;

	if (test_list->first_test == NULL)
		test_list->first_test = create_test(description, ft_test, waited);
	else
	{
		elem = test_list->first_test;
		while (elem->next_test != NULL)
			elem = elem->next_test;
		elem->next_test = create_test(description, ft_test, waited);
	}
}

void	add_test_list(t_tester *tester, t_tests_list *list)
{
	t_tests_list	*elem;

	if (tester->first_list == NULL)
		tester->first_list = list;
	else
	{
		elem = tester->first_list;
		while (elem->next_tests != NULL)
			elem = elem->next_tests;
		elem->next_tests = list;
	}
}