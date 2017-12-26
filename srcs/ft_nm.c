#include "ft_nm.h"
#include "libft.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "loader.h"
#include "nlist.h"

void	print_out(int nsyms, int symoff, int stroff, char *ptr)
{
	int					i;
	char				*string_table;
	struct nlist_64		*array;

	array = (void *) ptr + symoff;
	string_table = (void *) ptr + stroff;
	i = -1;
	while (++i < nsyms)
	{
		ft_putendl(string_table + array[i].n_un.n_strx);
	}
}

void	handle_64(char *ptr)
{
	int						i;
	int						ncmds;
	struct mach_header_64	*header;
	struct load_command		*lc;
	struct symtab_command	*sym;

	header = (struct mach_header_64 *)ptr;
	ncmds = header->ncmds;
	lc = (void *) ptr + sizeof(*header);
	i = -1;
	while (++i < ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *)lc;
			print_out(sym->nsyms, sym->symoff, sym->stroff, ptr);
			break ;
		}
		lc = (void *) lc + lc->cmdsize;
	}
}

void	ft_nm(char *ptr)
{
	unsigned int	magic_nbr;

	magic_nbr = *(unsigned int *)ptr;
	if (magic_nbr == MH_MAGIC_64)
		handle_64(ptr);
}

int		main(int ac, char **av)
{
	int				fd;
	char			*ptr;
	struct stat		buf;

	if (ac != 2)
		ft_error("no arg!", EXIT_FAILURE);
	if ((fd = open(av[1], O_RDONLY)) < 0)
		ft_error("open error!", EXIT_FAILURE);
	if (fstat(fd, &buf) < 0)
		ft_error("fstat error!", EXIT_FAILURE);
	if ((ptr = mmap(0, buf.st_size, PROT, MAP, fd, 0)) == MAP_FAILED)
		ft_error("mmap error!", EXIT_FAILURE);
	ft_nm(ptr);
	if (munmap(ptr, buf.st_size) < 0)
		ft_error("munmap error!", EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
