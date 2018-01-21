#include "ft_nm.h"
#include "libft.h"

uint32_t	get_ar_name_offset(t_ar *ar)
{
	int		n;
	char	*tmp;

	n = 0;
	if ((tmp = ft_strchr(ar->ar_name, '/')))
		n = ft_atoi(tmp + 1);
	return (n);
}

void		print_ar(char *ptr, uint32_t r_off)
{
	t_ar		*ar;
	char		*tmp;
	uint32_t	n;

	ar = (void *)ptr + r_off;
	n = get_ar_name_offset(ar);
	ft_putstr((void *)ar + sizeof(t_ar));
	ft_putendl(":");
	tmp = (void *)ar + sizeof(t_ar) + n;
	handle_arch(tmp);
}

void		handle_ar(char *ptr)
{
	t_ar		*ar;
	t_ranlib	*ran;
	uint32_t	i;
	uint64_t	size;
	uint32_t	n;

	ar = (void *)ptr + SARMAG;
	n = get_ar_name_offset(ar);
	size = *(uint64_t *)((void *)ar + sizeof(t_ar) + n);
	size = size / sizeof(t_ranlib);
	ran = (void *)ar + sizeof(t_ar) + n + sizeof(uint32_t);
	i = -1;
	while (++i < size)
	{
		print_ar(ptr, ran[i].ran_off);
		if (i + 1 != size)
			ft_putendl("");
	}
}
