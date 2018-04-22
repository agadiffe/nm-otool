/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadiffe <agadiffe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/23 00:53:13 by agadiffe          #+#    #+#             */
/*   Updated: 2018/04/23 00:59:20 by agadiffe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

uint32_t	swap32(uint32_t value, int swap)
{
	uint32_t x;

	x = 0;
	x |= (value & 0x000000FF) << 24;
	x |= (value & 0x0000FF00) << 8;
	x |= (value & 0x00FF0000) >> 8;
	x |= (value & 0xFF000000) >> 24;
	return (swap ? x : value);
}

uint64_t	swap64(uint64_t value, int swap)
{
	uint64_t x;

	x = value;
	x = (x & 0x00000000FFFFFFFF) << 32 | (x & 0xFFFFFFFF00000000) >> 32;
	x = (x & 0x0000FFFF0000FFFF) << 16 | (x & 0xFFFF0000FFFF0000) >> 16;
	x = (x & 0x00FF00FF00FF00FF) << 8 | (x & 0xFF00FF00FF00FF00) >> 8;
	return (swap ? x : value);
}
