#include "ft_nm.h"
#include "libft.h"

static void		get_nbr_arch_to_print(int sp[4], int host_cpu)
{
	int			*tab;
	uint32_t	i;

	if (host_cpu)
		sp[1] = 1;
	else
	{
		tab = get_arch_tab_printed(0, 0, 1);
		i = -1;
		while (++i < 13)
		{
			if (tab[i] == 1)
				sp[1] += 1;
		}
	}
}

static int		handle_host_cpu(void *arch, int *cpu, uint32_t nsi[3])
{
	int		current_cpu;

	is_ar(1, 1);
	current_cpu = nsi[2] ? swap32(((t_arch64 *)arch)->cputype, nsi[1])
						: swap32(((t_arch32 *)arch)->cputype, nsi[1]);
	if (current_cpu == HOST_CPU)
		*cpu = 1;
	print_arch(current_cpu, "", 1, -2);
	is_ar(1, 0);
	return (0);
}

int				check_fat_host_arch(char *ptr, uint32_t nsi[3], int sp[4])
{
	void		*arch;
	uint32_t	header_size;
	uint32_t	i;
	uint32_t	offset;
	int			host_cpu;

	host_cpu = 0;
	arch = (void *)ptr + sizeof(t_headerfat);
	header_size = nsi[2] ? sizeof(t_arch64) : sizeof(t_arch32);
	i = -1;
	while (++i != nsi[0])
	{
		if (is_invalid_addr((void *)arch + header_size, "fat ptr +header"))
			return (-42);
		offset = nsi[2] ? swap64(((t_arch64 *)arch)->offset, nsi[1])
						: swap32(((t_arch32 *)arch)->offset, nsi[1]);
		if (is_invalid_addr((void *)ptr + offset, "fat member ptr"))
			return (-42);
		handle_host_cpu(arch, &host_cpu, nsi);
		arch = (void *)arch + header_size;
	}
	get_nbr_arch_to_print(sp, host_cpu);
	return (host_cpu);
}
