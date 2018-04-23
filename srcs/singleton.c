/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadiffe <agadiffe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/23 00:49:43 by agadiffe          #+#    #+#             */
/*   Updated: 2018/04/23 16:47:58 by agadiffe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	**get_max_addr(void)
{
	static void		*max_addr;

	return (&max_addr);
}

void	**get_origin_addr(void)
{
	static void		*origin_addr;

	return (&origin_addr);
}

int		is_ar(int set, int value)
{
	static int	n;

	if (set)
		n = value;
	return (n);
}

int		get_ac(int set)
{
	static int	n;

	if (set)
		n = 1;
	return (n);
}
