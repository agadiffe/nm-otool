void	**get_max_addr(void)
{
	static void		*max_addr;

	return (&max_addr);
}

void	**get_origin_addr(void)
{
	static void		*origin_addr;

	return (&origin_addr);
}

int		is_ar(int set, int value)
{
	static int	n;

	if (set)
		n = value;
	return (n);
}
