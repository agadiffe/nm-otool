#include "ft_nm.h"
#include "libft.h"

static void		fat_arch(char *ptr, uint32_t n_fatarch, int swap, char *av)
{
	t_arch		*arch;
	uint32_t	i;
	uint32_t	offset;

	arch = (void *)ptr + sizeof(t_headerfat);
	i = -1;
	while (++i != n_fatarch)
	{
		if (is_invalid_addr((void *)arch + sizeof(t_arch), "fat ptr + header"))
			return ;
		offset = swap32(arch->offset, swap);
		if (is_invalid_addr((void *)ptr + offset, "fat member ptr"))
			return ;
		handle_arch((void *)ptr + offset, av, 0);
		arch = (void *)arch + sizeof(t_arch);
		if (i + 1 != n_fatarch)
			ft_putendl("");
	}
}

void			handle_fat(char *ptr, char *av)
{
	uint32_t		n_fatarch;
	unsigned int	magic_nbr;
	int				swap;

	magic_nbr = *(unsigned int *)ptr;
	if (is_invalid_addr((void *)ptr + sizeof(t_headerfat), "fat ptr"))
		return ;
	swap = magic_nbr == FAT_CIGAM ? 1 : 0;
	n_fatarch = swap32(((t_headerfat *)ptr)->nfat_arch, swap);
	fat_arch(ptr, n_fatarch, swap, av);
}
