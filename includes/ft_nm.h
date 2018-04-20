#ifndef FT_NM_H
# define FT_NM_H

# include "loader.h"
# include "nlist.h"
# include "fat.h"
# include "ar.h"

# define HOST_CPU	CPU_TYPE_X86_64

# define PROT		PROT_READ | PROT_WRITE
# define MAP		MAP_PRIVATE

# define NM		1
# define OTOOL	0

# define X64	1
# define X86	0

# define BASE16		"0123456789abcdef"

# define NLIST64	((t_nlist64 *)d->array)
# define NLIST32	((t_nlist32 *)d->array)

void		*g_max_addr;
void		*g_origin_addr;

typedef struct load_command			t_lc;
typedef struct symtab_command		t_symtab;
typedef struct mach_header_64		t_header64;
typedef struct mach_header			t_header32;
typedef struct fat_header			t_headerfat;
typedef struct fat_arch				t_arch;
typedef struct ar_hdr				t_ar;
typedef struct ranlib				t_ranlib;
typedef struct nlist_64				t_nlist64;
typedef struct nlist				t_nlist32;
typedef struct segment_command_64	t_seg64;
typedef struct segment_command		t_seg32;
typedef struct section_64			t_sect64;
typedef struct section				t_sect32;

typedef struct		s_data
{
	t_lc		*lc;
	t_symtab	*sym;
	char		*string_table;
	char		*ptr;
	char		*segname;
	int			cpu;
	int			is_64;
	int			swap;
	int			display;
	uint32_t	cmd;
	uint32_t	cmdsize;
	uint32_t	ncmds;
	uint64_t	n_value;
	uint32_t	filetype;
	uint32_t	nsects;
	uint8_t		n_type;
	uint8_t		n_sect;
	void		*array;
	void		*sect;
	void		*seg;
}					t_data;

/*
**	nm & otool
*/
int				handle_main(int ac, char **av);
void			handle_arch(char *ptr, char *av, int print);
void			handle_32_64(char *ptr, int is_64, char *av, int print);
void			handle_fat(char *ptr, char *av);
void			handle_ar(char *ptr, char *av);
int				is_invalid_addr(void *to_check, char *str);
int				is_not_terminated_string(char *s, char *str);
uint32_t		swap32(uint32_t value, int swap);
uint64_t		swap64(uint64_t value, int swap);
int				print_arch(int cpu, char *av, int is_nm, int print);
int				check_fat_host_arch(char *ptr, uint32_t n_fatarch, int swap);
int				is_32_or_64(char *ptr);
int				get_cpu(char *ptr);

/*
**	nm
*/
char			get_type(t_data *d, uint32_t i);
int				sort_nlist(t_data *d);
int				sort_nlist_same_name(t_data *d);
int				print_32_64(t_data *d, uint32_t i);

/*
**	otool
*/
int				print_32_64_otool(t_data *d);

#endif
