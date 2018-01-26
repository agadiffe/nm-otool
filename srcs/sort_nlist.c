#include "ft_nm.h"
#include "libft.h"

static int	cmp_nlist_arg(t_data *d, uint32_t j, int *error)
{
	char	*s1;
	char	*s2;

	if (d->is_64)
	{
		s1 = d->string_table + NLIST64[j].n_un.n_strx;
		s2 = d->string_table + NLIST64[j - 1].n_un.n_strx;
	}
	else
	{
		s1 = d->string_table + NLIST32[j].n_un.n_strx;
		s2 = d->string_table + NLIST32[j - 1].n_un.n_strx;
	}
	if (is_not_terminated_string(s1) || is_not_terminated_string(s2))
	{
		*error = 1;
		return (-1);
	}
	return (ft_strcmp(s1, s2));
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

int			sort_nlist(t_data *d)
{
	uint32_t	i;
	uint32_t	j;
	int			error;

	i = 1;
	error = 0;
	while (i <= d->sym->nsyms - 1)
	{
		j = i;
		while (j > 0 && cmp_nlist_arg(d, j, &error) < 0)
		{
			if (error)
				return (1);
			swap_nlist_arg(d, j);
			--j;
		}
		++i;
	}
	return (0);
}
