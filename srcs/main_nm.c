/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_nm.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadiffe <agadiffe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/23 00:52:21 by agadiffe          #+#    #+#             */
/*   Updated: 2018/04/23 00:52:33 by agadiffe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int				main(int ac, char **av)
{
	char	*av_tmp[2];

	if (ac < 2)
	{
		av_tmp[1] = "a.out";
		return (handle_main(2, av_tmp, NM));
	}
	return (handle_main(ac, av, NM));
}
