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

static void		print_ar(t_ar *ar)
{
	char		*tmp;
	uint32_t	n;

	if ((n = get_ar_name_offset(ar)))
		ft_putstr((void *)ar + sizeof(t_ar));
	else
		ft_putstr(ar->ar_name);
	ft_putendl(":");
	tmp = (void *)ar + sizeof(t_ar) + n;
	handle_arch(tmp);
}

void			handle_ar(char *ptr)
{
	t_ar		*ar;
	uint64_t	size;

	ar = (void *)ptr + SARMAG;
	size = sizeof(t_ar) + ft_atoi(ar->ar_size) + SARMAG;
	ar = (void *)ptr + size;
	print_ar(ar);
	while ((void *)ar + size < g_max_addr)
	{
		ft_putendl("");
		size = sizeof(t_ar) + ft_atoi(ar->ar_size);
		ar = (void *)ar + size;
		print_ar(ar);
	}
}
