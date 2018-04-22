/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_otool.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadiffe <agadiffe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/23 00:52:37 by agadiffe          #+#    #+#             */
/*   Updated: 2018/04/23 00:52:46 by agadiffe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int				main(int ac, char **av)
{
	char	*av_tmp[2];

	if (ac < 2)
	{
		av_tmp[1] = "a.out";
		return (handle_main(2, av_tmp, OTOOL));
	}
	return (handle_main(ac, av, OTOOL));
}
