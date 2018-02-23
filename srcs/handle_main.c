#include "ft_nm.h"
#include "libft.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

int				is_not_terminated_string(char *s)
{
	while ((void *)s < g_max_addr)
	{
		if (*s == '\0')
			return (0);
		s++;
	}
	return (ft_error_ret("file data error", 1));
}

int				is_invalid_addr(void *to_check)
{
	if (to_check > g_max_addr)
		return (ft_error_ret("file data error", 1));
	return (0);
}

void			handle_arch(char *ptr, char *av)
{
	unsigned int	magic_nbr;

	magic_nbr = *(unsigned int *)ptr;
	if (magic_nbr == MH_MAGIC_64 || magic_nbr == MH_CIGAM_64)
		handle_32_64(ptr, X64);
	else if (magic_nbr == MH_MAGIC || magic_nbr == MH_CIGAM)
		handle_32_64(ptr, X86);
	else if (magic_nbr == FAT_MAGIC || magic_nbr == FAT_CIGAM)
		handle_fat(ptr);
	else if (!ft_strncmp(ptr, ARMAG, SARMAG))
		handle_ar(ptr, av);
	else
		ft_putendl_fd("Invalid Architecture", 2);
}

int				handle_main(int ac, char **av)
{
	int				i;
	int				fd;
	char			*ptr;
	struct stat		buf;

	i = 0;
	while (++i < ac)
	{
		ft_putstr(av[i]);
		ft_putendl(":");
		if ((fd = open(av[i], O_RDONLY)) < 0)
			return (ft_error_ret("open error!", EXIT_FAILURE));
		if (fstat(fd, &buf) < 0)
			return (ft_error_ret("fstat error!", EXIT_FAILURE));
		if ((ptr = mmap(0, buf.st_size, PROT, MAP, fd, 0)) == MAP_FAILED)
			return (ft_error_ret("mmap error!", EXIT_FAILURE));
		g_max_addr = (void *)ptr + buf.st_size;
		handle_arch(ptr, av[i]);
		if (i + 1 < ac)
			ft_putendl("");
		if (munmap(ptr, buf.st_size) < 0)
			return (ft_error_ret("munmap error!", EXIT_FAILURE));
	}
	return (EXIT_SUCCESS);
}
