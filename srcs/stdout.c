/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stdout.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/23 13:21:20 by dhubleur          #+#    #+#             */
/*   Updated: 2022/01/23 13:32:54 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libunit.h"

t_stdout	*init_stdout_test()
{
	t_stdout *ft_stdout = malloc(sizeof(t_stdout));
	int	*fd;

	fd = malloc(sizeof(int) * 2);
	pipe(fd);
	ft_stdout->stdout = dup(STDOUT_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	ft_stdout->fd = fd;
	close(STDERR_FILENO);
	return ft_stdout;
}

static char	*ft_strjoin(char const *s1, char const *s2)
{
	int		s1_len;
	int		s2_len;
	int		i;
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	i = 0;
	s1_len = strlen(s1);
	s2_len = strlen(s2);
	str = (char *)malloc(sizeof(*str) * (s1_len + s2_len) + 1);
	if (!str)
		return (NULL);
	while (*s1)
		str[i++] = *s1++;
	while (*s2)
		str[i++] = *s2++;
	str[i] = 0;
	return (str);
}

static void	get_char(char *buff, t_stdout *ft_stdout, int *ret)
{
	char	*tmp;

	buff[1] = 0;
	*ret = read(ft_stdout->fd[0], buff, 1);
	if (*ret > 0)
	{
		tmp = ft_stdout->output;
		ft_stdout->output = ft_strjoin(ft_stdout->output, buff);
		free(tmp);
	}
}

void	end_stdout_test(t_stdout *ft_stdout)
{
	char	buff[2];
	int		ret;

	close(STDOUT_FILENO);
	ret = 1;
	buff[0] = 'a';
	close (ft_stdout->fd[1]);
	ft_stdout->output = strdup("");
	while (ret > 0 && buff[0])
		get_char(buff, ft_stdout, &ret);
	dup2(ft_stdout->stdout, STDOUT_FILENO);
	free(ft_stdout->fd);
}