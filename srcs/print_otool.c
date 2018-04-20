#include "ft_nm.h"
#include "libft.h"

static void		print_hexdump(void *ptr, size_t size, int cpu)
{
	static char		buf[] = "0123456789abcdef";
	unsigned char	c;
	int				ppc;

	ppc = cpu == CPU_TYPE_POWERPC || cpu == CPU_TYPE_POWERPC64 ? 1 : 0;
	while (size)
	{
		c = *(unsigned char *)ptr;
		ft_putchar(buf[c >> 4]);
		ft_putchar(buf[c & 0xF]);
		ptr++;
		size--;
		if (!ppc || size % 4 == 0)
			ft_putchar(' ');
	}
}

static void		print_addr(uint64_t addr, int is_64)
{
	size_t		len;

	len = is_64 ? 16 - ft_nlen(addr, 16) : 8 - ft_nlen(addr, 16);
	ft_putnchar('0', len);
	ft_putnbr_base(addr, BASE16);
}

static int		handle_print_addr(void *ptr, uint64_t addr,
									uint64_t size, t_data *d)
{
	if (is_invalid_addr((void *)ptr + size, "print_otool ptr + size")
			|| is_invalid_addr(g_origin_addr + (uint32_t)addr + size,
								"print_otool addr + size"))
		return (1);
	if (d->display)
	{
		print_addr(addr, d->is_64);
		ft_putchar('\t');
		print_hexdump(ptr, size, d->cpu);
		ft_putchar('\n');
	}
	return (0);
}

static void		fill_data(t_data *d, void **ptr, uint64_t *addr, uint64_t *size)
{
	*ptr = d->is_64 ? d->ptr + swap32(((t_sect64 *)d->sect)->offset, d->swap)
					: d->ptr + swap32(((t_sect32 *)d->sect)->offset, d->swap);
	*addr = d->is_64 ? swap64(((t_sect64 *)d->sect)->addr, d->swap)
					: swap32(((t_sect32 *)d->sect)->addr, d->swap);
	*size = d->is_64 ? swap64(((t_sect64 *)d->sect)->size, d->swap)
					: swap32(((t_sect32 *)d->sect)->size, d->swap);
}

int				print_32_64_otool(t_data *d)
{
	uint64_t	addr;
	uint64_t	size;
	void		*ptr;
	uint32_t	sect_size;

	if (d->display)
		ft_putendl("Contents of (__TEXT,__text) section");
	sect_size = d->is_64 ? sizeof(t_sect64) : sizeof(t_sect32);
	if (is_invalid_addr((void *)d->ptr + sect_size, "print_otool ptr + header"))
		return (1);
	fill_data(d, &ptr, &addr, &size);
	while (size > 16)
	{
		if (handle_print_addr(ptr, addr, 16, d))
			return (1);
		addr += 16;
		ptr += 16;
		size -= 16;
	}
	if (size > 0)
	{
		if (handle_print_addr(ptr, addr, size, d))
			return (1);
	}
	return (0);
}
