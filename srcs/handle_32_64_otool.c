#include "ft_nm.h"
#include "libft.h"

static int		handle_section(t_data *d)
{
	uint32_t	i;
	char		*sectname;
	uint32_t	sect_size;

	sect_size = d->is_64 ? sizeof(t_sect64) : sizeof(t_sect32);
	i = -1;
	while (++i < d->nsects)
	{
		if (is_invalid_addr((void *)d->sect + sect_size, "section"))
			return (1);
		sectname = d->is_64 ? ((t_sect64 *)d->sect)->sectname
							: ((t_sect32 *)d->sect)->sectname;
		if (is_not_terminated_string(sectname, "section name"))
			return (1);
		if (!ft_strcmp(sectname, SECT_TEXT))
		{
			if (print_32_64_otool(d))
				return (1);
		}
		d->sect = d->sect + sect_size;
	}
	return (0);
}

static int		handle_segment(t_data *d)
{
	uint32_t	seg_size;

	seg_size = d->is_64 ? sizeof(t_seg64) : sizeof(t_seg32);
	if (is_invalid_addr((void *)d->seg + seg_size, "segment"))
		return (1);
	d->segname = d->is_64 ? ((t_seg64 *)d->seg)->segname
						: ((t_seg32 *)d->seg)->segname;
	if (is_not_terminated_string(d->segname, "segment name"))
		return (1);
	d->cmd = d->is_64 ? ((t_seg64 *)d->seg)->cmd : ((t_seg32 *)d->seg)->cmd;
	d->cmd = swap32(d->cmd, d->swap);
	if ((d->cmd == LC_SEGMENT_64 || d->cmd == LC_SEGMENT)
			&& !ft_strcmp(d->segname, SEG_TEXT))
	{
		d->nsects = d->is_64 ? swap32(((t_seg64 *)d->seg)->nsects, d->swap)
							: swap32(((t_seg32 *)d->seg)->nsects, d->swap);
		d->sect = d->is_64 ? (void *)d->seg + sizeof(t_seg64)
							: (void *)d->seg + sizeof(t_seg32);
		if (handle_section(d))
			return (1);
	}
	d->seg = d->is_64 ? d->seg + swap32(((t_seg64 *)d->seg)->cmdsize, d->swap)
					: d->seg + swap32(((t_seg32 *)d->seg)->cmdsize, d->swap);
	return (0);
}

static int		fill_data(t_data *d, char *ptr, int is_64)
{
	uint32_t		header_size;
	unsigned int	magic;

	magic = *(unsigned int *)ptr;
	d->display = 1;
	d->swap = magic == MH_CIGAM || magic == MH_CIGAM_64 ? 1 : 0;
	header_size = is_64 ? sizeof(t_header64) : sizeof(t_header32);
	if (is_invalid_addr((void *)ptr + header_size, "ptr + header"))
		return (1);
	d->ptr = ptr;
	d->is_64 = is_64;
	d->ncmds = is_64 ? ((t_header64 *)ptr)->ncmds : ((t_header32 *)ptr)->ncmds;
	d->ncmds = swap32(d->ncmds, d->swap);
	d->seg = is_64 ? (void *)ptr + sizeof(t_header64)
					: (void *)ptr + sizeof(t_header32);
	d->filetype = is_64 ? swap32(((t_header64 *)ptr)->filetype, d->swap)
						: swap32(((t_header32 *)ptr)->filetype, d->swap);
	d->cpu = is_64 ? swap32(((t_header64 *)ptr)->cputype, d->swap)
						: swap32(((t_header32 *)ptr)->cputype, d->swap);
	return (0);
}

void			handle_32_64(char *ptr, int is_64, char *av, int print)
{
	t_data		d;
	uint32_t	i;

	if (fill_data(&d, ptr, is_64))
		return ;
	d.display = print_arch(d.cpu, av, OTOOL, print);
	i = -1;
	while (++i < d.ncmds)
	{
		if (handle_segment(&d))
			return ;
	}
}
