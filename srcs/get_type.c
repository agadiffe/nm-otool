#include "ft_nm.h"
#include "libft.h"

static int	cmp_sect_name(char *sect, t_ndata *ndata, int is_64)
{
	if (is_64)
		return (ft_strcmp(sect, ((t_sect64 *)ndata->lc)->sectname));
	else
		return (ft_strcmp(sect, ((t_sect32 *)ndata->lc)->sectname));
}

static int	handle_sect(char *sect, t_ndata *ndata, uint8_t *count, int is_64)
{
	uint32_t		i;
	uint32_t		nsects;

	i = 0;
	nsects = is_64 ? ((t_seg64 *)ndata->lc)->nsects
					: ((t_seg32 *)ndata->lc)->nsects;
	ndata->lc = is_64 ? (void *)ndata->lc + sizeof(t_seg64)
					: (void *)ndata->lc + sizeof(t_seg32);
	while (i != nsects)
	{
		if (*count == ndata->n_sect && !cmp_sect_name(sect, ndata, is_64))
			return (1);
		ndata->lc = is_64 ? (void *)ndata->lc + sizeof(t_sect64)
						: (void *)ndata->lc + sizeof(t_sect32);
		(*count)++;
		++i;
	}
	return (0);
}

static int	handle_seg(char *sect, t_ndata *ndata, int is_64)
{
	uint32_t	i;
	int			ret;
	uint8_t		count;

	i = 0;
	count = 1;
	ret = 0;
	while (i != ndata->ncmds)
	{
		if (ndata->lc->cmd == LC_SEGMENT_64 || ndata->lc->cmd == LC_SEGMENT)
		{
			if ((ret = handle_sect(sect, ndata, &count, is_64)) == 1)
				break ;
		}
		ndata->lc = (void *)ndata->lc + ndata->lc->cmdsize;
		i++;
	}
	return (ret);
}

static char	get_type_sect(t_ndata *ndata, int is_64)
{
	unsigned char	c;

	if (handle_seg(SECT_TEXT, ndata, is_64) == 1)
		c = 't';
	if (handle_seg(SECT_DATA, ndata, is_64) == 1)
		c = 'd';
	if (handle_seg(SECT_BSS, ndata, is_64) == 1)
		c = 'b';
	else
		c = 's';
	return (c);
}

char		get_type(void *array, uint32_t i, void *ptr, int is_64)
{
	unsigned char	c;
	t_ndata			ndata;

	ndata.n_type = is_64 ? NLIST64[i].n_type : NLIST32[i].n_type;
	ndata.n_value = is_64 ? NLIST64[i].n_value : NLIST32[i].n_value;
	ndata.n_sect = is_64 ? NLIST64[i].n_sect : NLIST32[i].n_sect;
	ndata.lc = is_64 ? (void *)ptr + sizeof(t_header64)
					: (void *)ptr + sizeof(t_header32);
	ndata.ncmds = is_64 ? ((t_header64 *)ptr)->ncmds
						: ((t_header32 *)ptr)->ncmds;
	c = ndata.n_type & N_TYPE;
	if (c == N_UNDF)
		c = ndata.n_value != 0 ? 'c' : 'u';
	else if (c == N_ABS)
		c = 'a';
	else if (c == N_SECT)
		c = get_type_sect(&ndata, is_64);
	else if (c == N_INDR)
		c = 'i';
	else
		c = '?';
	if (ndata.n_type & N_EXT && c != '?')
		c = ft_toupper(c);
	return (c);
}
