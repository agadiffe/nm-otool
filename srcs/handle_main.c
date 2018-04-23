/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadiffe <agadiffe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/23 00:52:03 by agadiffe          #+#    #+#             */
/*   Updated: 2018/04/23 18:05:28 by agadiffe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
#include "libft.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

void			handle_arch(char *ptr, char *av, int print, int is_nm)
{
	unsigned int	magic_nbr;

	magic_nbr = *(unsigned int *)ptr;
	if (magic_nbr == MH_MAGIC_64 || magic_nbr == MH_CIGAM_64
			|| magic_nbr == MH_MAGIC || magic_nbr == MH_CIGAM)
	{
		if (is_nm)
			handle_32_64_nm(ptr, av, print, is_nm);
		else
			handle_32_64_otool(ptr, av, print, is_nm);
	}
	else if (magic_nbr == FAT_MAGIC || magic_nbr == FAT_CIGAM
			|| magic_nbr == FAT_MAGIC_64 || magic_nbr == FAT_CIGAM_64)
		handle_fat(ptr, av, is_nm);
	else if (!ft_strncmp(ptr, ARMAG, SARMAG))
		handle_ar(ptr, av, print, is_nm);
	else
		ft_putendl_fd("Invalid Architecture", 2);
}

static int		handle_file(char *av, char **ptr, size_t *st_size)
{
	int				fd;
	struct stat		buf;
	void			**max_addr;
	void			**origin_addr;

	if ((fd = open(av, O_RDONLY)) < 0)
		return (ft_error_ret("Error: Can't open file", 1));
	if (fstat(fd, &buf) < 0)
	{
		close(fd);
		return (ft_error_ret("Error: fstat() failed", 1));
	}
	*st_size = buf.st_size;
	if ((*ptr = mmap(0, buf.st_size, PROT, MAP, fd, 0)) == MAP_FAILED)
	{
		close(fd);
		return (ft_error_ret("Error: Invalid file", 1));
	}
	close(fd);
	max_addr = get_max_addr();
	origin_addr = get_origin_addr();
	*max_addr = (void *)*ptr + buf.st_size;
	*origin_addr = (void *)*ptr;
	return (0);
}

int				handle_main(int ac, char **av, int is_nm)
{
	int		i;
	char	*ptr;
	size_t	st_size;

	i = 0;
	while (++i < ac)
	{
		if (ac > 2)
			get_ac(1);
		if (handle_file(av[i], &ptr, &st_size))
			continue ;
		handle_arch(ptr, av[i], 0, is_nm);
		if (i + 1 < ac)
			ft_putendl("");
		if (munmap(ptr, st_size) < 0)
		{
			ft_putendl_fd("Error: munmap() failed", 2);
			continue ;
		}
		get_arch_tab_printed(1, 1, 0);
		is_ar(1, 0);
	}
	return (0);
}
