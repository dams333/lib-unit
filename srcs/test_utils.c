/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 11:28:58 by dhubleur          #+#    #+#             */
/*   Updated: 2022/01/24 12:37:55 by dhubleur         ###   ########.fr       */
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

static t_test	*create_test(char *name, char *code, t_bool accept_crash, t_testtype type, void *test, void *compare)
{
	t_test	*test_elem;

	test_elem = malloc(sizeof(t_test));
	test_elem->test_name = name;
	test_elem->test_code = code;
	test_elem->test = test;
	test_elem->compare = compare;
	test_elem->accept_crash = accept_crash;
	test_elem->next_test = NULL;
	return (test_elem);
}

void	add_test(t_tests_list *test_list, char *name, char *code, t_bool accept_crash, t_testtype type, void *test, ...)
{
	t_test	*elem;
	va_list ap;
	void	*compare;

	va_start(ap, test);
	if(type == INT_COMPARE)
	{
		int	(**fct)(void) = malloc(sizeof(int(**)(void)));
		*fct = va_arg(ap, int(**)(void));
		compare = *fct;
	}
	if(type == INT_COMPARE)
	{
		char	(***fct)(void) = malloc(sizeof(int(***)(void)));
		*fct = va_arg(ap, char(***)(void));
		compare = *fct;
	}
	if(type == INT_VALUE)
	{
		int	*i = malloc(sizeof(int));
		*i = va_arg(ap, int);
	}
	if(type == STR_VALUE)
	{
		char *str = va_arg(ap, char*);
		char *copy = strdup(str);
		compare = copy;
	}
	va_end(ap);
	if (test_list->first_test == NULL)
		test_list->first_test = create_test(name, code, accept_crash, type, test, compare);
	else
	{
		elem = test_list->first_test;
		while (elem->next_test != NULL)
			elem = elem->next_test;
		elem->next_test = create_test(name, code, accept_crash, type, test, compare);
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