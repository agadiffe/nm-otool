/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_ar.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadiffe <agadiffe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/23 00:51:34 by agadiffe          #+#    #+#             */
/*   Updated: 2018/04/23 19:40:41 by agadiffe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
#include "libft.h"

static int		get_cpu(char *ptr)
{
	int				cpu;
	int				swap;
	int				is_64;
	unsigned int	magic_nbr;
	uint32_t		header_size;

	magic_nbr = *(unsigned int *)ptr;
	if (!(magic_nbr == MH_CIGAM || magic_nbr == MH_MAGIC
			|| magic_nbr == MH_CIGAM_64 || magic_nbr == MH_MAGIC_64))
		return (0);
	swap = magic_nbr == MH_CIGAM || magic_nbr == MH_CIGAM_64 ? 1 : 0;
	is_64 = magic_nbr == MH_MAGIC_64 || magic_nbr == MH_CIGAM_64 ? 1 : 0;
	header_size = is_64 ? sizeof(t_header64) : sizeof(t_header32);
	if (is_invalid_addr((void *)ptr + header_size, "fat member ptr + header"))
		return (-42);
	cpu = is_64 ? swap32(((t_header64 *)ptr)->cputype, swap)
				: swap32(((t_header32 *)ptr)->cputype, swap);
	return (cpu);
}

static int		print_ar_name(char *ptr, char *av, int is_nm, int print)
{
	static int		no_first;
	int				cpu;

	if ((cpu = get_cpu(ptr)) == -42)
		return (1);
	is_ar(1, 0);
	print_arch(cpu, av, is_nm, -2);
	is_ar(1, 1);
	if (!no_first && !is_nm)
	{
		no_first = 1;
		ft_putstr("Archive : ");
		ft_putstr(av);
		if (print < 1 || !cpu)
			ft_putendl("");
		else
			print_arch(cpu, av, is_nm, 3);
	}
	return (0);
}

static int		print_name_member(t_ar *ar, uint32_t offset)
{
	if (is_not_terminated_string(ar->ar_name, "archive name"))
		return (1);
	if (offset)
	{
		if (is_not_terminated_string((void *)ar + sizeof(t_ar), "archive name"))
			return (1);
		ft_putstr((void *)ar + sizeof(t_ar));
	}
	else
		ft_putstr(ar->ar_name);
	return (0);
}

static int		print_ar(t_ar *ar, char *av, int print, int is_nm)
{
	void		*tmp;
	uint32_t	n;

	n = 0;
	if ((tmp = ft_strchr(ar->ar_name, '/')))
		n = ft_atoi(tmp + 1);
	tmp = (void *)ar + sizeof(t_ar) + n;
	if (is_invalid_addr(tmp, "ptr archive member"))
		return (1);
	if (print_ar_name(tmp, av, is_nm, print))
		return (1);
	ft_putstr(av);
	ft_putchar('(');
	if (print_name_member(ar, n))
		return (1);
	ft_putchar(')');
	if (print < 1)
		ft_putendl(":");
	else
		print_arch(get_cpu(tmp), av, is_nm, print);
	handle_arch(tmp, av, -2, is_nm);
	is_ar(1, 1);
	return (0);
}

int				handle_ar(char *ptr, char *av, int print, int is_nm)
{
	t_ar	*ar;
	int		first;

	is_ar(1, 1);
	ar = (void *)ptr + SARMAG;
	if (is_invalid_addr((void *)ar + sizeof(t_ar), "ptr archive"))
		return (1);
	if (is_not_terminated_string(ar->ar_size, "archive size"))
		return (1);
	ar = (void *)ar + sizeof(t_ar) + ft_atoi(ar->ar_size);
	first = 1;
	while ((void *)ar < *get_max_addr())
	{
		if (ft_strncmp(ar->ar_fmag, ARFMAG, 2))
			break ;
		if (!first && is_nm)
			ft_putendl("");
		first = 0;
		if (print_ar(ar, av, print, is_nm))
			return (1);
		if (is_not_terminated_string(ar->ar_size, "archive size"))
			return (1);
		ar = (void *)ar + sizeof(t_ar) + ft_atoi(ar->ar_size);
	}
	return (is_ar(1, 0));
}
