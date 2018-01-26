#include "ft_nm.h"
#include "libft.h"

static void		print_addr(t_data *d, char type)
{
	size_t	len;

	if (!d->n_value && type != 't' && type != 'T')
		d->is_64 ? ft_putnchar(' ', 16) : ft_putnchar(' ', 8);
	else
	{
		len = d->is_64 ? 16 - ft_nlen(d->n_value, 16)
						: 8 - ft_nlen(d->n_value, 16);
		ft_putnchar('0', len);
		ft_putnbr_base(d->n_value, BASE16);
	}
}

int				print_32_64(t_data *d, uint32_t i)
{
	char	type;
	char	*name;

	type = get_type(d, i);
	if (type == '!')
		return (1);
	name = d->is_64 ? d->string_table + NLIST64[i].n_un.n_strx
					: d->string_table + NLIST32[i].n_un.n_strx;
	if (is_not_terminated_string(name))
		return (1);
	if (ft_strlen(name) && type != '?' && type != 'u')
	{
		print_addr(d, type);
		ft_putchar(' ');
		ft_putchar(type);
		ft_putchar(' ');
		ft_putendl(name);
	}
	return (0);
}
