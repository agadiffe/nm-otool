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
		d->sect = (void *)d->sect + sect_size;
	}
	return (0);
}

static int		handle_segment(t_data *d)
{
	if (is_invalid_addr((void *)d->lc + sizeof(t_lc), "segment"))
		return (1);
	d->segname = d->is_64 ? ((t_seg64 *)d->lc)->segname
						: ((t_seg32 *)d->lc)->segname;
	if (is_not_terminated_string(d->segname, "segment name"))
		return (1);
	d->cmd = d->is_64 ? ((t_seg64 *)d->lc)->cmd : ((t_seg32 *)d->lc)->cmd;
	d->cmd = swap32(d->cmd, d->swap);
	if ((d->cmd == LC_SEGMENT_64 || d->cmd == LC_SEGMENT)
			&& (!ft_strcmp(d->segname, SEG_TEXT) || d->filetype == MH_OBJECT))
	{
		d->nsects = d->is_64 ? swap32(((t_seg64 *)d->lc)->nsects, d->swap)
							: swap32(((t_seg32 *)d->lc)->nsects, d->swap);
		d->sect = d->is_64 ? (void *)d->lc + sizeof(t_seg64)
							: (void *)d->lc + sizeof(t_seg32);
		if (handle_section(d))
			return (1);
	}
	d->lc = (void *)d->lc + swap32(((t_lc *)d->lc)->cmdsize, d->swap);
	return (0);
}

static int		fill_data(t_data *d, char *ptr)
{
	uint32_t		header_size;
	unsigned int	magic;

	magic = *(unsigned int *)ptr;
	d->ptr = ptr;
	d->is_64 = magic == MH_MAGIC_64 || magic == MH_CIGAM_64 ? 1 : 0;
	d->display = 1;
	d->swap = magic == MH_CIGAM || magic == MH_CIGAM_64 ? 1 : 0;
	header_size = d->is_64 ? sizeof(t_header64) : sizeof(t_header32);
	if (is_invalid_addr((void *)ptr + header_size, "ptr + header"))
		return (1);
	d->ncmds = d->is_64 ? ((t_header64 *)ptr)->ncmds
						: ((t_header32 *)ptr)->ncmds;
	d->ncmds = swap32(d->ncmds, d->swap);
	d->lc = d->is_64 ? (void *)ptr + sizeof(t_header64)
					: (void *)ptr + sizeof(t_header32);
	d->filetype = d->is_64 ? swap32(((t_header64 *)ptr)->filetype, d->swap)
						: swap32(((t_header32 *)ptr)->filetype, d->swap);
	d->cpu = d->is_64 ? swap32(((t_header64 *)ptr)->cputype, d->swap)
					: swap32(((t_header32 *)ptr)->cputype, d->swap);
	return (0);
}

int				handle_32_64_otool(char *ptr, char *av, int print, int is_nm)
{
	t_data		d;
	uint32_t	i;

	if (handle_32_64_nm(ptr, av, -2, 42))
		return (0);
	if (fill_data(&d, ptr))
		return (1);
	d.display = print_arch(d.cpu, av, is_nm, print);
	i = -1;
	while (++i < d.ncmds)
	{
		if (handle_segment(&d))
			return (1);
	}
	return (0);
}
