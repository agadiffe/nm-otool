#include "ft_nm.h"
#include "libft.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

/*
 **	U (undefined)
 **	A (absolute)
 **	C (common  symbol)
 **	I (indirect symbol)
 **	T (text section symbol)
 **	D (data section symbol)
 **	B  (bss  section  symbol)
 **	S (symbol in a section other than those above)
 */

int		cmp_sect_name(char *sect, t_ndata *ndata, int is_64)
{
	if (is_64)
		return (ft_strcmp(sect, ((t_sect64 *)ndata->lc)->sectname));
	else
		return (ft_strcmp(sect, ((t_sect32 *)ndata->lc)->sectname));
}

int		handle_sect(char *sect, t_ndata *ndata, uint8_t *count, int is_64)
{
	uint32_t		i;
	uint32_t		nsects;

	(void)is_64;
	i = 0;
	nsects = is_64 ? ((t_seg64 *)ndata->lc)->nsects
					: ((t_seg32 *)ndata->lc)->nsects;
	ndata->lc = is_64 ? (void *)ndata->lc + sizeof(t_seg64)
					: (void *)ndata->lc + sizeof(t_seg32);
	while (i != nsects)
	{
		if (*count == ndata->n_sect && !cmp_sect_name(sect, ndata, is_64))
			return (1);
		ndata->lc = is_64 ? (void *)ndata->lc + sizeof(t_sect64)
						: (void *)ndata->lc + sizeof(t_sect32);
		(*count)++;
		++i;
	}
	return (0);
}

int		handle_seg(char *sect, t_ndata *ndata, int is_64)
{
	uint32_t	i;
	int			ret;
	uint8_t		count;

	i = 0;
	count = 1;
	ret = 0;
	while (i != ndata->ncmds)
	{
		if (ndata->lc->cmd == LC_SEGMENT_64 || ndata->lc->cmd == LC_SEGMENT)
		{
			if ((ret = handle_sect(sect, ndata, &count, is_64)) == 1)
				break ;
		}
		ndata->lc = (void *)ndata->lc + ndata->lc->cmdsize;
		i++;
	}
	return (ret);
}

char	get_type_sect(t_ndata *ndata, int is_64)
{
	unsigned char	c;

	if (handle_seg(SECT_TEXT, ndata, is_64) == 1)
		c = 't';
	if (handle_seg(SECT_DATA, ndata, is_64) == 1)
		c = 'd';
	if (handle_seg(SECT_BSS, ndata, is_64) == 1)
		c = 'b';
	else
		c = 's';
	return (c);
}

char	get_type(void *array, uint32_t i, void *ptr, int is_64)
{
	unsigned char	c;
	t_ndata			ndata;

	ndata.n_type = is_64 ? NLIST64[i].n_type : NLIST32[i].n_type;
	ndata.n_value = is_64 ? NLIST64[i].n_value : NLIST32[i].n_value;
	ndata.n_sect = is_64 ? NLIST64[i].n_sect : NLIST32[i].n_sect;
	ndata.lc = is_64 ? (void *)ptr + sizeof(t_header64)
					: (void *)ptr + sizeof(t_header32);
	ndata.ncmds = is_64 ? ((t_header64 *)ptr)->ncmds
						: ((t_header32 *)ptr)->ncmds;
	c = ndata.n_type & N_TYPE;
	if (c == N_UNDF)
		c = ndata.n_value != 0 ? 'c' : 'u';
	else if (c == N_ABS)
		c = 'a';
	else if (c == N_SECT)
		c = get_type_sect(&ndata, is_64);
	else if (c == N_INDR)
		c = 'i';
	else
		c = '?';
	if (ndata.n_type & N_EXT && c != '?')
		c = ft_toupper(c);
	return (c);
}

int		cmp_nlist_arg(void *array, char *string_table, uint32_t j, int is_64)
{
	if (is_64)
		return (ft_strcmp(string_table + NLIST64[j].n_un.n_strx,
				string_table + NLIST64[j - 1].n_un.n_strx));
	else
		return (ft_strcmp(string_table + NLIST32[j].n_un.n_strx,
				string_table + NLIST32[j - 1].n_un.n_strx));
}

void	swap_nlist_arg(void *array, uint32_t j, int is_64)
{
	t_nlist64	tmp64;
	t_nlist32	tmp32;

	if (is_64)
	{
		tmp64 = NLIST64[j];
		NLIST64[j] = NLIST64[j -1];
		NLIST64[j - 1] = tmp64;
	}
	else
	{
		tmp32 = NLIST32[j];
		NLIST32[j] = NLIST32[j -1];
		NLIST32[j - 1] = tmp32;
	}
}

void	sort_nlist(void *array, char *string_table, uint32_t nsyms, int is_64)
{
	uint32_t	i;
	uint32_t	j;

	i = 1;
	while (i <= nsyms - 1)
	{
		j = i;
		while (j > 0 && cmp_nlist_arg(array, string_table, j, is_64) < 0)
		{
			swap_nlist_arg(array, j, is_64);
			--j;
		}
		++i;
	}
}

void	handle_symtab(char *ptr, t_symtab *sym, int is_64)
{
	uint32_t	i;
	void		*string_table;
	void		*array;

	array = ptr + sym->symoff;
	string_table = ptr + sym->stroff;
	i = -1;
	sort_nlist(array, string_table, sym->nsyms, is_64);
	while (++i < sym->nsyms)
	{
		if (is_64)
		{
			ft_putchar(get_type(array, i, ptr, is_64));
			ft_putchar(' ');
			ft_putendl(string_table + NLIST64[i].n_un.n_strx);
		}
		else
		{
			ft_putendl(string_table + NLIST32[i].n_un.n_strx);
		}
	}
}

void	handle_32_64(char *ptr, int is_64)
{
	uint32_t	i;
	uint32_t	ncmds;
	t_lc		*lc;
	t_symtab	*sym;

	ncmds = is_64 ? ((t_header64 *)ptr)->ncmds : ((t_header32 *)ptr)->ncmds;
	lc = is_64 ? (void *)ptr + sizeof(t_header64)
				: (void *)ptr + sizeof(t_header32);
	i = -1;
	while (++i < ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (t_symtab *)lc;
			handle_symtab(ptr, sym, is_64);
			break ;
		}
		lc = (void *)lc + lc->cmdsize;
	}
}

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
