#include "ft_nm.h"
#include "libft.h"

int				is_32_or_64(char *ptr)
{
	unsigned int	magic_nbr;

	magic_nbr = *(unsigned int *)ptr;
	return (magic_nbr == MH_CIGAM || magic_nbr == MH_MAGIC
			|| magic_nbr == MH_CIGAM_64 || magic_nbr == MH_MAGIC_64);
}

int				get_cpu(char *ptr)
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
	print_arch(cpu, "", 2, -2);
	return (cpu);
}

static void		get_nbr_arch_to_print(int *print)
{
	int		*tab;
	uint32_t	i;

	tab = get_arch_tab_printed(0, 0, 1);
	i = -1;
	while (++i < 13)
	{
		if (tab[i] == 1)
			*print += 1;
	}
}

static int		handle_host_cpu(char *ptr, int *print)
{
	int		cpu;

	if (is_32_or_64(ptr))
	{
		if ((cpu = get_cpu(ptr)) < 0)
			return (-42);
		if (cpu == HOST_CPU)
			return (1);
	}
	else if (!ft_strncmp(ptr, ARMAG, SARMAG))
		*print += 1;
	return (0);
}

int				check_fat_host_arch(char *ptr, uint32_t n_fatarch,
									int swap, int *print)
{
	t_arch		*arch;
	uint32_t	i;
	uint32_t	offset;
	int			cpu_host;
	int			ret;

	cpu_host = 0;
	arch = (void *)ptr + sizeof(t_headerfat);
	i = -1;
	while (++i != n_fatarch)
	{
		if (is_invalid_addr((void *)arch + sizeof(t_arch), "fat ptr + header"))
			return (-42);
		offset = swap32(arch->offset, swap);
		if (is_invalid_addr((void *)ptr + offset, "fat member ptr"))
			return (-42);
		if ((ret = handle_host_cpu((void *)ptr + offset, print)) == -42)
			return (-42);
		if (ret)
			cpu_host = ret;
		arch = (void *)arch + sizeof(t_arch);
	}
	get_nbr_arch_to_print(print);
	return (cpu_host);
}
