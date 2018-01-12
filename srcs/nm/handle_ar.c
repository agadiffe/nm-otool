#include "ft_nm.h"
#include "libft.h"

int		get_ar_size(char *name)
{
	int		x;
	char	*word;

	word = ft_strchr(name, '/') + 1;
	x = ft_atoi(word);
	return (x);
}

char	*get_ar_name(char *name)
{
	int		len;

	len = ft_strlen(ARFMAG);
	return (ft_strstr(name, ARFMAG) + len);
}

void	print_ar(char *ptr, uint32_t r_off)
{
	t_ar	*ar;
	char	*tmp;

	ar = (void *)ptr + r_off;
	ft_putstr(get_ar_name(ar->ar_name)); 
	ft_putendl(":");
	tmp = (void *)ar + sizeof(t_ar) + get_ar_size(ar->ar_name);
	handle_arch(tmp);
}

void	handle_ar(char *ptr)
{
	uint32_t	i;
	uint32_t	size;
	t_ar		*ar;
	t_ranlib	*ran;

	ar = (void *)ptr + SARMAG;
	size = *((int *)((void *)ar + sizeof(t_ar) + get_ar_size(ar->ar_name)));
	size /= sizeof(t_ranlib);
	ran = (void *)ar + sizeof(t_ar) + get_ar_size(ar->ar_name) + 4;
	i = -1;
	while (++i < size)
		print_ar(ptr, ran[i].ran_off);
}
