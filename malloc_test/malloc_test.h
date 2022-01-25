#ifndef MALLOC_TEST_H
# define MALLOC_TEST_H

# include <stdlib.h>
# include <stdio.h>
# include <execinfo.h>
# include <errno.h>
# include <fcntl.h>
# include <string.h>
# include <dlfcn.h>
# include <signal.h>
# include <unistd.h>

typedef struct
{
	const char *dli_fname;
	void *dli_fbase;
	const char *dli_sname;
	void *dli_saddr;
} Dl_info;

typedef struct s_alloc_list
{
	void	*ptr;
	struct s_alloc_list *next;
	void 	**backtrace;
}	t_alloc_list;

extern  void *__libc_malloc(size_t size);
extern  void *__libc_free(void *ptr);
int 		dladdr(void *address, Dl_info *dlip);

void *malloc(size_t size);
void free(void *ptr);

void	start_malloc_catcher();
void	stop_malloc_catcher();
void	stop_malloc_catcher_and_print_leaks();
void	start_malloc_breaker();
void	stop_malloc_breaker();

#endif