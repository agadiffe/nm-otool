#include "ft_nm.h"
#include "libft.h"

static void		handle_fat_64(void *ptr, uint32_t cpu, uint32_t offset)
{
	static uint32_t		tab[2];

	if (!tab[0] && cpu == CPU_TYPE_X86_64)
	{
		tab[0] = 1;
		ft_putendl("arch:X86_X64");
		handle_32_64((void *)ptr + offset, X64);
	}
	if (!tab[1] && cpu == CPU_TYPE_POWERPC64)
	{
		tab[1] = 1;
		ft_putendl("arch:POWERPC64");
		handle_32_64((void *)ptr + offset, X64);
	}
}

static void		handle_fat_32(void *ptr, uint32_t cpu, uint32_t offset)
{
	static uint32_t		tab[2];

	if (!tab[0] && cpu == CPU_TYPE_X86)
	{
		tab[0] = 1;
		ft_putendl("arch:X86");
		handle_32_64((void *)ptr + offset, X86);
	}
	if (!tab[1] && cpu == CPU_TYPE_POWERPC)
	{
		tab[1] = 1;
		ft_putendl("arch:POWERPC");
		handle_32_64((void *)ptr + offset, X86);
	}
}

static void		handle_fat_arch(void *ptr, uint32_t cpu, uint32_t offset)
{
	unsigned int	magic_nbr;

	if (is_invalid_addr((void *)ptr + offset))
		return ;
	magic_nbr = *(unsigned int *)(ptr + offset);
	if (magic_nbr == MH_MAGIC_64 || magic_nbr == MH_CIGAM_64)
		handle_fat_64(ptr, cpu, offset);
	else if (magic_nbr == MH_MAGIC || magic_nbr == MH_CIGAM)
		handle_fat_32(ptr, cpu, offset);
	else
		ft_putendl("Unknow architecture");
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
