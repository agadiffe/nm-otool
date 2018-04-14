#include "ft_nm.h"
#include "libft.h"

static void		handle_symtab(t_data *d)
{
	uint32_t	i;
	uint32_t	strsize;
	uint32_t	nsyms;

	strsize = swap32(d->sym->strsize, d->swap);
	nsyms = swap32(d->sym->nsyms, d->swap);
	d->array = d->ptr + swap32(d->sym->symoff, d->swap);
	d->string_table = d->ptr + swap32(d->sym->stroff, d->swap);
	if (is_invalid_addr((void *)d->string_table + strsize))
		return ;
	i = -1;
	if (sort_nlist(d))
		return ;
	while (++i < nsyms)
	{
		if (print_32_64(d, i))
			return ;
	}
}

static int		fill_data(t_data *d, char *ptr, int is_64)
{
	uint32_t		header_size;
	unsigned int	magic;

	magic = *(unsigned int *)ptr;
	d->swap = magic == MH_CIGAM || magic == MH_CIGAM_64 ? 1 : 0;
	header_size = is_64 ? sizeof(t_header64) : sizeof(t_header32);
	if (is_invalid_addr((void *)ptr + header_size))
		return (1);
	d->ptr = ptr;
	d->is_64 = is_64;
	d->ncmds = is_64 ? ((t_header64 *)ptr)->ncmds : ((t_header32 *)ptr)->ncmds;
	d->ncmds = swap32(d->ncmds, d->swap);
	d->lc = is_64 ? (void *)ptr + sizeof(t_header64)
					: (void *)ptr + sizeof(t_header32);
	if (is_invalid_addr((void *)d->lc + sizeof(t_lc)))
		return (1);
	return (0);
}

void			handle_32_64(char *ptr, int is_64)
{
	uint32_t		i;
	t_data			d;
	t_lc			*tmp;

	if (is_invalid_addr((void *)ptr))
		return ;
	if (fill_data(&d, ptr, is_64))
		return ;
	tmp = d.lc;
	i = -1;
	while (++i < d.ncmds)
	{
		if (swap32(tmp->cmd, d.swap) == LC_SYMTAB)
		{
			d.sym = (t_symtab *)tmp;
			if (is_invalid_addr((void *)d.sym + sizeof(t_symtab)))
				return ;
			handle_symtab(&d);
			break ;
		}
		tmp = (void *)tmp + swap32(tmp->cmdsize, d.swap);
		if (is_invalid_addr((void *)tmp + sizeof(t_lc)))
			return ;
	}
}
