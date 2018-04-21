#include "ft_nm.h"
#include "libft.h"

int			is_32_or_64(char *ptr)
{
	unsigned int	magic_nbr;

	magic_nbr = *(unsigned int *)ptr;
	return (magic_nbr == MH_CIGAM || magic_nbr == MH_MAGIC
			|| magic_nbr == MH_CIGAM_64 || magic_nbr == MH_MAGIC_64);
}

int			get_cpu(char *ptr)
{
	int				cpu;
	int				swap;
	int				is_64;
	unsigned int	magic_nbr;
	uint32_t		header_size;

	magic_nbr = *(unsigned int *)ptr;
	swap = magic_nbr == MH_CIGAM || magic_nbr == MH_CIGAM_64 ? 1 : 0;
	is_64 = magic_nbr == MH_MAGIC_64 || magic_nbr == MH_CIGAM_64 ? 1 : 0;
	header_size = is_64 ? sizeof(t_header64) : sizeof(t_header32);
	if (is_invalid_addr((void *)ptr + header_size, "fat member ptr + header"))
		return (-42);
	cpu = is_64 ? swap32(((t_header64 *)ptr)->cputype, swap)
				: swap32(((t_header32 *)ptr)->cputype, swap);
	return (cpu);
}

int			check_fat_host_arch(char *ptr, uint32_t n_fatarch, int swap)
{
	t_arch		*arch;
	uint32_t	i;
	uint32_t	offset;
	int			cpu[2];

	cpu[0] = 0;
	arch = (void *)ptr + sizeof(t_headerfat);
	i = -1;
	while (++i != n_fatarch)
	{
		if (is_invalid_addr((void *)arch + sizeof(t_arch), "fat ptr + header"))
			return (1);
		offset = swap32(arch->offset, swap);
		if (is_invalid_addr((void *)ptr + offset, "fat member ptr"))
			return (1);
		if (is_32_or_64((void *)ptr + offset))
		{
			if ((cpu[1] = get_cpu((void *)ptr + offset)) < 0)
				return (-42);
			if (cpu[1] == HOST_CPU)
				cpu[0] = 1;
		}
		arch = (void *)arch + sizeof(t_arch);
	}
	return (cpu[0]);
}
