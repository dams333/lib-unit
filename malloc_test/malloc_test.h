#ifndef MALLOC_TEST_H
# define MALLOC_TEST_H

# include <stdlib.h>
# include <stdio.h>
# include <execinfo.h>
# include <errno.h>
# include <fcntl.h>
# include <string.h>
# include <dlfcn.h>

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
	void 	**backtrace;
	struct s_alloc_list *next;
}	t_alloc_list;

extern int g_malloc_hook_active;
extern  void *__libc_malloc(size_t size);
extern  void *__libc_free(void *ptr);

void *malloc(size_t size);
void free(void *ptr);
int	count_no_free();

int 		dladdr(void *address, Dl_info *dlip);

#endif