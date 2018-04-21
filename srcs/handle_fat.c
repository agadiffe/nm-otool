#include "ft_nm.h"
#include "libft.h"

static void		fat_arch(char *ptr, uint32_t n_fatarch, char *av, int is_nm)
{
	t_arch		*arch;
	uint32_t	i;
	uint32_t	offset;
	int			is_host_cpu;
	int			swap;

	swap = *(unsigned int *)ptr == FAT_CIGAM ? 1 : 0;
	is_host_cpu = check_fat_host_arch(ptr, n_fatarch, swap);
	if (is_host_cpu < 0)
		return ;
	arch = (void *)ptr + sizeof(t_headerfat);
	i = -1;
	while (++i != n_fatarch)
	{
		offset = swap32(arch->offset, swap);
		if (!is_host_cpu || !is_32_or_64((void *)ptr + offset)
				|| get_cpu((void *)ptr + offset) == HOST_CPU)
			handle_arch((void *)ptr + offset, av, 1, is_nm);
		arch = (void *)arch + sizeof(t_arch);
		if (is_nm && i + 1 != n_fatarch)
			ft_putendl("");
	}
}

void			handle_fat(char *ptr, char *av, int is_nm)
{
	uint32_t		n_fatarch;
	unsigned int	magic_nbr;
	int				swap;

	magic_nbr = *(unsigned int *)ptr;
	if (is_invalid_addr((void *)ptr + sizeof(t_headerfat), "fat ptr"))
		return ;
	swap = magic_nbr == FAT_CIGAM ? 1 : 0;
	n_fatarch = swap32(((t_headerfat *)ptr)->nfat_arch, swap);
	fat_arch(ptr, n_fatarch, av, is_nm);
}
