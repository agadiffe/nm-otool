#include "ft_nm.h"
#include "libft.h"

static int		check_host(void)
{
	int		*tab;

	tab = get_arch_tab_printed(0, 0, 0);
	return (tab[1]);
}

static void		fat_arch_32(char *ptr, uint32_t nsi[3], char *av, int is_nm)
{
	void		*arch;
	uint32_t	i;
	uint32_t	offset;
	int			is_host_cpu;
	int			sp[4];

	sp[1] = 0;
	sp[0] = nsi[1];
	if ((is_host_cpu = check_fat_host_arch(ptr, nsi, sp)) == -42)
		return ;
	sp[2] = sp[1];
	arch = (void *)ptr + sizeof(t_headerfat);
	i = -1;
	while (++i != nsi[0])
	{
		offset = nsi[2] ? swap64(((t_arch64 *)arch)->offset, nsi[1])
						: swap32(((t_arch32 *)arch)->offset, nsi[1]);
		sp[3] = nsi[2] ? swap32(((t_arch64 *)arch)->cputype, nsi[1])
						: swap32(((t_arch32 *)arch)->cputype, nsi[1]);
		if (!is_host_cpu || (sp[3] == HOST_CPU && !check_host()))
			handle_arch((void *)ptr + offset, av,
						sp[1] > 1 || !is_nm ? sp[1] > 1 : -2 , is_nm);
		arch = (void *)arch + (nsi[2] ? sizeof(t_arch64) : sizeof(t_arch32));
		is_nm && --sp[2] > 0 ? ft_putendl("") : 0;
	}
}

void			handle_fat(char *ptr, char *av, int is_nm)
{
	unsigned int	magic_nbr;
	uint32_t		nsi[3];

	is_ar(1, 0);
	magic_nbr = *(unsigned int *)ptr;
	if (is_invalid_addr((void *)ptr + sizeof(t_headerfat), "fat ptr"))
		return ;
	nsi[1] = magic_nbr == FAT_CIGAM || magic_nbr == FAT_CIGAM_64 ? 1 : 0;
	nsi[2] = magic_nbr == FAT_MAGIC_64 || magic_nbr == FAT_CIGAM_64 ? 1 : 0;
	nsi[0] = swap32(((t_headerfat *)ptr)->nfat_arch, nsi[1]);
	fat_arch_32(ptr, nsi, av, is_nm);
}
