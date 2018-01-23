#include "ft_nm.h"
#include "libft.h"

static void		handle_section(t_data *d, char *ptr, int is_64)
{
	uint32_t	i;
	char		*sectname;

	d->ptr = ptr;
	d->is_64 = is_64;
	i = -1;
	while (++i < d->nsects)
	{
		sectname = is_64 ? ((t_sect64 *)d->sect)->sectname
						: ((t_sect32 *)d->sect)->sectname;
		if (!ft_strcmp(sectname, SECT_TEXT))
		{
			ft_putendl("Contents of (__TEXT,__text) section");
			print_32_64_otool(d);
		}
		d->sect++;
	}
}

void			handle_32_64(char *ptr, int is_64)
{
	t_data		d;
	uint32_t	i;

	d.ncmds = is_64 ? ((t_header64 *)ptr)->ncmds : ((t_header32 *)ptr)->ncmds;
	d.seg = is_64 ? ptr + sizeof(t_header64) : ptr + sizeof(t_header32);
	d.cmd = is_64 ? ((t_seg64 *)d.seg)->cmd : ((t_seg32 *)d.seg)->cmd;
	i = -1;
	while (++i < d.ncmds)
	{
		d.filetype = is_64 ? ((t_header64 *)ptr)->filetype
							: ((t_header32 *)ptr)->filetype;
		d.segname = is_64 ? ((t_seg64 *)d.seg)->segname
							: ((t_seg32 *)d.seg)->segname;
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
