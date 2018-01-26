#include "ft_nm.h"
#include "libft.h"

static void		handle_section(t_data *d, char *ptr, int is_64)
{
	uint32_t	i;
	char		*sectname;
	uint32_t	sect_size;

	d->ptr = ptr;
	d->is_64 = is_64;
	sect_size = is_64 ? sizeof(t_sect64) : sizeof(t_sect32);
	i = -1;
	while (++i < d->nsects)
	{
		if (is_invalid_addr((void *)d->sect + sect_size))
			return ;
		sectname = is_64 ? ((t_sect64 *)d->sect)->sectname
						: ((t_sect32 *)d->sect)->sectname;
		if (is_not_terminated_string(sectname))
			return ;
		if (!ft_strcmp(sectname, SECT_TEXT))
		{
			ft_putendl("Contents of (__TEXT,__text) section");
			if (print_32_64_otool(d))
				return ;
		}
		d->sect++;
	}
}

static int		fill_data(t_data *d, char *ptr, int is_64)
{
	uint32_t	header_size;
	uint32_t	seg_size;

	header_size = is_64 ? sizeof(t_header64) : sizeof(t_header32);
	if (is_invalid_addr((void *)ptr + header_size))
		return (1);
	d->ncmds = is_64 ? ((t_header64 *)ptr)->ncmds : ((t_header32 *)ptr)->ncmds;
	d->seg = is_64 ? ptr + sizeof(t_header64) : ptr + sizeof(t_header32);
	seg_size = is_64 ? sizeof(t_seg64) : sizeof(t_seg32);
	if (is_invalid_addr((void *)d->seg + seg_size))
		return (1);
	d->cmd = is_64 ? ((t_seg64 *)d->seg)->cmd : ((t_seg32 *)d->seg)->cmd;
	return (0);
}

void			handle_32_64(char *ptr, int is_64)
{
	t_data		d;
	uint32_t	i;
	uint32_t	header_size;
	uint32_t	seg_size;

	if (fill_data(&d, ptr, is_64))
		return ;
	header_size = is_64 ? sizeof(t_header64) : sizeof(t_header32);
	seg_size = is_64 ? sizeof(t_seg64) : sizeof(t_seg32);
	i = -1;
	while (++i < d.ncmds)
	{
		if (is_invalid_addr((void *)ptr + header_size)
				|| is_invalid_addr((void *)d.seg + seg_size))
			return ;
		d.filetype = is_64 ? ((t_header64 *)ptr)->filetype
							: ((t_header32 *)ptr)->filetype;
		d.segname = is_64 ? ((t_seg64 *)d.seg)->segname
							: ((t_seg32 *)d.seg)->segname;
		if (is_not_terminated_string(d.segname))
			return ;
		if ((d.cmd == LC_SEGMENT_64 || d.cmd == LC_SEGMENT)
			&& (!ft_strcmp(d.segname, SEG_TEXT) || d.filetype == MH_OBJECT))
		{
			d.nsects = is_64 ? ((t_seg64 *)d.seg)->nsects
							: ((t_seg32 *)d.seg)->nsects;
			d.sect = is_64 ? d.seg + sizeof(t_seg64) : d.seg + sizeof(t_seg32);
			handle_section(&d, ptr, is_64);
			break ;
		}
		d.seg = is_64 ? d.seg + ((t_seg64 *)d.seg)->cmdsize
					: d.seg + ((t_seg32 *)d.seg)->cmdsize;
	}
}
