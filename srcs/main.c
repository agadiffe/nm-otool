#include "ft_nm.h"
#include "libft.h"
#include <stdlib.h>

int				main(int ac, char **av)
{
	char	*av_tmp[2];

	if (ac < 2)
	{
		av_tmp[1] = "a.out";
		return (handle_main(2, av_tmp));
	}
	return (handle_main(ac, av));
}
