#include "malloc_test.h"

int g_malloc_hook_active = 0;
int	g_malloc_crash_active = 0;
t_alloc_list	*alloc_list = NULL;

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
	{
		printf("can't find main in backtrace, did you not setup -rdynamic flag ?\n");
		exit(EXIT_FAILURE);
	}
	size_t main_pos = size - i - 1;

	size_t y = 0;
	for (size_t i = 2; i < size - main_pos; i++)
		trace[y++] = array[i];
	trace[y] = NULL;
}

void	print_trace(void *trace[])
{
	for(int i = 0; trace[i]; i++)
	{
		if(i == 0)
			printf("%s", get_func_name(trace[i]));
		else
			printf(" <-- %s", get_func_name(trace[i]));
	}
	printf("\n");
}


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

t_alloc_list *get_last()
{
	t_alloc_list *elem = alloc_list;
	if(elem == NULL)
		return NULL;
	while(elem->next != NULL)
		elem = elem->next;
	return elem;
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
	printf("===== Leaks Summary =====\n");
	if(leaks == 0)
		printf("No Leaks !\n");
	else
	{
		t_alloc_list *elem = alloc_list;
		while(elem != NULL)
		{
			if(elem->ptr != NULL)
			{
				printf("Leak from: ");
				print_trace(elem->backtrace);
			}
			elem = elem->next;
		}
	}
}

void	sig_handler(int signum)
{
	g_malloc_hook_active = 0;
	g_malloc_crash_active = 0;
	printf("Crash detected, probably your malloc are not protected !\n");
	signal(signum, SIG_DFL);
    kill(getpid(), signum);
}

void	start_malloc_catcher()
{
	g_malloc_hook_active = 1;
	signal(SIGSEGV, &sig_handler);
}

void	stop_malloc_catcher()
{
	g_malloc_hook_active = 0;
	signal(SIGSEGV, SIG_DFL);
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