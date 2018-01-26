#include "ft_nm.h"
#include "libft.h"

static void		handle_symtab(t_data *d)
{
	uint32_t	i;

	d->array = d->ptr + d->sym->symoff;
	d->string_table = d->ptr + d->sym->stroff;
	if (is_invalid_addr((void *)d->string_table + d->sym->strsize))
		return ;
	i = -1;
	if (sort_nlist(d))
		return ;
	while (++i < d->sym->nsyms)
	{
		if (print_32_64(d, i))
			return ;
	}
}

static int		fill_data(t_data *d, char *ptr, int is_64)
{
	uint32_t	header_size;

	header_size = is_64 ? sizeof(t_header64) : sizeof(t_header32);
	if (is_invalid_addr((void *)ptr + header_size))
		return (1);
	d->ptr = ptr;
	d->is_64 = is_64;
	d->ncmds = is_64 ? ((t_header64 *)ptr)->ncmds : ((t_header32 *)ptr)->ncmds;
	d->lc = is_64 ? (void *)ptr + sizeof(t_header64)
					: (void *)ptr + sizeof(t_header32);
	if (is_invalid_addr((void *)d->lc + sizeof(t_lc)))
		return (1);
	return (0);
}

void			handle_32_64(char *ptr, int is_64)
{
	uint32_t	i;
	t_data		d;
	t_lc		*tmp;

	if (fill_data(&d, ptr, is_64))
		return ;
	tmp = d.lc;
	i = -1;
	while (++i < d.ncmds)
	{
		if (tmp->cmd == LC_SYMTAB)
		{
			d.sym = (t_symtab *)tmp;
			if (is_invalid_addr((void *)d.sym + sizeof(t_symtab)))
				return ;
			handle_symtab(&d);
			break ;
		}
		tmp = (void *)tmp + tmp->cmdsize;
		if (is_invalid_addr((void *)tmp + sizeof(t_lc)))
			return ;
	}
}
