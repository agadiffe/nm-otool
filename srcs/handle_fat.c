#include "ft_nm.h"
#include "libft.h"

static void		handle_fat_arch(void *ptr, uint32_t cpu, uint32_t offset)
{
	static uint32_t		tab[2];

	if (cpu == CPU_TYPE_X86_64)
	{
		if (!tab[0])
		{
			handle_32_64((void *)ptr + offset, X64);
			tab[0] = 1;
		}
	}
	else if (cpu == CPU_TYPE_X86)
	{
		if (!tab[1])
		{
			handle_32_64((void *)ptr + offset, X86);
			tab[1] = 1;
		}
	}
	else
		ft_putendl("Unknow cpu type");
}

static void		fat_arch(void *ptr, uint32_t n_fatarch, int swap)
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
		handle_fat_arch(ptr, cpu, offset);
		arch = (void *)arch + sizeof(t_arch);
		if (is_invalid_addr((void *)arch + sizeof(t_arch)))
			return ;
		if (i + 1 != n_fatarch)
			ft_putendl("");
	}
}

void			handle_fat(char *ptr)
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
