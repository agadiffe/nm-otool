#include "machine.h"
#include "ft_nm.h"
#include "libft.h"

int				*get_arch_tab_printed(int reset, int reset_ar, int is_ar)
{
	static int		tab[13];
	static int		tab_ar[13];
	int				i;

	if (reset)
	{
		i = -1;
		while (++i < 13)
			tab[i] = 0;
	}
	if (reset_ar)
	{
		i = -1;
		while (++i < 13)
			tab_ar[i] = 0;
	}
	return (is_ar ? tab_ar : tab);
}

static int		print_info(char *av, char *arch, int is_nm, int print)
{
	if (print != -2)
	{
		if (!is_ar(0, 0))
			ft_putstr(av);
		if (print)
		{
			if (is_nm)
				ft_putstr(" (for architecture ");
			else
				ft_putstr(" (architecture ");
			ft_putstr(arch);
			ft_putstr(")");
		}
		if (print != 3)
			ft_putendl(":");
		else
			ft_putendl("");
	}
	return (1);
}

static int		print_arch_bis(int cpu, char *av, int is_nm, int print)
{
	int		*tab;
	int		ar;

	ar = is_ar(0, 0);
	tab = ar ? get_arch_tab_printed(0, 0, 1)
						: get_arch_tab_printed(0, 0, 0);
	if ((ar || !tab[8]) && cpu == CPU_TYPE_HPPA)
		return ((tab[8] = print_info(av, "hppa", is_nm, print)));
	if ((ar || !tab[9]) && cpu == CPU_TYPE_ARM)
		return ((tab[9] = print_info(av, "arm", is_nm, print)));
	if ((ar || !tab[10]) && cpu == CPU_TYPE_ARM64)
		return ((tab[10] = print_info(av, "arm64", is_nm, print)));
	if ((ar || !tab[11]) && cpu == CPU_TYPE_SPARC)
		return ((tab[11] = print_info(av, "sparc", is_nm, print)));
	if ((ar || !tab[12]) && cpu == CPU_TYPE_I860)
		return ((tab[12] = print_info(av, "i860", is_nm, print)));
	return (0);
}

int				print_arch(int cpu, char *av, int is_nm, int print)
{
	int		*tab;
	int		ar;

	ar = is_ar(0, 0);
	tab = ar ? get_arch_tab_printed(0, 0, 1)
				: get_arch_tab_printed(0, 0, 0);
	if ((ar || !tab[0])
			&& (cpu == CPU_TYPE_X86 || cpu == CPU_TYPE_I386))
		return ((tab[0] = print_info(av, "i386", is_nm, print)));
	if ((ar || !tab[1]) && cpu == CPU_TYPE_X86_64)
		return ((tab[1] = print_info(av, "x86_64", is_nm, print)));
	if ((ar || !tab[2]) && cpu == CPU_TYPE_POWERPC)
		return ((tab[2] = print_info(av, "ppc", is_nm, print)));
	if ((ar || !tab[3]) && cpu == CPU_TYPE_POWERPC64)
		return ((tab[3] = print_info(av, "ppc64", is_nm, print)));
	if ((ar || !tab[4]) && cpu == CPU_TYPE_VAX)
		return ((tab[4] = print_info(av, "vax", is_nm, print)));
	if ((ar || !tab[5]) && cpu == CPU_TYPE_MC680x0)
		return ((tab[5] = print_info(av, "mc680", is_nm, print)));
	if ((ar || !tab[6]) && cpu == CPU_TYPE_MC98000)
		return ((tab[6] = print_info(av, "mc98000", is_nm, print)));
	if ((ar || !tab[7]) && cpu == CPU_TYPE_MC88000)
		return ((tab[7] = print_info(av, "mc88000", is_nm, print)));
	return (print_arch_bis(cpu, av, is_nm, print));
}
