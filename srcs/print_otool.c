#include "ft_nm.h"
#include "libft.h"

static void		print_hexdump(void *ptr, size_t size)
{
	static char		buf[] = "0123456789abcdef";
	unsigned char	c;

	while (size)
	{
		c = *(unsigned char *)ptr;
		ft_putchar(buf[c >> 4]);
		ft_putchar(buf[c & 0xF]);
		ft_putchar(' ');
		ptr++;
		size--;
	}
}

static void		print_addr(uint64_t *addr, int is_64)
{
	size_t		len;

	len = is_64 ? 16 - ft_nlen(*addr, 16) : 8 - ft_nlen(*addr, 16);
	ft_putnchar('0', len);
	ft_putnbr_base(*addr, BASE16);
	*addr += 16;
}

static int		handle_print_addr(void *ptr, uint64_t *addr,
									uint64_t size, int is_64)
{
	if (is_invalid_addr((void *)ptr + size)
			|| is_invalid_addr((void *)*addr + size))
	{
		ft_putendl("here");
		return (1);
	}
	print_addr(addr, is_64);
	ft_putchar('\t');
	print_hexdump(ptr, size);
	ft_putchar('\n');
	return (0);
}

int				print_32_64_otool(t_data *d)
{
	uint64_t	addr;
	uint64_t	size;
	void		*ptr;
	uint32_t	sect_size;

	sect_size = d->is_64 ? sizeof(t_sect64) : sizeof(t_sect32);
	if (is_invalid_addr((void *)d->ptr + sect_size))
		return (1);
	ptr = d->is_64 ? d->ptr + ((t_sect64 *)d->sect)->offset
					: d->ptr + ((t_sect32 *)d->sect)->offset;
	addr = d->is_64 ? ((t_sect64 *)d->sect)->addr : ((t_sect32 *)d->sect)->addr;
	size = d->is_64 ? ((t_sect64 *)d->sect)->size : ((t_sect32 *)d->sect)->size;
	while (size > 16)
	{
		if (handle_print_addr(ptr, &addr, 16, d->is_64))
			return (1);
		ptr += 16;
		size -= 16;
	}
	if (size)
	{
		if (handle_print_addr(ptr, &addr, size, d->is_64))
			return (1);
	}
	return (0);
}
