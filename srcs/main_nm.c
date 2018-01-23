#include "ft_nm.h"
#include "libft.h"
#include <stdlib.h>

int				main(int ac, char **av)
{
	if (ac < 2)
		return (ft_error_ret("no argument!", EXIT_FAILURE));
	return (handle_main(ac, av, NM));
}
