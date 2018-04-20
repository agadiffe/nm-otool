#include "machine.h"
#include "libft.h"

static void		print_arch(char *av, char *arch)
{
	ft_putstr(av);
	ft_putstr(" (for architecture ");
	ft_putstr(arch);
	ft_putendl("):");
}

int				check_duplicate_print_arch(int cpu, char *av)
{
	static int	tab[4];

	if (!tab[0] && cpu == CPU_TYPE_X86)
	{
		print_arch(av, "i386");
		return ((tab[0] = 1));
	}
	if (!tab[1] && cpu == CPU_TYPE_POWERPC)
	{
		print_arch(av, "ppc");
		return ((tab[1] = 1));
	}
	if (!tab[2] && cpu == CPU_TYPE_X86_64)
	{
		print_arch(av, "x86_64");
		return ((tab[2] = 1));
	}
	if (!tab[3] && cpu == CPU_TYPE_POWERPC64)
	{
		print_arch(av, "ppc64");
		return ((tab[3] = 1));
	}
	return (0);
}
