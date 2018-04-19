#include "ft_nm.h"
#include "libft.h"

static uint32_t	get_ar_name_offset(t_ar *ar)
{
	int		n;
	char	*tmp;

	n = 0;
	if ((tmp = ft_strchr(ar->ar_name, '/')))
		n = ft_atoi(tmp + 1);
	return (n);
}

static int		print_ar(t_ar *ar, char *av)
{
	void		*tmp;
	uint32_t	n;

	ft_putstr(av);
	ft_putchar('(');
	if (is_not_terminated_string(ar->ar_name, "archive name"))
		return (1);
	if ((n = get_ar_name_offset(ar)))
	{
		if (is_not_terminated_string((void *)ar + sizeof(t_ar), "archive name"))
			return (1);
		ft_putstr((void *)ar + sizeof(t_ar));
	}
	else
		ft_putstr(ar->ar_name);
	ft_putchar(')');
	ft_putendl(":");
	tmp = (void *)ar + sizeof(t_ar) + n;
	if (is_invalid_addr(tmp, "ptr archive member"))
		return (1);
	handle_arch(tmp, av, 1);
	return (0);
}

void			handle_ar(char *ptr, char *av)
{
	t_ar		*ar;
	int			no_first;

	ar = (void *)ptr + SARMAG;
	if (is_invalid_addr((void *)ar + sizeof(t_ar), "ptr archive"))
		return ;
	if (is_not_terminated_string(ar->ar_size, "archive size"))
		return ;
	ar = (void *)ar + sizeof(t_ar) + ft_atoi(ar->ar_size);
	no_first = 0;
	while ((void *)ar < g_max_addr)
	{
		if (no_first)
			ft_putendl("");
		no_first = 1;
		if (print_ar(ar, av))
			return ;
		if (is_not_terminated_string(ar->ar_size, "archive size"))
			return ;
		ar = (void *)ar + sizeof(t_ar) + ft_atoi(ar->ar_size);
	}
}
