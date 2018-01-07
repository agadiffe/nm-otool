#include "ft_nm.h"
#include "libft.h"

static void		print_addr(t_data *d, char type)
{
	size_t	len;

	if (d->is_64 && !d->n_value64 && type != 't')
		ft_putnchar(' ', d->is_64);
	else if (!d->is_64 && !d->n_value32 && type != 't')
		ft_putnchar(' ', d->is_64);
	else
	{
		len = d->is_64 ? 16 - ft_nlen(d->n_value64, 16)
						: 8 - ft_nlen(d->n_value32, 8);
		ft_putnchar('0', len);
		if (d->is_64)
			ft_putnbr_base(d->n_value64, BASE16);
		else
			ft_putnbr_base(d->n_value32, BASE16);
	}
}

void			print_32_64(t_data *d, uint32_t i)
{
	char		type;

	if ((type = (get_type(d, i))) != '?')
	{
		print_addr(d, type);
		ft_putchar(' ');
		ft_putchar(type);
		ft_putchar(' ');
		if (d->is_64)
			ft_putendl(d->string_table + NLIST64[i].n_un.n_strx);
		else
			ft_putendl(d->string_table + NLIST32[i].n_un.n_strx);
	}
}
