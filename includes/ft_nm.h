#ifndef FT_NM_H
# define FT_NM_H

#include "loader.h"
#include "nlist.h"
#include "fat.h"
#include "ar.h"

# define PROT		PROT_READ | PROT_WRITE
# define MAP		MAP_PRIVATE

# define OTOOL	1
# define NM		0

# define X64	1
# define X86	0

# define B_ENDIAN	1
# define L_ENDIAN	0

# define BASE16		"0123456789abcdef"

# define NLIST64	((t_nlist64 *)d->array)
# define NLIST32	((t_nlist32 *)d->array)

void		*g_max_addr;

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
	int			is_64;
	char		*ptr;
	uint32_t	ncmds;
	t_lc		*lc;
	t_symtab	*sym;
	void		*array;
	char		*string_table;
	uint64_t	n_value;
	uint8_t		n_type;
	uint8_t		n_sect;
	uint32_t	cmd;
	uint32_t	filetype;
	char		*segname;
	uint32_t	nsects;
	void		*sect;
	void		*seg;
}					t_data;

/*
**	nm
*/
char				get_type(t_data *d, uint32_t i);
void				sort_nlist(t_data *d);
void				print_32_64(t_data *d, uint32_t i);

/*
**	nm & otool
*/
int					handle_main(int ac, char **av, int is_otool);
void				handle_arch(char *ptr, char *av);
void				handle_32_64(char *ptr, int is_64);
void				handle_fat(char *ptr, int is_little_endian);
void				handle_ar(char *ptr, char *av);

/*
**	otool
*/
void				print_32_64_otool(t_data *d);

#endif
