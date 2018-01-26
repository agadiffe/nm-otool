#include "ft_nm.h"
#include "libft.h"

int			is64bit(void)
{
	if (sizeof(void *) == 4)
		return (0);
	return (1);
}

uint32_t	endianness(uint32_t value)
{
	uint32_t tmp;

	tmp = 0;
	tmp |= (value & 0x000000FF) << 24;
	tmp |= (value & 0x0000FF00) << 8;
	tmp |= (value & 0x00FF0000) >> 8;
	tmp |= (value & 0xFF000000) >> 24;
	return (tmp);
}

static void	fat_arch(void *ptr, uint32_t n_fatarch, int is_big_endian)
{
	t_arch		*arch;
	uint32_t	i;
	uint32_t	offset;

	arch = (void *)ptr + sizeof(t_headerfat);
	if (is_invalid_addr((void *)arch + sizeof(t_arch)))
		return ;
	i = -1;
	while (++i != n_fatarch)
	{
		offset = is_big_endian ? endianness(arch->offset) : arch->offset;
		if (is64bit() && (endianness(arch->cputype) == CPU_TYPE_X86_64
						|| arch->cputype == CPU_TYPE_X86_64))
			handle_32_64((void *)ptr + offset, X64);
		else if (!is64bit() && (endianness(arch->cputype) == CPU_TYPE_I386
								|| arch->cputype == CPU_TYPE_I386))
			handle_32_64((void *)ptr + offset, X86);
		arch = (void *)arch + sizeof(*arch);
		if (is_invalid_addr((void *)arch + sizeof(t_arch)))
			return ;
	}
}

void	handle_fat(char *ptr, int is_big_endian)
{
	uint32_t	n_fatarch;

	if (is_invalid_addr((void *)ptr + sizeof(t_headerfat)))
		return ;
	if (is_big_endian)
		n_fatarch = endianness(((t_headerfat *)ptr)->nfat_arch);
	else
		n_fatarch = ((t_headerfat *)ptr)->nfat_arch;
	fat_arch(ptr, n_fatarch, is_big_endian);
}
