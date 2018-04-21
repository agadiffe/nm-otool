#include "ft_nm.h"
#include "libft.h"

static int	cmp_nlist_arg(t_data *d, uint32_t j, int *error)
{
	char	*s1;
	char	*s2;

	if (d->is_64)
	{
		s1 = d->string_table + swap32(NLIST64[j].n_un.n_strx, d->swap);
		s2 = d->string_table + swap32(NLIST64[j - 1].n_un.n_strx, d->swap);
	}
	else
	{
		s1 = d->string_table + swap32(NLIST32[j].n_un.n_strx, d->swap);
		s2 = d->string_table + swap32(NLIST32[j - 1].n_un.n_strx, d->swap);
	}
	if (is_not_terminated_string(s1, "sort nlist strintable name")
			|| is_not_terminated_string(s2, "sort nlist strintable name"))
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

int			sort_nlist_same_name(t_data *d)
{
	uint32_t	i;
	uint32_t	j;
	uint32_t	nsyms;
	int			error;

	i = 1;
	error = 0;
	nsyms = swap32(d->sym->nsyms, d->swap);
	while (i <= nsyms - 1)
	{
		j = i;
		while (j > 0 && cmp_nlist_arg(d, j, &error) == 0)
		{
			d->n_value = d->is_64 ? swap64(NLIST64[j].n_value, d->swap)
									: swap32(NLIST32[j].n_value, d->swap);
			if (error)
				return (1);
			if (!d->n_value)
				swap_nlist_arg(d, j);
			--j;
		}
		++i;
	}
	return (0);
}

int			sort_nlist(t_data *d)
{
	uint32_t	i;
	uint32_t	j;
	int			error;
	uint32_t	nsyms;

	i = 1;
	error = 0;
	nsyms = swap32(d->sym->nsyms, d->swap);
	while (i <= nsyms - 1)
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
