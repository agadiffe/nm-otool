/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_nm.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadiffe <agadiffe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/23 00:52:53 by agadiffe          #+#    #+#             */
/*   Updated: 2018/04/23 00:52:56 by agadiffe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
#include "libft.h"

static void		print_addr(t_data *d, char type)
{
	size_t	len;

	if (!d->n_value && type != 't' && type != 'T' && type != 'A')
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
	name = d->is_64 ? d->string_table + swap32(NLIST64[i].n_un.n_strx, d->swap)
					: d->string_table + swap32(NLIST32[i].n_un.n_strx, d->swap);
	if (is_not_terminated_string(name, "print_nm()"))
		return (1);
	if (d->display && ft_strlen(name)
			&& type != '?' && type != 'u' && type != '-')
	{
		print_addr(d, type);
		ft_putchar(' ');
		ft_putchar(type);
		ft_putchar(' ');
		ft_putendl(name);
	}
	return (0);
}
