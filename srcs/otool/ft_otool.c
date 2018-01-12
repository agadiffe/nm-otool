#include "ft_nm.h"
#include "libft.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

void			handle_arch(char *ptr)
{
	unsigned int	magic_nbr;

	magic_nbr = *(unsigned int *)ptr;
	if (magic_nbr == MH_MAGIC_64)
		handle_32_64(ptr, X64);
	else if (magic_nbr == MH_MAGIC)
		handle_32_64(ptr, X86);
	else if (magic_nbr == FAT_MAGIC)
		handle_fat(ptr, B_ENDIAN);
	else if (magic_nbr == FAT_CIGAM)
		handle_fat(ptr, L_ENDIAN);
	else
		ft_putendl_fd("Invalid Architecture", 2);
}

static int		handle_av(int ac, char **av)
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
		handle_arch(ptr);
		if (munmap(ptr, buf.st_size) < 0)
			return (ft_error_ret("munmap error!", EXIT_FAILURE));
	}
	return (EXIT_SUCCESS);
}

int				main(int ac, char **av)
{

	if (ac < 2)
		return (ft_error_ret("no argument!", EXIT_FAILURE));
	return (handle_av(ac, av));
}
