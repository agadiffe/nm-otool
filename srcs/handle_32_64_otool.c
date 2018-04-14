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
	while (++i < swap32(d->nsects, d->swap))
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
		d->sect = is_64 ? d->sect + swap64(((t_sect64 *)d->sect)->size, d->swap)
					: d->sect + swap32(((t_sect32 *)d->sect)->size, d->swap);
	}
}

static int		handle_segment(t_data *d, char *ptr, int is_64)
{
	uint32_t	seg_size;
	uint32_t	cmd;

	seg_size = is_64 ? sizeof(t_seg64) : sizeof(t_seg32);
	if (is_invalid_addr((void *)d->seg + seg_size))
		return (1);
	d->segname = is_64 ? ((t_seg64 *)d->seg)->segname
					: ((t_seg32 *)d->seg)->segname;
	if (is_not_terminated_string(d->segname))
		return (1);
	cmd = swap32(d->cmd, d->swap);
	if ((cmd == LC_SEGMENT_64 || cmd == LC_SEGMENT)
		&& (!ft_strcmp(d->segname, SEG_TEXT) || d->filetype == MH_OBJECT))
	{
		d->nsects = is_64 ? swap32(((t_seg64 *)d->seg)->nsects, d->swap)
						: swap32(((t_seg32 *)d->seg)->nsects, d->swap);
		d->sect = is_64 ? d->seg + sizeof(t_seg64) : d->seg + sizeof(t_seg32);
		handle_section(d, ptr, is_64);
		return (1);
	}
	d->seg = is_64 ? d->seg + swap32(((t_seg64 *)d->seg)->cmdsize, d->swap)
				: d->seg + swap32(((t_seg32 *)d->seg)->cmdsize, d->swap);
	return (0);
}

static int		fill_data(t_data *d, char *ptr, int is_64)
{
	uint32_t	header_size;
	uint32_t	seg_size;

	header_size = is_64 ? sizeof(t_header64) : sizeof(t_header32);
	if (is_invalid_addr((void *)ptr + header_size))
		return (1);
	d->ncmds = is_64 ? ((t_header64 *)ptr)->ncmds : ((t_header32 *)ptr)->ncmds;
	d->ncmds = swap32(d->ncmds, d->swap);
	d->seg = is_64 ? ptr + sizeof(t_header64) : ptr + sizeof(t_header32);
	seg_size = is_64 ? sizeof(t_seg64) : sizeof(t_seg32);
	if (is_invalid_addr((void *)d->seg + seg_size))
		return (1);
	d->cmd = is_64 ? ((t_seg64 *)d->seg)->cmd : ((t_seg32 *)d->seg)->cmd;
	d->cmd = swap32(d->cmd, d->swap);
	return (0);
}

void			handle_32_64(char *ptr, int is_64)
{
	t_data		d;
	uint32_t	i;
	uint32_t	header_size;

	if (is_invalid_addr((void *)ptr))
		return ;
	if (fill_data(&d, ptr, is_64))
		return ;
	header_size = is_64 ? sizeof(t_header64) : sizeof(t_header32);
	if (is_invalid_addr((void *)ptr + header_size))
		return ;
	d.filetype = is_64 ? swap32(((t_header64 *)ptr)->filetype, d.swap)
						: swap32(((t_header32 *)ptr)->filetype, d.swap);
	i = -1;
	while (++i < d.ncmds)
	{
		if (handle_segment(&d, ptr, is_64))
			return ;
	}
}
