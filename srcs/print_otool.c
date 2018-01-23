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

void			print_32_64_otool(t_data *d)
{
	uint64_t	addr;
	uint64_t	size;
	void		*ptr;

	ptr = d->is_64 ? d->ptr + ((t_sect64 *)d->sect)->offset
					: d->ptr + ((t_sect32 *)d->sect)->offset;
	addr = d->is_64 ? ((t_sect64 *)d->sect)->addr : ((t_sect32 *)d->sect)->addr;
	size = d->is_64 ? ((t_sect64 *)d->sect)->size : ((t_sect32 *)d->sect)->size;
	while (size > 16)
	{
		print_addr(&addr, d->is_64);
		ft_putchar('\t');
		print_hexdump(ptr, 16);
		ft_putchar('\n');
		ptr += 16;
		size -= 16;
	}
	if (size)
	{
		print_addr(&addr, d->is_64);
		ft_putchar('\t');
		print_hexdump(ptr, size);
		ft_putchar('\n');
	}
}
