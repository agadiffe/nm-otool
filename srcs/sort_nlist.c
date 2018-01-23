#include "ft_nm.h"
#include "libft.h"

static int	cmp_nlist_arg(t_data *d, uint32_t j)
{
	if (d->is_64)
		return (ft_strcmp(d->string_table + NLIST64[j].n_un.n_strx,
				d->string_table + NLIST64[j - 1].n_un.n_strx));
	else
		return (ft_strcmp(d->string_table + NLIST32[j].n_un.n_strx,
				d->string_table + NLIST32[j - 1].n_un.n_strx));
}

static void	swap_nlist_arg(t_data *d, uint32_t j)
{
	t_nlist64	tmp64;
	t_nlist32	tmp32;

	if (d->is_64)
	{
		tmp64 = NLIST64[j];
		NLIST64[j] = NLIST64[j - 1];
		NLIST64[j - 1] = tmp64;
	}
	else
	{
		tmp32 = NLIST32[j];
		NLIST32[j] = NLIST32[j - 1];
		NLIST32[j - 1] = tmp32;
	}
}

void		sort_nlist(t_data *d)
{
	uint32_t	i;
	uint32_t	j;

	i = 1;
	while (i <= d->sym->nsyms - 1)
	{
		j = i;
		while (j > 0 && cmp_nlist_arg(d, j) < 0)
		{
			swap_nlist_arg(d, j);
			--j;
		}
		++i;
	}
}
