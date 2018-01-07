#include "ft_nm.h"
#include "libft.h"

static int	cmp_sect_name(char *sect, t_data *d, t_lc *lc)
{
	if (d->is_64)
		return (ft_strcmp(sect, ((t_sect64 *)lc)->sectname));
	else
		return (ft_strcmp(sect, ((t_sect32 *)lc)->sectname));
}

static int	handle_sect(char *sect, t_data *d, uint8_t *count, t_lc *lc)
{
	uint32_t		i;
	uint32_t		nsects;

	nsects = d->is_64 ? ((t_seg64 *)lc)->nsects : ((t_seg32 *)lc)->nsects;
	lc = d->is_64 ? (void *)lc + sizeof(t_seg64)
					: (void *)lc + sizeof(t_seg32);
	i = -1;
	while (++i < nsects)
	{
		if (*count == d->n_sect && !cmp_sect_name(sect, d, lc))
			return (1);
		lc = d->is_64 ? (void *)lc + sizeof(t_sect64)
						: (void *)lc + sizeof(t_sect32);
		(*count)++;
	}
	return (0);
}

static int	handle_seg(char *sect, t_data *d)
{
	uint32_t	i;
	int			ret;
	uint8_t		count;
	t_lc		*tmp;

	count = 1;
	ret = 0;
	tmp = d->lc;
	i = -1;
	while (++i < d->ncmds)
	{
		if (tmp->cmd == LC_SEGMENT_64 || tmp->cmd == LC_SEGMENT)
		{
			if ((ret = handle_sect(sect, d, &count, tmp)))
				break ;
		}
		tmp = (void *)tmp + tmp->cmdsize;
	}
	return (ret);
}

static char	get_type_sect(t_data *d)
{
	unsigned char	c;

	if (handle_seg(SECT_TEXT, d))
		c = 't';
	else if (handle_seg(SECT_DATA, d))
		c = 'd';
	else if (handle_seg(SECT_BSS, d))
		c = 'b';
	else
		c = 's';
	return (c);
}

char		get_type(t_data *d, uint32_t i)
{
	unsigned char	c;

	d->n_type = d->is_64 ? NLIST64[i].n_type : NLIST32[i].n_type;
	if (d->is_64)
		d->n_value64 = NLIST64[i].n_value;
	else
		d->n_value32 = NLIST32[i].n_value;
	d->n_sect = d->is_64 ? NLIST64[i].n_sect : NLIST32[i].n_sect;
	c = d->n_type & N_TYPE;
	if (c == N_UNDF)
	{
		if (d->is_64)
			c = d->n_value64 != 0 ? 'c' : 'u';
		else
			c = d->n_value32 != 0 ? 'c' : 'u';
	}
	else if (c == N_ABS)
		c = 'a';
	else if (c == N_SECT)
		c = get_type_sect(d);
	else if (c == N_INDR)
		c = 'i';
	else
		c = '?';
	if (d->n_type & N_EXT && c != '?')
		c = ft_toupper(c);
	return (c);
}
