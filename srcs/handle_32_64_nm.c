#include "ft_nm.h"
#include "libft.h"

static void		handle_symtab(t_data *d)
{
	uint32_t	i;

	d->array = d->ptr + d->sym->symoff;
	d->string_table = d->ptr + d->sym->stroff;
	i = -1;
	sort_nlist(d);
	while (++i < d->sym->nsyms)
		print_32_64(d, i);
}

void			handle_32_64(char *ptr, int is_64)
{
	uint32_t	i;
	t_data		d;
	t_lc		*tmp;

	d.ptr = ptr;
	d.is_64 = is_64;
	d.ncmds = is_64 ? ((t_header64 *)ptr)->ncmds : ((t_header32 *)ptr)->ncmds;
	d.lc = is_64 ? (void *)ptr + sizeof(t_header64)
					: (void *)ptr + sizeof(t_header32);
	tmp = d.lc;
	i = -1;
	while (++i < d.ncmds)
	{
		if (tmp->cmd == LC_SYMTAB)
		{
			d.sym = (t_symtab *)tmp;
			handle_symtab(&d);
			break ;
		}
		tmp = (void *)tmp + tmp->cmdsize;
	}
}
