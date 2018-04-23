/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_corrupted_file.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadiffe <agadiffe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/23 16:44:38 by agadiffe          #+#    #+#             */
/*   Updated: 2018/04/23 16:45:06 by agadiffe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
#include "libft.h"

int				is_not_terminated_string(char *s, char *str)
{
	while ((void *)s < *get_max_addr())
	{
		if (*s == '\0')
			return (0);
		s++;
	}
	ft_putstr_fd("file data error: ", 2);
	ft_putendl_fd(str, 2);
	return (1);
}

int				is_invalid_addr(void *to_check, char *str)
{
	if (to_check > *get_max_addr() || to_check < *get_origin_addr())
	{
		ft_putstr_fd("file data error: ", 2);
		ft_putendl_fd(str, 2);
		return (1);
	}
	return (0);
}
