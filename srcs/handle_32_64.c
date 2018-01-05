#include "ft_nm.h"
#include "libft.h"

static void		handle_symtab(char *ptr, t_symtab *sym, int is_64)
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

void			handle_32_64(char *ptr, int is_64)
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

