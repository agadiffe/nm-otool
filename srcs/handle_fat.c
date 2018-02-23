#include "ft_nm.h"
#include "libft.h"

static void	fat_arch(void *ptr, uint32_t n_fatarch, int swap)
{
	t_arch		*arch;
	uint32_t	i;
	uint32_t	offset;
	uint32_t	cpu;

	arch = (void *)ptr + sizeof(t_headerfat);
	if (is_invalid_addr((void *)arch + sizeof(t_arch)))
		return ;
	i = -1;
	while (++i != n_fatarch)
	{
		offset = swap32(arch->offset, swap);
		cpu = swap32(arch->cputype, swap);
		if (cpu == CPU_TYPE_X86_64)
			handle_32_64((void *)ptr + offset, X64);
		else if (cpu == CPU_TYPE_I386)
			handle_32_64((void *)ptr + offset, X86);
		else
			ft_putendl("Unknow cpu type");
		arch = (void *)arch + sizeof(t_arch);
		if (is_invalid_addr((void *)arch + sizeof(t_arch)))
			return ;
		if (i + 1 != n_fatarch)
			ft_putendl("");
	}
}

void	handle_fat(char *ptr)
{
	uint32_t		n_fatarch;
	unsigned int	magic_nbr;
	int				swap;

	magic_nbr = *(unsigned int *)ptr;
	if (is_invalid_addr((void *)ptr + sizeof(t_headerfat)))
		return ;
	swap = magic_nbr == FAT_CIGAM ? 1 : 0;
	n_fatarch = swap32(((t_headerfat *)ptr)->nfat_arch, swap);
	fat_arch(ptr, n_fatarch, swap);
}
