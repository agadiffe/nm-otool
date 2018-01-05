#include "ft_nm.h"
#include "libft.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

void	ft_nm(char *ptr)
{
	unsigned int	magic_nbr;

	magic_nbr = *(unsigned int *)ptr;
	if (magic_nbr == MH_MAGIC_64)
		handle_32_64(ptr, X64);
}

int		main(int ac, char **av)
{
	int				fd;
	char			*ptr;
	struct stat		buf;

	if (ac != 2)
		ft_error_ret("no arg!", EXIT_FAILURE);
	if ((fd = open(av[1], O_RDONLY)) < 0)
		ft_error_ret("open error!", EXIT_FAILURE);
	if (fstat(fd, &buf) < 0)
		ft_error_ret("fstat error!", EXIT_FAILURE);
	if ((ptr = mmap(0, buf.st_size, PROT, MAP, fd, 0)) == MAP_FAILED)
		ft_error_ret("mmap error!", EXIT_FAILURE);
	ft_nm(ptr);
	if (munmap(ptr, buf.st_size) < 0)
		ft_error_ret("munmap error!", EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
