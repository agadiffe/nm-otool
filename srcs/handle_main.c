/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadiffe <agadiffe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/23 00:52:03 by agadiffe          #+#    #+#             */
/*   Updated: 2018/04/23 19:49:39 by agadiffe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
#include "libft.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int				handle_arch(char *ptr, char *av, int print, int is_nm)
{
	unsigned int	magic_nbr;

	magic_nbr = *(unsigned int *)ptr;
	if (magic_nbr == MH_MAGIC_64 || magic_nbr == MH_CIGAM_64
			|| magic_nbr == MH_MAGIC || magic_nbr == MH_CIGAM)
	{
		if (is_nm)
			return (handle_32_64_nm(ptr, av, print, is_nm));
		else
			return (handle_32_64_otool(ptr, av, print, is_nm));
	}
	else if (magic_nbr == FAT_MAGIC || magic_nbr == FAT_CIGAM
			|| magic_nbr == FAT_MAGIC_64 || magic_nbr == FAT_CIGAM_64)
		return (handle_fat(ptr, av, is_nm));
	else if (!ft_strncmp(ptr, ARMAG, SARMAG))
		return (handle_ar(ptr, av, print, is_nm));
	else
		ft_putendl_fd("Invalid Architecture", 2);
	return (1);
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
	int		ret;
	char	*ptr;
	size_t	st_size;

	ret = 0;
	i = 0;
	while (++i < ac)
	{
		if (ac > 2)
			get_ac(1);
		if ((ret += handle_file(av[i], &ptr, &st_size)))
			continue ;
		ret += handle_arch(ptr, av[i], 0, is_nm);
		if (i + 1 < ac)
			ft_putendl("");
		if (munmap(ptr, st_size) < 0)
		{
			ret += ft_error_ret("Error: munmap() failed", 1);
			continue ;
		}
		get_arch_tab_printed(1, 1, 0);
		is_ar(1, 0);
	}
	return (ret > 0);
}
