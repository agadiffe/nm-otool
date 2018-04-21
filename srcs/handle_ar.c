#include "ft_nm.h"
#include "libft.h"

static void		print_ar_name(char *ptr, char *av, int is_nm)
{
	static int		no_first;

	if (!no_first && !is_nm)
	{
		no_first = 1;
		ft_putstr("Archive : ");
		ft_putstr(av);
		print_arch(get_cpu(ptr), av, 2, 3);
	}
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

static uint32_t	get_ar_name_offset(t_ar *ar)
{
	int		n;
	char	*tmp;

	n = 0;
	if ((tmp = ft_strchr(ar->ar_name, '/')))
		n = ft_atoi(tmp + 1);
	return (n);
}

static int		print_ar(t_ar *ar, char *av, int print, int is_nm)
{
	void		*tmp;
	uint32_t	n;

	n = get_ar_name_offset(ar);
	tmp = (void *)ar + sizeof(t_ar) + n;
	if (is_invalid_addr(tmp, "ptr archive member"))
		return (1);
	print_ar_name(tmp, av, is_nm);
	ft_putstr(av);
	ft_putchar('(');
	if (print_name_member(ar, n))
		return (1);
	ft_putchar(')');
	if (!print)
		ft_putendl(":");
	else
		print_arch(get_cpu(tmp), av, 2, print);
	handle_arch(tmp, av, -2, 2);
	return (0);
}

void			handle_ar(char *ptr, char *av, int print, int is_nm)
{
	t_ar	*ar;
	int		first;

	ar = (void *)ptr + SARMAG;
	if (is_invalid_addr((void *)ar + sizeof(t_ar), "ptr archive"))
		return ;
	if (is_not_terminated_string(ar->ar_size, "archive size"))
		return ;
	ar = (void *)ar + sizeof(t_ar) + ft_atoi(ar->ar_size);
	first = 1;
	while ((void *)ar < g_max_addr)
	{
		if (ft_strncmp(ar->ar_fmag, ARFMAG, 2))
			break ;
		if (!first && is_nm)
			ft_putendl("");
		first = 0;
		if (print_ar(ar, av, print, is_nm))
			return ;
		if (is_not_terminated_string(ar->ar_size, "archive size"))
			return ;
		ar = (void *)ar + sizeof(t_ar) + ft_atoi(ar->ar_size);
	}
}
