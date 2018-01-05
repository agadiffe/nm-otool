#include "ft_nm.h"
#include "libft.h"

static int	cmp_nlist_arg(void *array, char *string_table,
							uint32_t j, int is_64)
{
	if (is_64)
		return (ft_strcmp(string_table + NLIST64[j].n_un.n_strx,
				string_table + NLIST64[j - 1].n_un.n_strx));
	else
		return (ft_strcmp(string_table + NLIST32[j].n_un.n_strx,
				string_table + NLIST32[j - 1].n_un.n_strx));
}

static void	swap_nlist_arg(void *array, uint32_t j, int is_64)
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

void		sort_nlist(void *array, char *string_table,
						uint32_t nsyms, int is_64)
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
