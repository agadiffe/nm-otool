#include "machine.h"
#include "libft.h"

static int		print_info(char *av, char *arch, int is_nm, int print)
{
	if (print != -2)
	{
		if (is_nm != 2)
			ft_putstr(av);
		if (print)
		{
			if (is_nm == 1)
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
	static int	tab[5];

	if (is_nm == 2 || (!tab[0] && cpu == CPU_TYPE_HPPA))
		return ((tab[0] = print_info(av, "hppa", is_nm, print)));
	if (is_nm == 2 || (!tab[1] && cpu == CPU_TYPE_ARM))
		return ((tab[1] = print_info(av, "arm", is_nm, print)));
	if (is_nm == 2 || (!tab[2] && cpu == CPU_TYPE_ARM64))
		return ((tab[2] = print_info(av, "arm64", is_nm, print)));
	if (is_nm == 2 || (!tab[3] && cpu == CPU_TYPE_SPARC))
		return ((tab[3] = print_info(av, "sparc", is_nm, print)));
	if (is_nm == 2 || (!tab[4] && cpu == CPU_TYPE_I860))
		return ((tab[4] = print_info(av, "i860", is_nm, print)));
	return (0);
}

int				print_arch(int cpu, char *av, int is_nm, int print)
{
	static int	tab[8];

	if (is_nm == 2
			|| (!tab[0] && (cpu == CPU_TYPE_X86 || cpu == CPU_TYPE_I386)))
		return ((tab[0] = print_info(av, "i386", is_nm, print)));
	if (is_nm == 2 || (!tab[1] && cpu == CPU_TYPE_X86_64))
		return ((tab[1] = print_info(av, "x86_64", is_nm, print)));
	if (is_nm == 2 || (!tab[2] && cpu == CPU_TYPE_POWERPC))
		return ((tab[2] = print_info(av, "ppc", is_nm, print)));
	if (is_nm == 2 || (!tab[3] && cpu == CPU_TYPE_POWERPC64))
		return ((tab[3] = print_info(av, "ppc64", is_nm, print)));
	if (is_nm == 2 || (!tab[4] && cpu == CPU_TYPE_VAX))
		return ((tab[4] = print_info(av, "vax", is_nm, print)));
	if (is_nm == 2 || (!tab[5] && cpu == CPU_TYPE_MC680x0))
		return ((tab[5] = print_info(av, "mc680", is_nm, print)));
	if (is_nm == 2 || (!tab[6] && cpu == CPU_TYPE_MC98000))
		return ((tab[6] = print_info(av, "mc98000", is_nm, print)));
	if (is_nm == 2 || (!tab[7] && cpu == CPU_TYPE_MC88000))
		return ((tab[7] = print_info(av, "mc88000", is_nm, print)));
	return (print_arch_bis(cpu, av, is_nm, print));
}
